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
#include "Keyboard.h"

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

    // Remove Disk2 card from slot 6 before MemInitialize:
    // Apple IIe with disk controller + no disk = infinite boot loop.
    // Without disk controller, ROM boots straight to Applesoft BASIC.
    // LoadDisk() will re-insert the card and re-init memory if needed.
    GetCardMgr().Remove(SLOT6);

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

    // Insert Disk2 card if not present, then re-init memory so ROM at $C600 is mapped
    if (GetCardMgr().QuerySlot(SLOT6) != CT_Disk2)
    {
        GetCardMgr().Insert(SLOT6, CT_Disk2);
        MemInitialize();
        CpuReset();
        fprintf(stderr, "[apple2] LoadDisk: inserted Disk2 card and re-initialized memory\n");
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

    // DEBUG: log cycles and PC on first few frames
    static int s_dbgFrame = 0;
    if (++s_dbgFrame <= 3) {
        FILE* f = fopen("/tmp/apple2_scr.log", "a");
        if (f) {
            LPBYTE pc_lo = MemGetMainPtr(0xFFFC);
            fprintf(f, "dbg frame=%d nCycles=%d executed=%u PC_reset_vec=%02X%02X mem[0400]=%02X\n",
                s_dbgFrame, nCyclesToExecute, dwExecutedCycles,
                pc_lo[1], pc_lo[0],
                MemGetMainPtr(0x0400)[0]);
            fclose(f);
        }
    }

    g_dwCyclesThisFrame += dwExecutedCycles;
    if (g_dwCyclesThisFrame >= nCyclesPerFrame)
        g_dwCyclesThisFrame -= nCyclesPerFrame;

    // Feedback for next frame (keeps average cycle count accurate)
    g_nCpuCyclesFeedback = (int)nCyclesPerFrame - (int)dwExecutedCycles;

    // Update peripherals (disk motor, speaker, Mockingboard)
    GetCardMgr().GetDisk2CardMgr().Update(dwExecutedCycles);
    SpkrUpdate(dwExecutedCycles);
    GetCardMgr().GetMockingboardCardMgr().Update(dwExecutedCycles);

    // Redraw the whole screen from current Apple II memory state.
    // The real-time NTSC path (bVideoUpdate=true) may miss updates;
    // this guarantees a correct frame.
    GetFrame().VideoRefreshScreen(GetVideo().GetVideoMode(), true);

    // DEBUG: dump screen memory to /tmp every 60 frames
    static int s_frameCount = 0;
    ++s_frameCount;
    if (s_frameCount == 1 || s_frameCount % 60 == 0) {
        FILE* f = fopen("/tmp/apple2_scr.log", "a");
        if (f) {
            // Row addresses in Apple II text screen (non-linear layout)
            const uint16_t rowAddr[] = {
                0x0400, 0x0480, 0x0500, 0x0580, 0x0600, 0x0680, 0x0700, 0x0780,
                0x0428, 0x04A8, 0x0528, 0x05A8, 0x0628, 0x06A8, 0x0728, 0x07A8,
                0x0450, 0x04D0, 0x0550, 0x05D0, 0x0650, 0x06D0, 0x0750, 0x07D0
            };
            // Cursor position
            LPBYTE cv = MemGetMainPtr(0x24);
            fprintf(f, "frame=%d cursor=col%d,row%d ", s_frameCount, cv[0], cv[1]);
            // Scan all 24 rows for non-space bytes
            bool found = false;
            for (int r = 0; r < 24; r++) {
                LPBYTE row = MemGetMainPtr(rowAddr[r]);
                for (int c = 0; c < 8; c++) {
                    if (row[c] != 0xA0 && row[c] != 0x00) {
                        fprintf(f, "row%d[$%04X]:", r, rowAddr[r]);
                        for (int i = 0; i < 8; i++) fprintf(f, " %02X", row[i]);
                        fprintf(f, " ");
                        found = true;
                        break;
                    }
                }
            }
            if (!found) fprintf(f, "(all spaces)");
            fprintf(f, "\n");
            fclose(f);
        }
    }
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
    const UINT totalH  = video.GetFrameBufferHeight();       // 420
    const UINT borderW = video.GetFrameBufferBorderWidth();  // 20
    const UINT borderH = video.GetFrameBufferBorderHeight(); // 18

    // AppleWin stores rows bottom-up (Windows DIB style):
    // scanline 0 (top of image) is at buffer row (totalH - 1 - borderH) = 401
    // scanline 383 (bottom of image) is at buffer row borderH = 18
    const uint32_t* src = reinterpret_cast<const uint32_t*>(srcBytes)
                          + (totalH - 1 - borderH) * totalW + borderW;

    for (unsigned y = 0; y < SCREEN_H; ++y)
    {
        memcpy(g_outputFb + y * SCREEN_W, src, SCREEN_W * sizeof(uint32_t));
        src -= totalW;  // go backwards through bottom-up buffer
    }

    return g_outputFb;
}

unsigned GetWidth()  { return SCREEN_W; }
unsigned GetHeight() { return SCREEN_H; }

// Arrow key codes for Apple IIe Enhanced
// Left=8(ctrl-H), Right=21(ctrl-U), Up=11(ctrl-K), Down=10(ctrl-J)
void KeyPress(uint32_t character)
{
    if (!g_initialized || character == 0 || character > 127)
        return;
    KeybQueueKeypress((WPARAM)character, ASCII);
}

void ArrowKey(int direction) // 0=left,1=right,2=up,3=down
{
    if (!g_initialized) return;
    static const uint8_t codes[4] = { 0x08, 0x15, 0x0B, 0x0A };
    if (direction >= 0 && direction < 4)
        KeybQueueKeypress(codes[direction], ASCII);
}

} // namespace Apple2Core
