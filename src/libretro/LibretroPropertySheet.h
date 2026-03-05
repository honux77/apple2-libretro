#pragma once

#include "StdAfx.h"
#include "Configuration/IPropertySheet.h"

class LibretroPropertySheet : public IPropertySheet
{
public:
    LibretroPropertySheet();
    virtual ~LibretroPropertySheet() = default;

    // IPropertySheet pure virtual implementations
    virtual void Init(void) override;
    virtual uint32_t GetVolumeMax(void) override;
    virtual bool SaveStateSelectImage(HWND hWindow, bool bSave) override;
    virtual void ApplyNewConfig(const CConfigNeedingRestart& ConfigNew, const CConfigNeedingRestart& ConfigOld) override;
    virtual void ApplyNewConfigFromSnapshot(const CConfigNeedingRestart& ConfigNew) override;
    virtual void ConfigSaveApple2Type(eApple2Type apple2Type) override;

    virtual UINT GetScrollLockToggle(void) override;
    virtual void SetScrollLockToggle(UINT uValue) override;
    virtual UINT GetJoystickCursorControl(void) override;
    virtual void SetJoystickCursorControl(UINT uValue) override;
    virtual UINT GetJoystickCenteringControl(void) override;
    virtual void SetJoystickCenteringControl(UINT uValue) override;
    virtual UINT GetAutofire(UINT uButton) override;
    virtual void SetAutofire(UINT uValue) override;
    virtual bool GetButtonsSwapState(void) override;
    virtual void SetButtonsSwapState(bool value) override;
    virtual UINT GetMouseShowCrosshair(void) override;
    virtual void SetMouseShowCrosshair(UINT uValue) override;
    virtual UINT GetMouseRestrictToWindow(void) override;
    virtual void SetMouseRestrictToWindow(UINT uValue) override;
    virtual UINT GetTheFreezesF8Rom(void) override;
    virtual void SetTheFreezesF8Rom(UINT uValue) override;

private:
    UINT m_scrollLockToggle;
    UINT m_joystickCursorControl;
    UINT m_joystickCenteringControl;
    UINT m_autofire;
    bool m_buttonsSwapped;
    UINT m_mouseShowCrosshair;
    UINT m_mouseRestrictToWindow;
    UINT m_theFreezesF8Rom;
};
