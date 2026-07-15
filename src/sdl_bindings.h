#pragma once

#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_mouse.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct
{
    const char* name;
    SDL_Scancode code;
} ScancodeName;
static const ScancodeName kScancodeNames[] =
{
    {"a", SDL_SCANCODE_A}, {"b", SDL_SCANCODE_B}, {"c", SDL_SCANCODE_C}, {"d", SDL_SCANCODE_D},
    {"e", SDL_SCANCODE_E}, {"f", SDL_SCANCODE_F}, {"g", SDL_SCANCODE_G}, {"h", SDL_SCANCODE_H},
    {"i", SDL_SCANCODE_I}, {"j", SDL_SCANCODE_J}, {"k", SDL_SCANCODE_K}, {"l", SDL_SCANCODE_L},
    {"m", SDL_SCANCODE_M}, {"n", SDL_SCANCODE_N}, {"o", SDL_SCANCODE_O}, {"p", SDL_SCANCODE_P},
    {"q", SDL_SCANCODE_Q}, {"r", SDL_SCANCODE_R}, {"s", SDL_SCANCODE_S}, {"t", SDL_SCANCODE_T},
    {"u", SDL_SCANCODE_U}, {"v", SDL_SCANCODE_V}, {"w", SDL_SCANCODE_W}, {"x", SDL_SCANCODE_X},
    {"y", SDL_SCANCODE_Y}, {"z", SDL_SCANCODE_Z},

    {"1", SDL_SCANCODE_1}, {"2", SDL_SCANCODE_2}, {"3", SDL_SCANCODE_3}, {"4", SDL_SCANCODE_4},
    {"5", SDL_SCANCODE_5}, {"6", SDL_SCANCODE_6}, {"7", SDL_SCANCODE_7}, {"8", SDL_SCANCODE_8},
    {"9", SDL_SCANCODE_9}, {"0", SDL_SCANCODE_0},

    {"return", SDL_SCANCODE_RETURN}, {"escape", SDL_SCANCODE_ESCAPE},
    {"backspace", SDL_SCANCODE_BACKSPACE}, {"tab", SDL_SCANCODE_TAB}, {"space", SDL_SCANCODE_SPACE},
    {"minus", SDL_SCANCODE_MINUS}, {"equals", SDL_SCANCODE_EQUALS},
    {"leftbracket", SDL_SCANCODE_LEFTBRACKET}, {"rightbracket", SDL_SCANCODE_RIGHTBRACKET},
    {"backslash", SDL_SCANCODE_BACKSLASH}, {"nonushash", SDL_SCANCODE_NONUSHASH},
    {"semicolon", SDL_SCANCODE_SEMICOLON}, {"apostrophe", SDL_SCANCODE_APOSTROPHE},
    {"grave", SDL_SCANCODE_GRAVE}, {"comma", SDL_SCANCODE_COMMA}, {"period", SDL_SCANCODE_PERIOD},
    {"slash", SDL_SCANCODE_SLASH}, {"capslock", SDL_SCANCODE_CAPSLOCK},

    {"f1", SDL_SCANCODE_F1}, {"f2", SDL_SCANCODE_F2}, {"f3", SDL_SCANCODE_F3}, {"f4", SDL_SCANCODE_F4},
    {"f5", SDL_SCANCODE_F5}, {"f6", SDL_SCANCODE_F6}, {"f7", SDL_SCANCODE_F7}, {"f8", SDL_SCANCODE_F8},
    {"f9", SDL_SCANCODE_F9}, {"f10", SDL_SCANCODE_F10}, {"f11", SDL_SCANCODE_F11}, {"f12", SDL_SCANCODE_F12},
    {"f13", SDL_SCANCODE_F13}, {"f14", SDL_SCANCODE_F14}, {"f15", SDL_SCANCODE_F15}, {"f16", SDL_SCANCODE_F16},
    {"f17", SDL_SCANCODE_F17}, {"f18", SDL_SCANCODE_F18}, {"f19", SDL_SCANCODE_F19}, {"f20", SDL_SCANCODE_F20},
    {"f21", SDL_SCANCODE_F21}, {"f22", SDL_SCANCODE_F22}, {"f23", SDL_SCANCODE_F23}, {"f24", SDL_SCANCODE_F24},

    {"printscreen", SDL_SCANCODE_PRINTSCREEN}, {"scrolllock", SDL_SCANCODE_SCROLLLOCK},
    {"pause", SDL_SCANCODE_PAUSE}, {"insert", SDL_SCANCODE_INSERT}, {"home", SDL_SCANCODE_HOME},
    {"pageup", SDL_SCANCODE_PAGEUP}, {"delete", SDL_SCANCODE_DELETE}, {"end", SDL_SCANCODE_END},
    {"pagedown", SDL_SCANCODE_PAGEDOWN}, {"right", SDL_SCANCODE_RIGHT}, {"left", SDL_SCANCODE_LEFT},
    {"down", SDL_SCANCODE_DOWN}, {"up", SDL_SCANCODE_UP},

    {"numlockclear", SDL_SCANCODE_NUMLOCKCLEAR}, {"kp_divide", SDL_SCANCODE_KP_DIVIDE},
    {"kp_multiply", SDL_SCANCODE_KP_MULTIPLY}, {"kp_minus", SDL_SCANCODE_KP_MINUS},
    {"kp_plus", SDL_SCANCODE_KP_PLUS}, {"kp_enter", SDL_SCANCODE_KP_ENTER},
    {"kp_1", SDL_SCANCODE_KP_1}, {"kp_2", SDL_SCANCODE_KP_2}, {"kp_3", SDL_SCANCODE_KP_3},
    {"kp_4", SDL_SCANCODE_KP_4}, {"kp_5", SDL_SCANCODE_KP_5}, {"kp_6", SDL_SCANCODE_KP_6},
    {"kp_7", SDL_SCANCODE_KP_7}, {"kp_8", SDL_SCANCODE_KP_8}, {"kp_9", SDL_SCANCODE_KP_9},
    {"kp_0", SDL_SCANCODE_KP_0}, {"kp_period", SDL_SCANCODE_KP_PERIOD}, {"kp_equals", SDL_SCANCODE_KP_EQUALS},

    {"nonusbackslash", SDL_SCANCODE_NONUSBACKSLASH}, {"application", SDL_SCANCODE_APPLICATION},
    {"power", SDL_SCANCODE_POWER}, {"menu", SDL_SCANCODE_MENU},

    {"mute", SDL_SCANCODE_MUTE}, {"volumeup", SDL_SCANCODE_VOLUMEUP}, {"volumedown", SDL_SCANCODE_VOLUMEDOWN},
    {"audionext", SDL_SCANCODE_AUDIONEXT}, {"audioprev", SDL_SCANCODE_AUDIOPREV},
    {"audiostop", SDL_SCANCODE_AUDIOSTOP}, {"audioplay", SDL_SCANCODE_AUDIOPLAY},
    {"audiomute", SDL_SCANCODE_AUDIOMUTE},

    {"lctrl", SDL_SCANCODE_LCTRL}, {"lshift", SDL_SCANCODE_LSHIFT}, {"lalt", SDL_SCANCODE_LALT},
    {"lgui", SDL_SCANCODE_LGUI}, {"rctrl", SDL_SCANCODE_RCTRL}, {"rshift", SDL_SCANCODE_RSHIFT},
    {"ralt", SDL_SCANCODE_RALT}, {"rgui", SDL_SCANCODE_RGUI},
};

#define SCANCODE_NAME_COUNT (sizeof(kScancodeNames) / sizeof(kScancodeNames[0]))
static inline SDL_Scancode ScancodeFromKeyName(const char* name)
{
    for (size_t i = 0; i < SCANCODE_NAME_COUNT; i++)
    	if (strcmp(kScancodeNames[i].name, name) == 0)
            return kScancodeNames[i].code;
    return SDL_SCANCODE_UNKNOWN;
}

static inline SDL_Scancode ScancodeFromBinding(const char* binding)
{
    const char* key = strstr(binding, "key:");
    if (!key)
        return SDL_SCANCODE_UNKNOWN;
    return ScancodeFromKeyName(key + 4);
}

typedef struct
{
    const char* name;
    int button;
} MouseButtonName;
static const MouseButtonName kMouseButtonNames[] =
{
    {"left", SDL_BUTTON_LEFT}, {"right", SDL_BUTTON_RIGHT}, {"middle", SDL_BUTTON_MIDDLE},
    {"x1", SDL_BUTTON_X1}, {"x2", SDL_BUTTON_X2},
};

#define MOUSE_BUTTON_NAME_COUNT (sizeof(kMouseButtonNames) / sizeof(kMouseButtonNames[0]))
static inline int MouseButtonFromName(const char* name)
{
    for (size_t i = 0; i < MOUSE_BUTTON_NAME_COUNT; i++)
    	if (strcmp(kMouseButtonNames[i].name, name) == 0)
            return kMouseButtonNames[i].button;
    return 0;
}

typedef enum
{
    BINDING_KEY,
    BINDING_MOUSE,
    BINDING_CONTROLLER,
} BindingType;

typedef struct
{
    BindingType type;
    SDL_Scancode scancode;
    int mouse_button;
    char raw[40];
} Binding;

static inline int ParseBindingToken(const char* token, Binding* out)
{
    const char* key = strstr(token, "key:");
    if (key)
	{
        SDL_Scancode code = ScancodeFromKeyName(key + 4);
        if (code == SDL_SCANCODE_UNKNOWN)
            return 0;
        out->type = BINDING_KEY;
        out->scancode = code;
        out->mouse_button = 0;
        strncpy(out->raw, key + 4, sizeof(out->raw) - 1);
        out->raw[sizeof(out->raw) - 1] = '\0';
        return 1;
    }
    if (strncmp(token, "mouse:", 6) == 0)
	{
        out->type = BINDING_MOUSE;
        out->scancode = SDL_SCANCODE_UNKNOWN;
        out->mouse_button = MouseButtonFromName(token + 6);
        strncpy(out->raw, token + 6, sizeof(out->raw) - 1);
        out->raw[sizeof(out->raw) - 1] = '\0';
        return 1;
    }
    if (strncmp(token, "c:", 2) == 0)
	{
        out->type = BINDING_CONTROLLER;
        out->scancode = SDL_SCANCODE_UNKNOWN;
        out->mouse_button = 0;
        strncpy(out->raw, token + 2, sizeof(out->raw) - 1);
        out->raw[sizeof(out->raw) - 1] = '\0';
        return 1;
    }
    return 0;
}

#define LNCT_MAX_ACTIONS 256
#define LNCT_MAX_BINDINGS_PER_ACTION 2
#define LNCT_MAX_ACTION_NAME 64
typedef struct
{
    char name[LNCT_MAX_ACTION_NAME];
    Binding bindings[LNCT_MAX_BINDINGS_PER_ACTION];
    int binding_count;
} ActionBindings;
typedef struct
{
    ActionBindings actions[LNCT_MAX_ACTIONS];
    int action_count;
} BindingSet;

static inline const char* SkipWs(const char* p)
{
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')
        p++;
    return p;
}

static inline const char* ParseJsonString(const char* p, char* out, size_t out_size)
{
    if (*p != '"')
        return NULL;
    p++;

    size_t i = 0;
    while (*p != '"')
	{
        if (*p == '\0')
            return NULL;
        char c = *p++;
        if (c == '\\')
		{
            if (*p == '\0')
                return NULL;
            c = *p++;
        }
        if (i + 1 < out_size)
            out[i++] = c;
    }
    out[i] = '\0';
    return p + 1;
}

static inline int ParseBindingsJson(const char* json, BindingSet* out)
{
    out->action_count = 0;

    const char* p = SkipWs(json);
    if (*p != '{')
        return 0;
    p = SkipWs(p + 1);
    if (*p == '}')
        return 1;

    for (;;)
	{
        char action_name[LNCT_MAX_ACTION_NAME];
        p = ParseJsonString(SkipWs(p), action_name, sizeof(action_name));
        if (!p)
            return 0;

        p = SkipWs(p);
        if (*p != ':')
            return 0;
        p = SkipWs(p + 1);
        if (*p != '[')
            return 0;
        p = SkipWs(p + 1);

        Binding found[LNCT_MAX_BINDINGS_PER_ACTION];
        int found_count = 0;

        if (*p != ']')
		{
            for (;;)
			{
                char token[64];
                p = ParseJsonString(SkipWs(p), token, sizeof(token));
                if (!p)
                    return 0;

                Binding b;
                if (ParseBindingToken(token, &b) && found_count < LNCT_MAX_BINDINGS_PER_ACTION)
                    found[found_count++] = b;

                p = SkipWs(p);
                if (*p == ',')
				{
					p = SkipWs(p + 1);
					continue;
				}
                break;
            }
        }
        if (*p != ']')
            return 0;
        p = SkipWs(p + 1);

        if (out->action_count < LNCT_MAX_ACTIONS)
		{
            ActionBindings* ab = &out->actions[out->action_count++];
            strncpy(ab->name, action_name, sizeof(ab->name) - 1);
            ab->name[sizeof(ab->name) - 1] = '\0';
            ab->binding_count = found_count;
            memcpy(ab->bindings, found, sizeof(Binding) * (size_t)found_count);
        }

        if (*p == ',')
		{
			p = SkipWs(p + 1);
			continue;
		}
        break;
    }

    return *p == '}';
}

static inline int ExpandHomePath(const char* path, char* out, size_t out_size)
{
    if (path[0] == '~' && (path[1] == '/' || path[1] == '\0'))
	{
        const char* home = getenv("HOME");
        if (!home)
            return 0;
        int n = snprintf(out, out_size, "%s%s", home, path + 1);
        return n > 0 && (size_t)n < out_size;
    }
    if (strlen(path) >= out_size)
        return 0;
    strcpy(out, path);
    return 1;
}

static inline char* ReadBindsFile(const char* path)
{
	char expanded_path[1024];
    if (!ExpandHomePath(path, expanded_path, sizeof(expanded_path)))
        return NULL;
 
    FILE* f = fopen(expanded_path, "rb");
    if (!f)
        return NULL;

    if (fseek(f, 0, SEEK_END) != 0)
	{ 
		fclose(f); 
		return NULL; 
	}
    long size = ftell(f);
    if (size < 0) 
	{ 
		fclose(f); 
		return NULL; 
	}
    fseek(f, 0, SEEK_SET);

    char* buf = (char*)malloc((size_t)size + 1);
    if (!buf) 
	{ 
		fclose(f); 
		return NULL; 
	}

    size_t read = fread(buf, 1, (size_t)size, f);
    fclose(f);
    buf[read] = '\0';
    return buf;
}

static inline int LoadBindingsFromFile(const char* path, BindingSet* out)
{
    char* json = ReadBindsFile(path);
    if (!json)
        return 0;
    int ret = ParseBindingsJson(json, out);
    free(json);
    return ret;
}

static inline const ActionBindings* FindAction(const BindingSet* set, const char* action)
{
    for (int i = 0; i < set->action_count; i++) 
	{
        if (strcmp(set->actions[i].name, action) == 0)
            return &set->actions[i];
    }
    return NULL;
}

static inline const Binding* FindActionBindingOfType(const BindingSet* set, const char* action, BindingType type)
{
    const ActionBindings* ab = FindAction(set, action);
    if (!ab)
        return NULL;
    for (int i = 0; i < ab->binding_count; i++)
	{
        if (ab->bindings[i].type == type)
            return &ab->bindings[i];
    }
    return NULL;
}
