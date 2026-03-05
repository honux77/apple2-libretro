#pragma once

#include "StdAfx.h"
#include "FrameBase.h"

#include <vector>
#include <string>

// LibretroSoundBuffer - stub SoundBuffer impl that discards audio
// (real audio mixing is done separately via libretro audio callbacks)
#include "SoundBuffer.h"

class LibretroSoundBuffer : public SoundBuffer
{
public:
    LibretroSoundBuffer(uint32_t dwBufferSize, uint32_t nSampleRate, int nChannels);
    virtual ~LibretroSoundBuffer() = default;

    virtual HRESULT SetCurrentPosition(DWORD dwNewPosition) override;
    virtual HRESULT GetCurrentPosition(LPDWORD lpdwCurrentPlayCursor, LPDWORD lpdwCurrentWriteCursor) override;
    virtual HRESULT Lock(DWORD dwWriteCursor, DWORD dwWriteBytes,
                         LPVOID* lplpvAudioPtr1, DWORD* lpdwAudioBytes1,
                         LPVOID* lplpvAudioPtr2, DWORD* lpdwAudioBytes2,
                         DWORD dwFlags) override;
    virtual HRESULT Unlock(LPVOID lpvAudioPtr1, DWORD dwAudioBytes1,
                           LPVOID lpvAudioPtr2, DWORD dwAudioBytes2) override;
    virtual HRESULT Stop() override;
    virtual HRESULT Play(DWORD dwReserved1, DWORD dwReserved2, DWORD dwFlags) override;
    virtual HRESULT SetVolume(LONG lVolume) override;
    virtual HRESULT GetVolume(LONG* lplVolume) override;
    virtual HRESULT GetStatus(LPDWORD lpdwStatus) override;
    virtual HRESULT Restore() override;

private:
    std::vector<uint8_t> m_buffer;
    DWORD m_writePos;
    DWORD m_playPos;
    LONG  m_volume;
    DWORD m_status;
};

// Forward declaration
class NetworkBackend;

class LibretroFrame : public FrameBase
{
public:
    LibretroFrame();
    virtual ~LibretroFrame();

    void SetSystemDir(const std::string& systemDir) { m_systemDir = systemDir; }

    // FrameBase pure virtual implementations
    virtual void Initialize(bool resetVideoState) override;
    virtual void Destroy(void) override;

    virtual void FrameDrawDiskLEDS() override;
    virtual void FrameDrawDiskStatus() override;
    virtual void FrameRefreshStatus(int drawflags) override;
    virtual void FrameUpdateApple2Type() override;
    virtual void FrameSetCursorPosByMousePos() override;

    virtual void SetFullScreenShowSubunitStatus(bool bShow) override;
    virtual void SetWindowedModeShowDiskiiStatus(bool bShow) override;
    virtual bool GetBestDisplayResolutionForFullScreen(UINT& bestWidth, UINT& bestHeight,
                                                       UINT userSpecifiedWidth = 0,
                                                       UINT userSpecifiedHeight = 0) override;
    virtual int  SetViewportScale(int nNewScale, bool bForce = false) override;
    virtual void SetAltEnterToggleFullScreen(bool mode) override;
    virtual void SetLoadedSaveStateFlag(const bool bFlag) override;

    virtual void VideoPresentScreen(void) override;
    virtual void ResizeWindow(void) override;

    virtual int  FrameMessageBox(LPCSTR lpText, LPCSTR lpCaption, UINT uType) override;
    virtual void GetBitmap(WORD id, LONG cb, LPVOID lpvBits) override;

    virtual std::shared_ptr<NetworkBackend> CreateNetworkBackend(const std::string& interfaceName) override;
    virtual std::shared_ptr<SoundBuffer>    CreateSoundBuffer(uint32_t dwBufferSize,
                                                               uint32_t nSampleRate,
                                                               int nChannels,
                                                               const char* pszVoiceName) override;

    virtual BYTE* GetResource(WORD id, LPCSTR lpType, uint32_t expectedSize) override;
    virtual void  Restart() override;
    virtual std::string Video_GetScreenShotFolder() const override;

private:
    std::string m_systemDir;
    std::vector<uint8_t> m_resourceCache;
    std::vector<uint8_t> m_framebuffer; // BGRA framebuffer

    const char* ResourceIdToFilename(WORD id);
};
