#include <dlfcn.h>
#include <sys/types.h>
#include <stdatomic.h>
#include <SDL2/SDL.h>

#include "sdl_bindings.h"
#include "utils.h"
#include "offsets.h"


#define VERSION "1.0.1"
#define GAME_BUILD "4.1.1.7209685"

#define ROLL_SENSITIVITY 2.f
#define ZOOM_FACTOR 0.25f

#define CONTROLLER_ROLL_SPEED 2.f
#define CONTROLLER_DEADZONE 4000


static pid_t g_pid = 0;
static uint64_t g_base;

static float* g_zoom;
static float* g_roll;

static atomic_int g_mouse_delta_y;
static atomic_int g_mouse_wheel_y;
static atomic_int g_roll_keydown = 0;
static atomic_int g_controller_right_stick_y;
static atomic_int g_controller_right_stick_axis_motion_y = 0;
static atomic_int g_controller_right_stick_button_down = 0;

static BindingSet g_bs;
static ActionBindings* g_binds[1];
	 
static int (*O_PollEvent)(SDL_Event*) = NULL;
static int (*O_SDL_GetRelativeMouseMode)() = NULL;
static void (*O_SDL_SetRelativeMouseMode)(int) = NULL;

typedef float (*CalculateCameraAngle_t)(void*, uint8_t);
static CalculateCameraAngle_t O_CalculateCameraAngle;
typedef uint8_t undefined8[8];
typedef void (*SaveToInputConfigFile_t)(undefined8, undefined8, undefined8, undefined8, undefined8, undefined8,
		undefined8, undefined8, long*, long*, mbstate_t, uint*, mbstate_t, undefined8);
static SaveToInputConfigFile_t O_SaveToInputConfigFile;


void H_SaveToInputConfigFile_CallSite(undefined8 p1, undefined8 p2, undefined8 p3, undefined8 p4, undefined8 p5, undefined8 p6,
		undefined8 p7, undefined8 p8, long* p9, long* p10, mbstate_t p11, uint* p12, mbstate_t p13, undefined8 p14)
{
	O_SaveToInputConfigFile(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14);

	if (!LoadBindingsFromFile("~/.local/share/Larian Studios/Baldur's Gate 3/PlayerProfiles/Public/inputconfig_p1.json", &g_bs))
	{
    	fprintf(stderr, "\e[1;95m[LNCT]\e[0m ERR: H_SaveToInputConfigFile_CallSite(): Couldn't load ~/.local/share/Larian Studios/Baldur's Gate 3/PlayerProfiles/Public/inputconfig_p1.json\n");
    	return;
	}
	g_binds[0] = (ActionBindings*)FindAction(&g_bs, "CameraToggleMouseRotate");
	if (!g_binds[0])
	{
    	fprintf(stderr, "\e[1;95m[LNCT]\e[0m ERR: Can't find any bind(s) for action \"CameraToggleMouseRotate\" in ~/.local/share/Larian Studios/Baldur's Gate 3/PlayerProfiles/Public/inputconfig_p1.json\n");
	}
}

float H_CalculateCameraAngle_CallSite(void* pCameraObject, uint8_t angle)
{
	g_zoom = (float*)((uint8_t*)pCameraObject + 0x58);
	int right_stick_button_down = atomic_load(&g_controller_right_stick_button_down);
	int right_stick_axis_motion_y = atomic_load(&g_controller_right_stick_axis_motion_y);
	int right_stick_y;
	if (right_stick_button_down && right_stick_axis_motion_y)
	{
		int right_stick_y = atomic_load(&g_controller_right_stick_y);
		if (right_stick_y > -CONTROLLER_DEADZONE && right_stick_y < CONTROLLER_DEADZONE)
			right_stick_y = 0;

		*g_zoom += -((float)right_stick_y / 32767.f * ZOOM_FACTOR);
		return O_CalculateCameraAngle(pCameraObject, angle);
	}
	else
	{
		int zoom = atomic_exchange(&g_mouse_wheel_y, 0);
		*g_zoom += -((float)zoom * ZOOM_FACTOR);
	}

	int roll_keydown = atomic_load(&g_roll_keydown);
	if (!roll_keydown && !right_stick_axis_motion_y)
	{
		if (O_SDL_GetRelativeMouseMode() == SDL_TRUE)
			O_SDL_SetRelativeMouseMode(SDL_FALSE);
		return O_CalculateCameraAngle(pCameraObject, angle);
	}
	if (O_SDL_GetRelativeMouseMode() != SDL_TRUE)
		O_SDL_SetRelativeMouseMode(SDL_TRUE);

	g_roll = (float*)((uint8_t*)pCameraObject + 0x164);
	float roll = *g_roll;

	if (right_stick_axis_motion_y)
	{
		int val = atomic_load(&g_controller_right_stick_y);
		if (val > -CONTROLLER_DEADZONE && val < CONTROLLER_DEADZONE)
			val = 0;

		float norm = (float)val / 32767.f;
		roll += norm * CONTROLLER_ROLL_SPEED;
	}
	else
	{
		int delta = atomic_exchange(&g_mouse_delta_y, 0);
		roll += (float)delta * ROLL_SENSITIVITY;
	}

	if (roll > 89.f)
		roll = 89.f;
	else
		roll = roll;
	if (roll < -89.f)
		roll = -89.f;
	else
		roll = roll;
	*g_roll = roll;

	return O_CalculateCameraAngle(pCameraObject, angle);
}

uint8_t PatchUpdateCamera()
{
	void* movss = (void*)(g_base + Offsets.instr_roll_movss);
	long page_size = sysconf(_SC_PAGESIZE);
	uint64_t page_start = (uint64_t)movss & ~(page_size - 1);
	size_t num_pages = (((uint64_t)movss + 8 - page_start) + page_size - 1) / page_size;
	if (num_pages < 1)
		num_pages = 1;

	if (mprotect((void*)page_start, num_pages * page_size, PROT_READ | PROT_WRITE | PROT_EXEC) != 0)
	{
		fprintf(stdout, "\e[1;95m[LNCT]\e[0m ERR: PatchUpdateCamera(): roll movss mprotect() failed\n");
		return 0;
	}

	uint8_t nop[8] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
	memcpy(movss, nop, 8);
	mprotect((void*)page_start, num_pages * page_size, PROT_READ | PROT_EXEC);

	movss = (void*)(g_base + Offsets.instr_zoom_movss);
	page_start = (uint64_t)movss & ~(page_size - 1);
	num_pages = (((uint64_t)movss + 6 - page_start) + page_size - 1) / page_size;
	if (num_pages < 1)
		num_pages = 1;

	if (mprotect((void*)page_start, num_pages * page_size, PROT_READ | PROT_WRITE | PROT_EXEC) != 0)
	{
		fprintf(stdout, "\e[1;95m[LNCT]\e[0m ERR: PatchUpdateCamera(): zoom movss mprotect() failed\n");
		return 0;
	}

	memcpy(movss, nop, 6);
	mprotect((void*)page_start, num_pages * page_size, PROT_READ | PROT_EXEC);

	return 1;
}

uint8_t SetupCallSitesTrampoline()
{
	size_t page_size = sysconf(_SC_PAGESIZE);

	void* callsite = (void*)(g_base + Offsets.CalculateCameraAngle_CallSite);
	void* trampoline = AllocNear(callsite, page_size);
	if (!trampoline)
	{
		fprintf(stderr, "\e[1;95m[LNCT]\e[0m ERR: SetupCallSitesTrampoline(): AllocNear() failed for CalculateCameraAngle()\n");
		return 0;
	}

	if (!PatchCallSite(callsite, trampoline, H_CalculateCameraAngle_CallSite))
	{
		fprintf(stderr, "\e[1;95m[LNCT]\e[0m ERR: SetupCallSitesTrampoline(): PatchCallSite() failed for CalculateCameraAngle()\n");
		munmap(trampoline, page_size);
		return 0;
	}

	callsite = (void*)(g_base + Offsets.SaveToInputConfigFile_CallSite);
	trampoline = AllocNear(callsite, page_size);
	if (!trampoline)
	{
		fprintf(stderr, "\e[1;95m[LNCT]\e[0m ERR: SetupCallSitesTrampoline(): AllocNear() failed for SaveToInputConfigFile()\n");
		return 0;
	}

	if (!PatchCallSite(callsite, trampoline, H_SaveToInputConfigFile_CallSite))
	{
		fprintf(stderr, "\e[1;95m[LNCT]\e[0m ERR: SetupCallSitesTrampoline(): PatchCallSite() failed for SaveToInputConfigFile()\n");
		munmap(trampoline, page_size);
		return 0;
	}

	return 1;
}

void Setup()
{
	g_pid = getpid();
	fprintf(stdout, "\e[1;95m[LNCT]\e[0m \e[1;37mLinux Native Camera Tweaks %s | Made for build %s\e[0m\n", VERSION, GAME_BUILD);
	fprintf(stdout, "\e[1;95m[LNCT]\e[0m Bug(s) ? Suggestion(s) ? Add me on discord: biiinks78\n");

	if (!LoadBindingsFromFile("~/.local/share/Larian Studios/Baldur's Gate 3/PlayerProfiles/Public/inputconfig_p1.json", &g_bs))
	{
    	fprintf(stderr, "\e[1;95m[LNCT]\e[0m ERR: Couldn't load ~/.local/share/Larian Studios/Baldur's Gate 3/PlayerProfiles/Public/inputconfig_p1.json\n");
    	return;
	}
	g_binds[0] = (ActionBindings*)FindAction(&g_bs, "CameraToggleMouseRotate");
	if (!g_binds[0])
	{
    	fprintf(stderr, "\e[1;95m[LNCT]\e[0m ERR: Can't find any bind(s) for action \"CameraToggleMouseRotate\" in ~/.local/share/Larian Studios/Baldur's Gate 3/PlayerProfiles/Public/inputconfig_p1.json\n");
    	return;
	}

	g_base = GetModuleBase(0);

	if (!PatchUpdateCamera())
	{
		fprintf(stderr, "\e[1;95m[LNCT]\e[0m ERR: Setup(): PatchUpdateCamera() failed\n");
		return;
	}

	O_CalculateCameraAngle = ResolveCallTarget((void*)(g_base + Offsets.CalculateCameraAngle_CallSite));
	O_SaveToInputConfigFile = ResolveCallTarget((void*)(g_base + Offsets.SaveToInputConfigFile_CallSite));

	if (!SetupCallSitesTrampoline())
	{
		fprintf(stderr, "\e[1;95m[LNCT]\e[0m ERR: Setup(): SetupCallSitesTrampoline() failed, please double check your game build and compare it to whats shown compatible on the nexus page\n");
		return;
	}

	fprintf(stdout, "\e[1;95m[LNCT]\e[0m \e[1;92mEverything has been initialized correctly, enjoy ;)\e[0m\n");
}

void ResolveSDLSym()
{
	O_PollEvent = (int(*)(SDL_Event*))(intptr_t)dlsym(RTLD_NEXT, "SDL_PollEvent");
	O_SDL_GetRelativeMouseMode = (int(*)(SDL_Event*))(intptr_t)dlsym(RTLD_NEXT, "SDL_GetRelativeMouseMode");
	O_SDL_SetRelativeMouseMode = (void(*)(int))(intptr_t)dlsym(RTLD_NEXT, "SDL_SetRelativeMouseMode");
}

int SDL_PollEvent(SDL_Event* event)
{
	if (!g_pid)
		Setup();

	if (!O_PollEvent)
		ResolveSDLSym();

	int ret = O_PollEvent(event);
	if (ret && event)
	{
		if (event->type == SDL_MOUSEMOTION)
			atomic_store(&g_mouse_delta_y, event->motion.yrel);
		else if (event->type == SDL_MOUSEWHEEL)
		{
			atomic_store(&g_mouse_wheel_y, event->wheel.y);
			return 0; 
		}
		else if (event->type == SDL_MOUSEBUTTONDOWN)
		{
			for (int i = 0; i < g_binds[0]->binding_count; i++)
			{
    			const Binding* b = &g_binds[0]->bindings[i];
				if ((b->type == BINDING_MOUSE) && event->button.button == b->mouse_button)
					atomic_store(&g_roll_keydown, 1);
			}
		}
		else if (event->type == SDL_MOUSEBUTTONUP)
		{
			for (int i = 0; i < g_binds[0]->binding_count; i++)
			{
    			const Binding* b = &g_binds[0]->bindings[i];
				if ((b->type == BINDING_MOUSE) && event->button.button == b->mouse_button)
					atomic_store(&g_roll_keydown, 0);
			}
		}
		else if (event->type == SDL_KEYDOWN)
		{
			for (int i = 0; i < g_binds[0]->binding_count; i++)
			{
    			const Binding* b = &g_binds[0]->bindings[i];
				if ((b->type == BINDING_KEY) && event->key.keysym.scancode == b->scancode)
					atomic_store(&g_roll_keydown, 1);
			}
		}
		else if (event->type == SDL_KEYUP)
		{
			for (int i = 0; i < g_binds[0]->binding_count; i++)
			{
    			const Binding* b = &g_binds[0]->bindings[i];
				if ((b->type == BINDING_KEY) && event->key.keysym.scancode == b->scancode)
					atomic_store(&g_roll_keydown, 0);
			}
		}
		else if ((event->type == SDL_CONTROLLERAXISMOTION) && event->caxis.axis == SDL_CONTROLLER_AXIS_RIGHTY)
		{
			int16_t val = event->caxis.value;
			if (val > CONTROLLER_DEADZONE || val < -CONTROLLER_DEADZONE)
        	{
				atomic_store(&g_controller_right_stick_axis_motion_y, 1);
				atomic_store(&g_controller_right_stick_y, val);
			}
			else
        		atomic_store(&g_controller_right_stick_axis_motion_y, 0);
			return 0;
		}
		else if ((event->type == SDL_CONTROLLERBUTTONDOWN) && event->cbutton.button == SDL_CONTROLLER_BUTTON_RIGHTSTICK)
		{
			atomic_store(&g_controller_right_stick_button_down, 1);
		}
		else if ((event->type == SDL_CONTROLLERBUTTONUP) && event->cbutton.button == SDL_CONTROLLER_BUTTON_RIGHTSTICK)
		{
			atomic_store(&g_controller_right_stick_button_down, 0);
		}
	}

	return ret;
}
