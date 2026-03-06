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

HRESULT LibretroSoundBuffer::Unlock(LPVOID lpvAudioPtr1, DWORD dwAudioBytes1,
                                     LPVOID lpvAudioPtr2, DWORD dwAudioBytes2)
{
    // Cap drain buffer to ~1 second of stereo audio to prevent runaway growth.
    // This can happen when the 6522 timer period is very short (uninitialized game),
    // causing thousands of UpdateSoundBuffer calls per frame.
    static const size_t kMaxDrainShorts = 44100 * 2;

    if (lpvAudioPtr1 && dwAudioBytes1 > 0) {
        const short* p = static_cast<const short*>(lpvAudioPtr1);
        size_t n = dwAudioBytes1 / sizeof(short);
        if (m_drainBuffer.size() + n <= kMaxDrainShorts)
            m_drainBuffer.insert(m_drainBuffer.end(), p, p + n);
    }
    if (lpvAudioPtr2 && dwAudioBytes2 > 0) {
        const short* p = static_cast<const short*>(lpvAudioPtr2);
        size_t n = dwAudioBytes2 / sizeof(short);
        if (m_drainBuffer.size() + n <= kMaxDrainShorts)
            m_drainBuffer.insert(m_drainBuffer.end(), p, p + n);
    }
    // Advance play cursor to match write cursor so callers see the buffer as
    // always ready to accept more data (simulates hardware consuming audio instantly).
    m_playPos = m_writePos;
    return S_OK;
}

std::vector<short> LibretroSoundBuffer::DrainAudio()
{
    std::vector<short> out;
    out.swap(m_drainBuffer);
    // Hard safety cap: never return more than 1 second of stereo audio
    static const size_t kMaxShorts = 44100 * 2;
    if (out.size() > kMaxShorts)
        out.resize(kMaxShorts);
    return out;
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
        // Full NTSC framebuffer: 560 + 2*20 border = 600, 384 + 2*18 border = 420
        w = 600;
        h = 420;
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
    auto buf = std::make_shared<LibretroSoundBuffer>(dwBufferSize, nSampleRate, nChannels);
    m_soundBuffers.push_back(buf);
    return buf;
}

void LibretroFrame::DrainAllAudio(retro_audio_sample_batch_t cb)
{
    if (!cb) return;

    // Mix all sound buffers (speaker + Mockingboard + ...) into one output batch.
    // Submitting them separately would send 2x the expected samples per frame,
    // causing RetroArch audio throttling and half-speed emulation.
    m_mixBuf.clear();

    auto it = m_soundBuffers.begin();
    while (it != m_soundBuffers.end()) {
        auto sp = it->lock();
        if (!sp) {
            it = m_soundBuffers.erase(it);
            continue;
        }

        std::vector<short> chunk = sp->DrainAudio();
        DWORD nShorts = (DWORD)chunk.size();

        // Clamp to 1 frame of stereo audio (44100/60 * 2 ≈ 1470 shorts)
        static const DWORD kMaxFrameShorts = 44100 / 30 * 2; // generous 2-frame budget
        if (nShorts > kMaxFrameShorts)
            nShorts = kMaxFrameShorts;

        if (nShorts > 0) {
            if (m_mixBuf.size() < nShorts)
                m_mixBuf.resize(nShorts, 0);

            for (DWORD i = 0; i < nShorts; i++) {
                int32_t v = (int32_t)m_mixBuf[i] + (int32_t)chunk[i];
                if (v >  32767) v =  32767;
                if (v < -32768) v = -32768;
                m_mixBuf[i] = (short)v;
            }
        }

        ++it;
    }

    if (m_mixBuf.size() >= 2)
        cb(m_mixBuf.data(), m_mixBuf.size() / 2);
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
