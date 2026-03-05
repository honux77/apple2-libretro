#pragma once

// Windows type compatibility for non-Windows platforms

#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <memory>
#include <string>

// Basic Windows types
typedef uint8_t   BYTE;
typedef uint16_t  WORD;
typedef uint32_t  DWORD;
typedef int64_t   LONGLONG;
typedef uint64_t  ULONGLONG;
typedef int32_t   LONG;
typedef uint32_t  ULONG;
typedef int       BOOL;
typedef uint8_t*  LPBYTE;
typedef uint16_t* LPWORD;
typedef uint32_t* LPDWORD;
typedef long*     LPLONG;
typedef void*     LPVOID;
typedef const void* LPCVOID;
typedef char*     LPSTR;
typedef const char* LPCSTR;
typedef unsigned int UINT;
typedef uint16_t  UINT16;
typedef uint32_t  UINT32;
typedef uint64_t  UINT64;
typedef int16_t   INT16;
typedef int32_t   INT32;
typedef uintptr_t ULONG_PTR;
typedef intptr_t  LONG_PTR;

// Windows HANDLE - use void* for portability
typedef void*     HANDLE;
typedef void*     HINSTANCE;
typedef void*     HWND;
typedef void*     HMODULE;
typedef uint16_t  WCHAR;

// HRESULT
typedef long HRESULT;
#define S_OK          ((HRESULT)0)
#define S_FALSE       ((HRESULT)1)
#define E_FAIL        ((HRESULT)0x80004005L)
#define E_NOTIMPL     ((HRESULT)0x80004001L)
#define E_OUTOFMEMORY ((HRESULT)0x8007000EL)
#define FAILED(hr)    ((hr) < 0)
#define SUCCEEDED(hr) ((hr) >= 0)

// INVALID_HANDLE_VALUE
#define INVALID_HANDLE_VALUE ((HANDLE)(LONG_PTR)-1)

// Boolean
#ifndef TRUE
#define TRUE  1
#endif
#ifndef FALSE
#define FALSE 0
#endif

// Assert
#define _ASSERT(x) assert(x)
#define _ASSERTE(x) assert(x)

// Windows message box flags (stubs)
#define MB_OK                0x00000000L
#define MB_OKCANCEL          0x00000001L
#define MB_YESNOCANCEL       0x00000003L
#define MB_YESNO             0x00000004L
#define MB_ICONWARNING       0x00000030L
#define MB_ICONERROR         0x00000010L
#define MB_ICONINFORMATION   0x00000040L
#define IDOK                 1
#define IDCANCEL             2
#define IDYES                6
#define IDNO                 7

// DirectSound flags
#define DSBPLAY_LOOPING      0x00000001L
#define DSBSTATUS_PLAYING    0x00000001L
#define DSBSTATUS_BUFFERLOST 0x00000002L
#define DSBSTATUS_LOOPING    0x00000004L
#define DSBLOCK_FROMWRITECURSOR 0x00000001L
#define DSBLOCK_ENTIREBUFFER    0x00000002L
#define DSBVOLUME_MIN        (-10000)
#define DSBVOLUME_MAX        0

// String functions
#ifndef _stricmp
#define _stricmp strcasecmp
#endif
#ifndef _strnicmp
#define _strnicmp strncasecmp
#endif

// Min/Max (avoid collisions with Common.h)
#ifndef NOMINMAX
#define NOMINMAX
#endif

// MAKEINTRESOURCE stub
#define MAKEINTRESOURCE(i) ((LPCSTR)((ULONG_PTR)((WORD)(i))))

// RT_* resource type stubs
#define RT_BITMAP  ((LPCSTR)2)
#define RT_RCDATA  ((LPCSTR)10)

// LARGE_INTEGER stub
typedef union _LARGE_INTEGER {
    struct { DWORD LowPart; LONG HighPart; };
    LONGLONG QuadPart;
} LARGE_INTEGER;

// POINT / SIZE / RECT stubs
typedef struct tagPOINT { LONG x; LONG y; } POINT;
typedef struct tagSIZE  { LONG cx; LONG cy; } SIZE;
typedef struct tagRECT  { LONG left; LONG top; LONG right; LONG bottom; } RECT;

// WM_USER for Common.h
#define WM_USER 0x0400

// Windows Multimedia Joystick API stubs (for FourPlay.cpp)
#define JOYERR_NOERROR    0
#define JOY_RETURNPOV     0x0040
#define JOY_RETURNBUTTONS 0x0080

typedef struct tagJOYINFOEX {
    DWORD dwSize;
    DWORD dwFlags;
    DWORD dwXpos;
    DWORD dwYpos;
    DWORD dwZpos;
    DWORD dwRpos;
    DWORD dwUpos;
    DWORD dwVpos;
    DWORD dwButtons;
    DWORD dwButtonNumber;
    DWORD dwPOV;
    DWORD dwReserved1;
    DWORD dwReserved2;
} JOYINFOEX;

static inline DWORD joyGetPosEx(UINT /*uJoyID*/, JOYINFOEX* /*pji*/) { return 1; /* not JOYERR_NOERROR */ }

// GetAsyncKeyState stub (always returns not-pressed)
static inline short GetAsyncKeyState(int /*vKey*/) { return 0; }

