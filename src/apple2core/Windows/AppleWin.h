#pragma once
// Stub Windows/AppleWin.h for libretro build

static inline void SingleStep(bool /*bReinit*/) {}
static inline bool GetLoadedSaveStateFlag(void) { return false; }
static inline bool GetHookAltTab(void) { return false; }
static inline bool GetHookAltGrControl(void) { return false; }
static inline bool GetFullScreenResolutionChangedByUser(void) { return false; }

extern bool g_bRestartFullScreen;
