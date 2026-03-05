// libretro_bridge.cpp - Apple2Core namespace: connects libretro to AppleWin emulator core
// Phase 2: real emulation using AppleWin core (linapple architecture as reference)

#include "StdAfx.h"
#include "libretro_bridge.h"
#include "LibretroVideo.h"
#include "LibretroFrame.h"

// AppleWin core
#include "Core.h"
#include "CPU.h"
#include "Memory.h"
#include "NTSC.h"
#include "Video.h"
#include "Speaker.h"
#include "Interface.h"
#include "CardManager.h"
#include "Disk.h"

#include <string.h>
#include <stdint.h>
#include <stdio.h>

// Debugger stub - extbench declared in Debugger/Debug.h
uint32_t extbench = 0;

// Output dimensions (borderless Apple II video, 2x zoom)
static const unsigned SCREEN_W = 560;
static const unsigned SCREEN_H = 384;

// XRGB8888 output buffer (border-stripped copy of the internal framebuffer)
static uint32_t g_outputFb[SCREEN_W * SCREEN_H];

static bool g_initialized = false;

namespace Apple2Core {

bool Init(const char* system_dir)
{
    if (g_initialized)
        return true;

    const char* sysdir = system_dir ? system_dir : ".";

    // Set up frame/video/property-sheet singletons
    LibretroInterface_Init(sysdir);

    // Machine: Apple IIe Enhanced (same default as linapple)
    SetApple2Type(A2TYPE_APPLE2EENHANCED);
    SetMainCpuDefault(g_Apple2Type);
    SetCurrentCLK6502();

    // Allocate framebuffer and initialise NTSC video engine
    GetFrame().Initialize(true);

    // Speaker (uses LibretroSoundBuffer, no real audio output yet)
    SpkrInitialize();

    // Memory: allocates 64K regions and loads ROM images from system_dir
    MemInitialize();

    // CPU
    CpuInitialize();

    // Enter running mode then reset the 6502
    g_nAppMode = MODE_RUNNING;
    CpuReset();

    memset(g_outputFb, 0, sizeof(g_outputFb));
    g_initialized = true;

    fprintf(stderr, "[apple2] Init OK, system_dir=%s\n", sysdir);
    return true;
}

void Destroy()
{
    if (!g_initialized)
        return;

    SpkrDestroy();
    CpuDestroy();
    MemDestroy();
    GetFrame().Destroy();

    g_initialized = false;
}

bool LoadDisk(const char* path)
{
    if (!path || !g_initialized)
        return false;

    if (GetCardMgr().QuerySlot(SLOT6) != CT_Disk2)
    {
        fprintf(stderr, "[apple2] LoadDisk: no Disk II card in slot 6\n");
        return false;
    }

    Disk2InterfaceCard& disk2 =
        dynamic_cast<Disk2InterfaceCard&>(GetCardMgr().GetRef(SLOT6));

    ImageError_e err = disk2.InsertDisk(
        DRIVE_1, path,
        IMAGE_USE_FILES_WRITE_PROTECT_STATUS,
        IMAGE_DONT_CREATE);

    if (err != eIMAGE_ERROR_NONE)
    {
        fprintf(stderr, "[apple2] LoadDisk: InsertDisk failed (err=%d) for '%s'\n",
                (int)err, path);
        return false;
    }

    fprintf(stderr, "[apple2] LoadDisk: inserted '%s'\n", path);
    return true;
}

// Run one video frame worth of 6502 cycles.
// Mirrors linapple ContinueExecution() but without SDL/timer coupling.
void RunFrame()
{
    if (!g_initialized)
        return;

    // 17030 cycles per NTSC frame (65 cycles * 262 lines)
    const uint32_t nCyclesPerFrame = NTSC_GetCyclesPerFrame();

    int nCyclesToExecute = (int)nCyclesPerFrame + g_nCpuCyclesFeedback;
    if (nCyclesToExecute < 0)
        nCyclesToExecute = 0;

    // Execute CPU cycles; bVideoUpdate=true lets the NTSC engine render
    // each scanline as the CPU runs (same as linapple's CpuExecute call)
    uint32_t dwExecutedCycles = CpuExecute((uint32_t)nCyclesToExecute, true);

    g_dwCyclesThisFrame += dwExecutedCycles;
    if (g_dwCyclesThisFrame >= nCyclesPerFrame)
        g_dwCyclesThisFrame -= nCyclesPerFrame;

    // Feedback for next frame (keeps average cycle count accurate)
    g_nCpuCyclesFeedback = (int)nCyclesPerFrame - (int)dwExecutedCycles;

    // Update peripherals (disk motor, speaker, Mockingboard)
    GetCardMgr().GetDisk2CardMgr().Update(dwExecutedCycles);
    SpkrUpdate(dwExecutedCycles);
    GetCardMgr().GetMockingboardCardMgr().Update(dwExecutedCycles);

    // Notify frame complete (no-op for libretro, but satisfies any internal state)
    GetFrame().VideoPresentScreen();
}

// Return a 560x384 XRGB8888 buffer, border-stripped from the internal framebuffer.
// The internal buffer is (560+40) x (384+36) = 600x420 with 20px left/right and
// 18px top/bottom borders that are never visible.
const uint32_t* GetFramebuffer()
{
    if (!g_initialized)
        return g_outputFb;

    Video& video = GetVideo();
    const uint8_t* srcBytes = video.GetFrameBuffer();
    if (!srcBytes)
        return g_outputFb;

    const UINT totalW  = video.GetFrameBufferWidth();        // 600
    const UINT borderW = video.GetFrameBufferBorderWidth();  // 20
    const UINT borderH = video.GetFrameBufferBorderHeight(); // 18

    // Each pixel is 4 bytes (BGRA = compatible with XRGB8888 on little-endian)
    const uint32_t* src = reinterpret_cast<const uint32_t*>(srcBytes)
                          + borderH * totalW + borderW;

    for (unsigned y = 0; y < SCREEN_H; ++y)
    {
        memcpy(g_outputFb + y * SCREEN_W, src, SCREEN_W * sizeof(uint32_t));
        src += totalW;
    }

    return g_outputFb;
}

unsigned GetWidth()  { return SCREEN_W; }
unsigned GetHeight() { return SCREEN_H; }

} // namespace Apple2Core
