// Apple II libretro core
// libretro adapter layer

#include "libretro-common/include/libretro.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>

// libretro callbacks
static retro_video_refresh_t video_cb = nullptr;
static retro_audio_sample_t audio_cb = nullptr;
static retro_audio_sample_batch_t audio_batch_cb = nullptr;
static retro_environment_t environ_cb = nullptr;
static retro_input_poll_t input_poll_cb = nullptr;
static retro_input_state_t input_state_cb = nullptr;
static retro_log_printf_t log_cb = nullptr;

// Forward declarations (Phase 2+)
namespace Apple2Core {
    bool Init(const char* system_dir);
    void Destroy();
    bool LoadDisk(const char* path);
    void RunFrame();
    const uint32_t* GetFramebuffer();
    unsigned GetWidth();
    unsigned GetHeight();
    void KeyPress(uint32_t character);
    void ArrowKey(int direction); // 0=left,1=right,2=up,3=down
}

// -------------------------------------------------------------------------
// libretro API
// -------------------------------------------------------------------------

static void keyboard_cb(bool down, unsigned keycode, uint32_t character, uint16_t /*key_modifiers*/)
{
    if (!down) return;

    // Arrow keys mapped to Apple IIe control codes
    switch (keycode) {
    case 276: Apple2Core::ArrowKey(0); return; // RETROK_LEFT  → ctrl-H (0x08)
    case 275: Apple2Core::ArrowKey(1); return; // RETROK_RIGHT → ctrl-U (0x15)
    case 273: Apple2Core::ArrowKey(2); return; // RETROK_UP    → ctrl-K (0x0B)
    case 274: Apple2Core::ArrowKey(3); return; // RETROK_DOWN  → ctrl-J (0x0A)
    default: break;
    }

    // Printable and control characters (1–127)
    if (character >= 1 && character <= 127)
        Apple2Core::KeyPress(character);
}

RETRO_API void retro_set_environment(retro_environment_t cb)
{
    environ_cb = cb;

    // Can run without a disk (boots to Applesoft BASIC prompt)
    bool no_game = true;
    cb(RETRO_ENVIRONMENT_SET_SUPPORT_NO_GAME, &no_game);

    // Register keyboard event callback
    struct retro_keyboard_callback kb = { keyboard_cb };
    cb(RETRO_ENVIRONMENT_SET_KEYBOARD_CALLBACK, &kb);

    // Get log callback
    struct retro_log_callback logging;
    if (cb(RETRO_ENVIRONMENT_GET_LOG_INTERFACE, &logging))
        log_cb = logging.log;
}

RETRO_API void retro_set_video_refresh(retro_video_refresh_t cb) { video_cb = cb; }
RETRO_API void retro_set_audio_sample(retro_audio_sample_t cb) { audio_cb = cb; }
RETRO_API void retro_set_audio_sample_batch(retro_audio_sample_batch_t cb) { audio_batch_cb = cb; }
RETRO_API void retro_set_input_poll(retro_input_poll_t cb) { input_poll_cb = cb; }
RETRO_API void retro_set_input_state(retro_input_state_t cb) { input_state_cb = cb; }

RETRO_API void retro_init(void)
{
    // Set pixel format
    retro_pixel_format fmt = RETRO_PIXEL_FORMAT_XRGB8888;
    if (environ_cb)
        environ_cb(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &fmt);

    const char* system_dir = nullptr;
    if (environ_cb)
        environ_cb(RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY, &system_dir);

    Apple2Core::Init(system_dir ? system_dir : ".");
}

RETRO_API void retro_deinit(void)
{
    Apple2Core::Destroy();
}

RETRO_API unsigned retro_api_version(void)
{
    return RETRO_API_VERSION;
}

RETRO_API void retro_get_system_info(struct retro_system_info* info)
{
    memset(info, 0, sizeof(*info));
    info->library_name     = "Apple II";
    info->library_version  = "1.0";
    info->valid_extensions = "dsk|do|po|nib|woz|2mg|img|gz|hdv";
    info->need_fullpath    = true;
    info->block_extract    = false;
}

RETRO_API void retro_get_system_av_info(struct retro_system_av_info* info)
{
    memset(info, 0, sizeof(*info));
    info->geometry.base_width   = Apple2Core::GetWidth();
    info->geometry.base_height  = Apple2Core::GetHeight();
    info->geometry.max_width    = Apple2Core::GetWidth();
    info->geometry.max_height   = Apple2Core::GetHeight();
    info->geometry.aspect_ratio = 4.0f / 3.0f;  // Apple II NTSC display is 4:3
    info->timing.fps            = 60.0;
    info->timing.sample_rate    = 44100.0;
}

RETRO_API void retro_set_controller_port_device(unsigned port, unsigned device)
{
    (void)port;
    (void)device;
}

RETRO_API void retro_reset(void)
{
    // TODO Phase 2: Reset CPU / memory
}

RETRO_API void retro_run(void)
{
    if (input_poll_cb)
        input_poll_cb();

    // Process input (Phase 6)
    // extern void libretro_input_update(retro_input_state_t cb);
    // libretro_input_update(input_state_cb);

    // Run one video frame worth of emulation (Phase 2)
    Apple2Core::RunFrame();

    // Send video frame
    if (video_cb)
    {
        const uint32_t* fb = Apple2Core::GetFramebuffer();
        unsigned w = Apple2Core::GetWidth();
        unsigned h = Apple2Core::GetHeight();
        video_cb(fb, w, h, w * sizeof(uint32_t));
    }

    // Send audio (Phase 5)
    // extern size_t libretro_audio_flush(retro_audio_sample_batch_t cb);
    // libretro_audio_flush(audio_batch_cb);
}

RETRO_API size_t retro_serialize_size(void)
{
    return 0; // TODO Phase 2: save state
}

RETRO_API bool retro_serialize(void* data, size_t size)
{
    (void)data; (void)size;
    return false;
}

RETRO_API bool retro_unserialize(const void* data, size_t size)
{
    (void)data; (void)size;
    return false;
}

RETRO_API void retro_cheat_reset(void) {}
RETRO_API void retro_cheat_set(unsigned index, bool enabled, const char* code)
{
    (void)index; (void)enabled; (void)code;
}

RETRO_API bool retro_load_game(const struct retro_game_info* game)
{
    if (game && game->path)
        return Apple2Core::LoadDisk(game->path);
    return true; // no game = boot to BASIC prompt
}

RETRO_API bool retro_load_game_special(unsigned game_type,
    const struct retro_game_info* info, size_t num_info)
{
    (void)game_type; (void)info; (void)num_info;
    return false;
}

RETRO_API void retro_unload_game(void) {}

RETRO_API unsigned retro_get_region(void)
{
    return RETRO_REGION_NTSC;
}

RETRO_API void* retro_get_memory_data(unsigned id)
{
    (void)id;
    return nullptr;
}

RETRO_API size_t retro_get_memory_size(unsigned id)
{
    (void)id;
    return 0;
}
