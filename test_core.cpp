// Standalone libretro core tester - no RetroArch needed
#include "libretro-common/include/libretro.h"
#include <dlfcn.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

// libretro function pointers
typedef void (*fn_retro_set_environment)(retro_environment_t);
typedef void (*fn_retro_set_video_refresh)(retro_video_refresh_t);
typedef void (*fn_retro_set_audio_sample)(retro_audio_sample_t);
typedef void (*fn_retro_set_audio_sample_batch)(retro_audio_sample_batch_t);
typedef void (*fn_retro_set_input_poll)(retro_input_poll_t);
typedef void (*fn_retro_set_input_state)(retro_input_state_t);
typedef void (*fn_retro_init)(void);
typedef bool (*fn_retro_load_game)(const struct retro_game_info*);
typedef void (*fn_retro_run)(void);
typedef void (*fn_retro_deinit)(void);

static bool env_cb(unsigned cmd, void* data)
{
    switch (cmd) {
    case RETRO_ENVIRONMENT_SET_SUPPORT_NO_GAME:
        return true;
    case RETRO_ENVIRONMENT_SET_PIXEL_FORMAT:
        return true;
    case RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY: {
        static const char* sysdir = "/Users/honux/games/bios";
        *(const char**)data = sysdir;
        return true;
    }
    case RETRO_ENVIRONMENT_GET_LOG_INTERFACE:
        return false;  // skip log interface
    default:
        return false;
    }
}

static void video_cb(const void* data, unsigned w, unsigned h, size_t pitch) {}
static void audio_sample_cb(int16_t l, int16_t r) {}
static size_t audio_batch_cb(const int16_t* data, size_t frames) { return frames; }
static void input_poll_cb(void) {}
static int16_t input_state_cb(unsigned port, unsigned device, unsigned index, unsigned id) { return 0; }

int main()
{
    const char* core_path = "/Users/honux/Library/Application Support/RetroArch/cores/apple2_libretro.dylib";
    void* handle = dlopen(core_path, RTLD_LAZY);
    if (!handle) { fprintf(stderr, "dlopen failed: %s\n", dlerror()); return 1; }

#define LOAD(name) auto name = (fn_##name)dlsym(handle, #name); if (!name) { fprintf(stderr, "missing: " #name "\n"); return 1; }
    LOAD(retro_set_environment)
    LOAD(retro_set_video_refresh)
    LOAD(retro_set_audio_sample)
    LOAD(retro_set_audio_sample_batch)
    LOAD(retro_set_input_poll)
    LOAD(retro_set_input_state)
    LOAD(retro_init)
    LOAD(retro_load_game)
    LOAD(retro_run)
    LOAD(retro_deinit)
#undef LOAD

    retro_set_environment(env_cb);
    retro_set_video_refresh(video_cb);
    retro_set_audio_sample(audio_sample_cb);
    retro_set_audio_sample_batch(audio_batch_cb);
    retro_set_input_poll(input_poll_cb);
    retro_set_input_state(input_state_cb);

    fprintf(stderr, "=== retro_init ===\n");
    retro_init();

    fprintf(stderr, "=== retro_load_game(NULL) ===\n");
    retro_load_game(nullptr);

    fprintf(stderr, "=== running 300 frames ===\n");
    for (int i = 0; i < 300; i++)
        retro_run();

    fprintf(stderr, "=== done, checking log ===\n");
    retro_deinit();
    dlclose(handle);

    FILE* f = fopen("/tmp/apple2_scr.log", "r");
    if (!f) { fprintf(stderr, "NO LOG FILE\n"); return 1; }
    char line[512];
    while (fgets(line, sizeof(line), f))
        printf("%s", line);
    fclose(f);
    return 0;
}
