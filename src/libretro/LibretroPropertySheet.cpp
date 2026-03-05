// LibretroPropertySheet.cpp - stub IPropertySheet for libretro port

#include "StdAfx.h"
#include "LibretroPropertySheet.h"

LibretroPropertySheet::LibretroPropertySheet()
    : m_scrollLockToggle(0)
    , m_joystickCursorControl(0)
    , m_joystickCenteringControl(0)
    , m_autofire(0)
    , m_buttonsSwapped(false)
    , m_mouseShowCrosshair(0)
    , m_mouseRestrictToWindow(0)
    , m_theFreezesF8Rom(0)
{
}

void LibretroPropertySheet::Init(void)
{
    // No-op for libretro
}

uint32_t LibretroPropertySheet::GetVolumeMax(void)
{
    return 127;
}

bool LibretroPropertySheet::SaveStateSelectImage(HWND /*hWindow*/, bool /*bSave*/)
{
    return false;
}

void LibretroPropertySheet::ApplyNewConfig(const CConfigNeedingRestart& /*ConfigNew*/,
                                            const CConfigNeedingRestart& /*ConfigOld*/)
{
    // No-op for libretro
}

void LibretroPropertySheet::ApplyNewConfigFromSnapshot(const CConfigNeedingRestart& /*ConfigNew*/)
{
    // No-op for libretro
}

void LibretroPropertySheet::ConfigSaveApple2Type(eApple2Type /*apple2Type*/)
{
    // No-op for libretro
}

UINT LibretroPropertySheet::GetScrollLockToggle(void)
{
    return m_scrollLockToggle;
}

void LibretroPropertySheet::SetScrollLockToggle(UINT uValue)
{
    m_scrollLockToggle = uValue;
}

UINT LibretroPropertySheet::GetJoystickCursorControl(void)
{
    return m_joystickCursorControl;
}

void LibretroPropertySheet::SetJoystickCursorControl(UINT uValue)
{
    m_joystickCursorControl = uValue;
}

UINT LibretroPropertySheet::GetJoystickCenteringControl(void)
{
    return m_joystickCenteringControl;
}

void LibretroPropertySheet::SetJoystickCenteringControl(UINT uValue)
{
    m_joystickCenteringControl = uValue;
}

UINT LibretroPropertySheet::GetAutofire(UINT /*uButton*/)
{
    return m_autofire;
}

void LibretroPropertySheet::SetAutofire(UINT uValue)
{
    m_autofire = uValue;
}

bool LibretroPropertySheet::GetButtonsSwapState(void)
{
    return m_buttonsSwapped;
}

void LibretroPropertySheet::SetButtonsSwapState(bool value)
{
    m_buttonsSwapped = value;
}

UINT LibretroPropertySheet::GetMouseShowCrosshair(void)
{
    return m_mouseShowCrosshair;
}

void LibretroPropertySheet::SetMouseShowCrosshair(UINT uValue)
{
    m_mouseShowCrosshair = uValue;
}

UINT LibretroPropertySheet::GetMouseRestrictToWindow(void)
{
    return m_mouseRestrictToWindow;
}

void LibretroPropertySheet::SetMouseRestrictToWindow(UINT uValue)
{
    m_mouseRestrictToWindow = uValue;
}

UINT LibretroPropertySheet::GetTheFreezesF8Rom(void)
{
    return m_theFreezesF8Rom;
}

void LibretroPropertySheet::SetTheFreezesF8Rom(UINT uValue)
{
    m_theFreezesF8Rom = uValue;
}
