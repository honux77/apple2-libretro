// LibretroFrame.cpp - concrete FrameBase subclass for libretro port

#include "StdAfx.h"
#include "LibretroFrame.h"
#include "Interface.h"
#include "Video.h"

#include <stdio.h>
#include <string.h>

// resource IDs from resource/resource.h
// We map numeric IDs to ROM filenames in the system directory
static const struct { WORD id; const char* filename; } s_resourceMap[] =
{
    { 126, "Apple2.rom"              },   // IDR_APPLE2_ROM
    { 127, "Apple2_Plus.rom"         },   // IDR_APPLE2_PLUS_ROM
    { 128, "Apple2_Video.rom"        },   // IDR_APPLE2_VIDEO_ROM
    { 129, "Apple2e.rom"             },   // IDR_APPLE2E_ROM
    { 130, "Apple2e_Enhanced.rom"    },   // IDR_APPLE2E_ENHANCED_ROM
    { 131, "Apple2e_Enhanced_Video.rom" },// IDR_APPLE2E_ENHANCED_VIDEO_ROM
    { 133, "Parallel.rom"            },   // IDR_PRINTDRVR_FW
    { 134, "SSC.rom"                 },   // IDR_SSC_FW
    { 135, "Mockingboard-D.rom"      },   // IDR_MOCKINGBOARD_D_FW
    { 136, "MouseInterface.rom"      },   // IDR_MOUSEINTERFACE_FW
    { 137, "ThunderClockPlus.rom"    },   // IDR_THUNDERCLOCKPLUS_FW
    { 138, "BootSector/bootsector.bin" }, // IDR_BOOT_SECTOR (in firmware subdir)
    { 139, "Freezes_Non-autostart_F8_Rom.rom" }, // IDR_FREEZES_F8_ROM
    { 140, "PRAVETS82.ROM"           },   // IDR_PRAVETS_82_ROM
    { 141, "PRAVETS8M.ROM"           },   // IDR_PRAVETS_8M_ROM
    { 142, "PRAVETS8C.ROM"           },   // IDR_PRAVETS_8C_ROM
    { 147, "TK3000e.rom"             },   // IDR_TK3000_2E_ROM
    { 148, "TKClock.rom"             },   // IDR_TKCLOCK_FW
    { 149, "DISK2-13sector.rom"      },   // IDR_DISK2_13SECTOR_FW
    { 150, "DISK2.rom"               },   // IDR_DISK2_16SECTOR_FW
    { 151, "Apple2_JPlus.rom"        },   // IDR_APPLE2_JPLUS_ROM
    { 152, "Apple2_JPlus_Video.rom"  },   // IDR_APPLE2_JPLUS_VIDEO_ROM
    { 153, "Base64A.rom"             },   // IDR_BASE_64A_ROM
    { 154, "Base64A_German_Video.rom" },  // IDR_BASE64A_VIDEO_ROM
    { 155, "Hddrvr-v2.bin"           },   // IDR_HDDRVR_V2_FW
    { 156, "HDC-SmartPort.bin"       },   // IDR_HDC_SMARTPORT_FW
    { 119, "Hddrvr.bin"              },   // IDR_HDDRVR_FW
    { 161, "dos33c.bin"              },   // IDR_OS_DOS33
    { 162, "prodos243.bin"           },   // IDR_OS_PRODOS243
    { 163, "bootsector_prodos243.bin" },  // IDR_BOOT_SECTOR_PRODOS243
    { 164, "bitsy.boot.bin"          },   // IDR_FILE_BITSY_BOOT
    { 165, "quit.system.bin"         },   // IDR_FILE_BITSY_BYE
    { 166, "basic17.system.bin"      },   // IDR_FILE_BASIC17
    { 0,   nullptr                   },
};

// -----------------------------------------------------------------------
// LibretroSoundBuffer implementation
// -----------------------------------------------------------------------

LibretroSoundBuffer::LibretroSoundBuffer(uint32_t dwBufferSize,
                                         uint32_t /*nSampleRate*/,
                                         int /*nChannels*/)
    : m_buffer(dwBufferSize, 0)
    , m_writePos(0)
    , m_playPos(0)
    , m_volume(DSBVOLUME_MAX)
    , m_status(0)
{
}

HRESULT LibretroSoundBuffer::SetCurrentPosition(DWORD dwNewPosition)
{
    m_playPos = dwNewPosition % (DWORD)m_buffer.size();
    return S_OK;
}

HRESULT LibretroSoundBuffer::GetCurrentPosition(LPDWORD lpdwCurrentPlayCursor,
                                                  LPDWORD lpdwCurrentWriteCursor)
{
    if (lpdwCurrentPlayCursor)  *lpdwCurrentPlayCursor  = m_playPos;
    if (lpdwCurrentWriteCursor) *lpdwCurrentWriteCursor = m_writePos;
    return S_OK;
}

HRESULT LibretroSoundBuffer::Lock(DWORD dwWriteCursor, DWORD dwWriteBytes,
                                   LPVOID* lplpvAudioPtr1, DWORD* lpdwAudioBytes1,
                                   LPVOID* lplpvAudioPtr2, DWORD* lpdwAudioBytes2,
                                   DWORD /*dwFlags*/)
{
    DWORD bufSize = (DWORD)m_buffer.size();
    if (bufSize == 0) return E_FAIL;

    dwWriteCursor = dwWriteCursor % bufSize;
    DWORD available = bufSize - dwWriteCursor;

    if (lplpvAudioPtr1)  *lplpvAudioPtr1  = m_buffer.data() + dwWriteCursor;
    if (lpdwAudioBytes1) *lpdwAudioBytes1 = (dwWriteBytes <= available) ? dwWriteBytes : available;
    if (lplpvAudioPtr2)  *lplpvAudioPtr2  = nullptr;
    if (lpdwAudioBytes2) *lpdwAudioBytes2 = 0;

    if (dwWriteBytes > available)
    {
        if (lplpvAudioPtr2)  *lplpvAudioPtr2  = m_buffer.data();
        if (lpdwAudioBytes2) *lpdwAudioBytes2 = dwWriteBytes - available;
    }

    m_writePos = (dwWriteCursor + dwWriteBytes) % bufSize;
    return S_OK;
}

HRESULT LibretroSoundBuffer::Unlock(LPVOID /*lpvAudioPtr1*/, DWORD /*dwAudioBytes1*/,
                                     LPVOID /*lpvAudioPtr2*/, DWORD /*dwAudioBytes2*/)
{
    return S_OK;
}

HRESULT LibretroSoundBuffer::Stop()
{
    m_status &= ~DSBSTATUS_PLAYING;
    return S_OK;
}

HRESULT LibretroSoundBuffer::Play(DWORD /*dwReserved1*/, DWORD /*dwReserved2*/, DWORD dwFlags)
{
    m_status |= DSBSTATUS_PLAYING;
    if (dwFlags & DSBPLAY_LOOPING)
        m_status |= DSBSTATUS_LOOPING;
    return S_OK;
}

HRESULT LibretroSoundBuffer::SetVolume(LONG lVolume)
{
    m_volume = lVolume;
    return S_OK;
}

HRESULT LibretroSoundBuffer::GetVolume(LONG* lplVolume)
{
    if (lplVolume) *lplVolume = m_volume;
    return S_OK;
}

HRESULT LibretroSoundBuffer::GetStatus(LPDWORD lpdwStatus)
{
    if (lpdwStatus) *lpdwStatus = m_status;
    return S_OK;
}

HRESULT LibretroSoundBuffer::Restore()
{
    m_status &= ~DSBSTATUS_BUFFERLOST;
    return S_OK;
}

// -----------------------------------------------------------------------
// DSAvailable() - libretro does its own audio; we provide buffers but
// the emulator should not gate on DirectSound availability.
// -----------------------------------------------------------------------
bool DSAvailable()
{
    return true; // return true so sound emulation runs
}

// -----------------------------------------------------------------------
// LibretroFrame implementation
// -----------------------------------------------------------------------

LibretroFrame::LibretroFrame()
    : m_systemDir(".")
{
}

LibretroFrame::~LibretroFrame()
{
}

void LibretroFrame::Initialize(bool resetVideoState)
{
    // Allocate framebuffer (BGRA, 560x384)
    Video& video = GetVideo();
    UINT w = video.GetFrameBufferWidth();
    UINT h = video.GetFrameBufferHeight();
    if (w == 0 || h == 0)
    {
        // Defaults before Video::Initialize() is called
        w = 560 + 2 * 4; // border
        h = 384 + 2 * 4;
    }
    m_framebuffer.assign((size_t)w * h * 4, 0);
    video.Initialize(m_framebuffer.data(), resetVideoState);
}

void LibretroFrame::Destroy(void)
{
    GetVideo().Destroy();
    m_framebuffer.clear();
}

void LibretroFrame::FrameDrawDiskLEDS()      { /* no-op */ }
void LibretroFrame::FrameDrawDiskStatus()    { /* no-op */ }
void LibretroFrame::FrameRefreshStatus(int /*drawflags*/) { /* no-op */ }
void LibretroFrame::FrameUpdateApple2Type()  { /* no-op */ }
void LibretroFrame::FrameSetCursorPosByMousePos() { /* no-op */ }
void LibretroFrame::SetFullScreenShowSubunitStatus(bool /*bShow*/) { /* no-op */ }
void LibretroFrame::SetWindowedModeShowDiskiiStatus(bool /*bShow*/) { /* no-op */ }

bool LibretroFrame::GetBestDisplayResolutionForFullScreen(UINT& /*bestWidth*/,
                                                           UINT& /*bestHeight*/,
                                                           UINT /*userSpecifiedWidth*/,
                                                           UINT /*userSpecifiedHeight*/)
{
    return false;
}

int LibretroFrame::SetViewportScale(int /*nNewScale*/, bool /*bForce*/)
{
    return 1;
}

void LibretroFrame::SetAltEnterToggleFullScreen(bool /*mode*/) { /* no-op */ }
void LibretroFrame::SetLoadedSaveStateFlag(const bool /*bFlag*/) { /* no-op */ }

void LibretroFrame::VideoPresentScreen(void)
{
    // Called by the emulator each time it finishes drawing a frame.
    // The framebuffer data is in m_framebuffer (BGRA).
    // The libretro video_cb will be called from libretro_bridge.cpp.
    // Nothing to do here; the bridge accesses the Video framebuffer directly.
}

void LibretroFrame::ResizeWindow(void) { /* no-op */ }

int LibretroFrame::FrameMessageBox(LPCSTR lpText, LPCSTR lpCaption, UINT /*uType*/)
{
    fprintf(stderr, "[AppleWin] %s: %s\n", lpCaption ? lpCaption : "", lpText ? lpText : "");
    return IDOK;
}

void LibretroFrame::GetBitmap(WORD /*id*/, LONG /*cb*/, LPVOID /*lpvBits*/)
{
    // No-op - bitmaps are for UI elements we don't render
}

std::shared_ptr<NetworkBackend> LibretroFrame::CreateNetworkBackend(const std::string& /*interfaceName*/)
{
    return nullptr;
}

std::shared_ptr<SoundBuffer> LibretroFrame::CreateSoundBuffer(uint32_t dwBufferSize,
                                                               uint32_t nSampleRate,
                                                               int nChannels,
                                                               const char* /*pszVoiceName*/)
{
    return std::make_shared<LibretroSoundBuffer>(dwBufferSize, nSampleRate, nChannels);
}

const char* LibretroFrame::ResourceIdToFilename(WORD id)
{
    for (int i = 0; s_resourceMap[i].filename != nullptr; ++i)
    {
        if (s_resourceMap[i].id == id)
            return s_resourceMap[i].filename;
    }
    return nullptr;
}

BYTE* LibretroFrame::GetResource(WORD id, LPCSTR /*lpType*/, uint32_t expectedSize)
{
    const char* filename = ResourceIdToFilename(id);
    if (!filename)
    {
        fprintf(stderr, "[AppleWin] GetResource: unknown resource ID %u\n", (unsigned)id);
        return nullptr;
    }

    // Build full path: system_dir/apple2/filename
    std::string path = m_systemDir + "/apple2/" + filename;

    FILE* f = fopen(path.c_str(), "rb");
    if (!f)
    {
        // Try system_dir directly
        path = m_systemDir + "/" + filename;
        f = fopen(path.c_str(), "rb");
    }
    if (!f)
    {
        fprintf(stderr, "[AppleWin] GetResource: cannot open '%s' (id=%u)\n", filename, (unsigned)id);
        return nullptr;
    }

    fseek(f, 0, SEEK_END);
    long fileSize = ftell(f);
    fseek(f, 0, SEEK_SET);

    if (expectedSize != 0 && (uint32_t)fileSize != expectedSize)
    {
        fprintf(stderr, "[AppleWin] GetResource: '%s' size mismatch: got %ld, expected %u\n",
                filename, fileSize, expectedSize);
        fclose(f);
        return nullptr;
    }

    m_resourceCache.resize((size_t)fileSize);
    size_t bytesRead = fread(m_resourceCache.data(), 1, (size_t)fileSize, f);
    fclose(f);

    if (bytesRead != (size_t)fileSize)
    {
        fprintf(stderr, "[AppleWin] GetResource: short read for '%s'\n", filename);
        return nullptr;
    }

    return m_resourceCache.data();
}

void LibretroFrame::Restart()
{
    // No-op for libretro - restart is handled by the frontend
}

std::string LibretroFrame::Video_GetScreenShotFolder() const
{
    return "";
}
