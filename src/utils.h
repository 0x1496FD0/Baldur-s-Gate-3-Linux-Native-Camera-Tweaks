#pragma once

#include <sys/mman.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <link.h>


typedef struct
{
    const char* substr;
    uint64_t base;
} ModuleBaseCtx;


static inline int ModuleBase_Callback(struct dl_phdr_info* info, size_t size, void* data)
{
    (void)size;
    ModuleBaseCtx* ctx = (ModuleBaseCtx*)data;
    if (info->dlpi_name[0] == '\0' ||
        (ctx->substr && strstr(info->dlpi_name, ctx->substr)))
	{
        ctx->base = info->dlpi_addr;
        return 1;
    }
    return 0;
}

static uint64_t GetModuleBase(const char* substr)
{
    ModuleBaseCtx ctx = { substr, 0 };
    dl_iterate_phdr(ModuleBase_Callback, &ctx);
    return ctx.base;
}

static void* AllocNear(void* target, size_t size)
{
	size_t page_size = sysconf(_SC_PAGESIZE);
	uint64_t target_page = (uint64_t)target & ~(page_size - 1);

	for (int64_t delta = page_size; delta < 0x60000000; delta += page_size)
	{
		void* candidates[2] =
		{
			(void*)(target_page + delta),
			(void*)(target_page - delta)
		};
		for (int i = 0; i < 2; i++)
		{
			void* p = mmap(candidates[i], size, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED_NOREPLACE, -1, 0);
			if (p != MAP_FAILED)
				return p;
		}
	}
	return NULL;
}

static void* ResolveCallTarget(void* callsite)
{
	uint8_t* cs = (uint8_t*)callsite;
	if (cs[0] != 0xE8)
	{
		fprintf(stderr, "\e[1;95m[LNCT]\e[0m ERR: ResolveCallTarget(): instruction isn't a rel32 call %#x at %p\n", cs[0], callsite);
		return NULL;
	}
	int32_t orig_rel;
	memcpy(&orig_rel, cs + 1, 4);
	return (void*)(cs + 5 + orig_rel);
}

static void* PatchCallSite(void* callsite, void* trampoline, void* trampoline_target)
{
	uint8_t* cs = (uint8_t*)callsite;
	if (cs[0] != 0xE8)
	{

		fprintf(stderr, "\e[1;95m[LNCT]\e[0m ERR: PatchCallSite(): instruction isn't a rel32 call %#x at %p\n", cs[0], callsite);
		return NULL;
	}

	int32_t orig_rel;
	memcpy(&orig_rel, cs + 1, 4);
	void* original_target = (void*)(cs + 5 + orig_rel);

	int64_t rel_to_tramp = (int64_t)((uint8_t*)trampoline - (cs + 5));
	if (rel_to_tramp > INT32_MAX || rel_to_tramp < INT32_MIN)
	{
		fprintf(stderr, "\e[1;95m[LNCT]\e[0m ERR: PatchCallSite(): trampoline addy isn't in range for a rel32 jmp\n");
		return NULL;
	}

	uint8_t* t = (uint8_t*)trampoline;
	size_t off = 0;

	t[off++] = 0x48;
	t[off++] = 0xB8;
	memcpy(t + off, &trampoline_target, 8); off += 8;

	t[off++] = 0xFF; t[off++] = 0xE0;

	long page_size = sysconf(_SC_PAGESIZE);
	uint64_t page_start = (uint64_t)cs & ~(page_size - 1);
	size_t num_pages = (((uint64_t)cs + 5 - page_start) + page_size - 1) / page_size;
	if (num_pages < 1)
		num_pages = 1;

	if (mprotect((void*)page_start, num_pages * page_size, PROT_READ | PROT_WRITE | PROT_EXEC) != 0)
	{
		fprintf(stderr, "\e[1;95m[LNCT]\e[0m ERR: PatchCallSite(): mprotect() failed on page %p for callsite %p\n", (void*)page_start, cs);
		return NULL;
	}

	int32_t new_rel = (int32_t)rel_to_tramp;
	memcpy(cs + 1, &new_rel, 4);

	mprotect((void*)page_start, num_pages * page_size, PROT_READ | PROT_EXEC);
	return original_target;
}
