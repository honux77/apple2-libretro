#pragma once
// Stub Debugger/Debug.h for libretro build - no debugger functionality

#include <stdint.h>

extern uint32_t extbench;

static inline void DebuggerBreakOnDmaToOrFromIoMemory(unsigned short /*nAddress*/, bool /*isDmaToMemory*/) {}
static inline bool DebuggerCheckMemBreakpoints(unsigned short /*nAddress*/, unsigned short /*nSize*/, bool /*isDmaToMemory*/) { return false; }
static inline void DebugReset(void) {}
static inline void DebugDisplay(int /*bActive*/) {}
