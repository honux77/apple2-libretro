// Apple2Core namespace stubs for Phase 1
// Will be filled in during Phase 2+

#include <stdint.h>
#include <string.h>
#include <stdio.h>

// Debugger stub - extbench is declared in Debugger/Debug.h but defined here
uint32_t extbench = 0;

// Screen dimensions: 560 x 384 (double hi-res)
static const unsigned SCREEN_W = 560;
static const unsigned SCREEN_H = 384;

// Black framebuffer
static uint32_t g_framebuffer[SCREEN_W * SCREEN_H];

namespace Apple2Core {

bool Init(const char* system_dir)
{
    (void)system_dir;
    memset(g_framebuffer, 0, sizeof(g_framebuffer));
    return true;
}

void Destroy() {}

bool LoadDisk(const char* path)
{
    (void)path;
    return true;
}

void RunFrame()
{
    // Phase 2: run CPU for one video frame
}

const uint32_t* GetFramebuffer()
{
    return g_framebuffer;
}

unsigned GetWidth()  { return SCREEN_W; }
unsigned GetHeight() { return SCREEN_H; }

} // namespace Apple2Core
