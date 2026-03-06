#pragma once

// Non-Windows compatibility header for AppleWin
// Included by StdAfx.h on non-_WIN32 builds via: #include "windows.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <errno.h>

// -----------------------------------------------------------------------
// Microsoft-specific integer types
// -----------------------------------------------------------------------
// __int64 as a macro so "unsigned __int64" becomes "unsigned long long"
#ifndef __int64
#define __int64 long long
#endif

// -----------------------------------------------------------------------
// __stdcall / __cdecl - ignored on Linux
// -----------------------------------------------------------------------
#ifndef __stdcall
#define __stdcall
#endif
#ifndef __cdecl
#define __cdecl
#endif
#ifndef WINAPI
#define WINAPI
#endif
#ifndef CALLBACK
#define CALLBACK
#endif

// -----------------------------------------------------------------------
// __forceinline - MSVC keyword, map to GCC equivalent
// -----------------------------------------------------------------------
#ifndef __forceinline
#define __forceinline __attribute__((always_inline)) inline
#endif

// -----------------------------------------------------------------------
// CRITICAL_SECTION - Windows threading primitive (stub as pthreads mutex)
// -----------------------------------------------------------------------
#include <pthread.h>
typedef pthread_mutex_t CRITICAL_SECTION;
static inline void InitializeCriticalSection(CRITICAL_SECTION* cs) { pthread_mutex_init(cs, NULL); }
static inline void DeleteCriticalSection(CRITICAL_SECTION* cs)     { pthread_mutex_destroy(cs); }
static inline void EnterCriticalSection(CRITICAL_SECTION* cs)      { pthread_mutex_lock(cs); }
static inline void LeaveCriticalSection(CRITICAL_SECTION* cs)      { pthread_mutex_unlock(cs); }

// -----------------------------------------------------------------------
// Basic Windows types
// -----------------------------------------------------------------------
typedef uint8_t   BYTE;
typedef uint16_t  WORD;
typedef uint32_t  DWORD;
typedef int64_t   LONGLONG;
typedef uint64_t  ULONGLONG;
typedef int32_t   LONG;
typedef uint32_t  ULONG;
typedef int       BOOL;
typedef uint8_t*  LPBYTE;
typedef const uint8_t* LPCBYTE;
typedef uint16_t* LPWORD;
typedef uint32_t* LPDWORD;
typedef long*     LPLONG;
typedef long*     PLONG;
typedef void*     LPVOID;
typedef const void* LPCVOID;
typedef char*     LPSTR;
typedef const char* LPCSTR;

// TCHAR = char (non-Unicode, ASCII mode)
typedef char      TCHAR;
typedef char*     LPTSTR;
typedef const char* LPCTSTR;

typedef unsigned int  UINT;
typedef uint8_t       UCHAR;
typedef uint16_t      USHORT;
typedef int16_t       SHORT;
typedef int           INT;
typedef uint16_t      UINT16;
typedef uint32_t      UINT32;
typedef uint64_t      UINT64;
typedef int16_t       INT16;
typedef int32_t       INT32;
typedef uintptr_t     ULONG_PTR;
typedef intptr_t      LONG_PTR;
typedef uintptr_t     UINT_PTR;
typedef intptr_t      INT_PTR;
typedef size_t        SIZE_T;
typedef uint16_t      WCHAR;
typedef WCHAR*        LPWSTR;
typedef const WCHAR*  LPCWSTR;
typedef int           errno_t;

// -----------------------------------------------------------------------
// WPARAM / LPARAM (needed early for function prototypes)
// -----------------------------------------------------------------------
typedef UINT_PTR WPARAM;
typedef LONG_PTR LPARAM;
typedef LONG_PTR LRESULT;

// -----------------------------------------------------------------------
// Windows HANDLE types
// -----------------------------------------------------------------------
typedef void*   HANDLE;
typedef void*   HINSTANCE;
typedef void*   HWND;
typedef void*   HMODULE;
typedef void*   HDC;
typedef void*   HBITMAP;
typedef void*   HFONT;
typedef void*   HGDIOBJ;
typedef void*   HRSRC;
typedef void*   HGLOBAL;
typedef void*   HACCEL;

// Registry key handle - just an int
typedef void*   HKEY;
typedef HKEY*   PHKEY;

// -----------------------------------------------------------------------
// Registry constants
// -----------------------------------------------------------------------
#define HKEY_CURRENT_USER   ((HKEY)(ULONG_PTR)0x80000001)
#define HKEY_LOCAL_MACHINE  ((HKEY)(ULONG_PTR)0x80000002)

typedef LONG    LSTATUS;
#define ERROR_SUCCESS           0L
#define ERROR_FILE_NOT_FOUND    2L
#define ERROR_MORE_DATA         234L

#define REG_SZ              1
#define REG_DWORD           4
#define REG_BINARY          3

#define KEY_READ            0x20019
#define KEY_WRITE           0x20006
#define REG_OPTION_NON_VOLATILE  0

typedef void*   LPSECURITY_ATTRIBUTES;

// -----------------------------------------------------------------------
// HRESULT
// -----------------------------------------------------------------------
typedef long HRESULT;
#define S_OK          ((HRESULT)0)
#define S_FALSE       ((HRESULT)1)
#define E_FAIL        ((HRESULT)0x80004005L)
#define E_NOTIMPL     ((HRESULT)0x80004001L)
#define E_OUTOFMEMORY ((HRESULT)0x8007000EL)
#define E_INVALIDARG  ((HRESULT)0x80070057L)
#define FAILED(hr)    ((hr) < 0)
#define SUCCEEDED(hr) ((hr) >= 0)

// -----------------------------------------------------------------------
// INVALID_HANDLE_VALUE
// -----------------------------------------------------------------------
#define INVALID_HANDLE_VALUE ((HANDLE)(LONG_PTR)-1)

// -----------------------------------------------------------------------
// Boolean
// -----------------------------------------------------------------------
#ifndef TRUE
#define TRUE  1
#endif
#ifndef FALSE
#define FALSE 0
#endif

// -----------------------------------------------------------------------
// Assert
// -----------------------------------------------------------------------
#define _ASSERT(x) assert(x)
#define _ASSERTE(x) assert(x)

// -----------------------------------------------------------------------
// Windows message box flags
// -----------------------------------------------------------------------
#define MB_OK                0x00000000L
#define MB_OKCANCEL          0x00000001L
#define MB_YESNOCANCEL       0x00000003L
#define MB_YESNO             0x00000004L
#define MB_ICONWARNING       0x00000030L
#define MB_ICONERROR         0x00000010L
#define MB_ICONINFORMATION   0x00000040L
#define MB_ICONQUESTION      0x00000020L
#define IDOK                 1
#define IDCANCEL             2
#define IDYES                6
#define IDNO                 7

// -----------------------------------------------------------------------
// DirectSound flags (stubs)
// -----------------------------------------------------------------------
#define DSBPLAY_LOOPING         0x00000001L
#define DSBSTATUS_PLAYING       0x00000001L
#define DSBSTATUS_BUFFERLOST    0x00000002L
#define DSBSTATUS_LOOPING       0x00000004L
#define DSBLOCK_FROMWRITECURSOR 0x00000001L
#define DSBLOCK_ENTIREBUFFER    0x00000002L
#define DSBVOLUME_MIN           (-10000)
#define DSBVOLUME_MAX           0

// -----------------------------------------------------------------------
// String type helpers
// -----------------------------------------------------------------------
#ifndef _stricmp
#define _stricmp  strcasecmp
#endif
#ifndef _strnicmp
#define _strnicmp strncasecmp
#endif
#ifndef _snprintf
#define _snprintf snprintf
#endif

// -----------------------------------------------------------------------
// Min/Max
// -----------------------------------------------------------------------
#ifndef NOMINMAX
#define NOMINMAX
#endif

// -----------------------------------------------------------------------
// Bitmap compression constants
// -----------------------------------------------------------------------
#define BI_RGB       0
#define BI_RLE8      1
#define BI_RLE4      2
#define BI_BITFIELDS 3

// -----------------------------------------------------------------------
// RGBQUAD (GDI bitmap palette entry)
// -----------------------------------------------------------------------
typedef struct tagRGBQUAD {
    BYTE rgbBlue;
    BYTE rgbGreen;
    BYTE rgbRed;
    BYTE rgbReserved;
} RGBQUAD;

// -----------------------------------------------------------------------
// COLORREF (RGB packed)
// -----------------------------------------------------------------------
typedef DWORD COLORREF;
#define RGB(r,g,b) ((COLORREF)(((BYTE)(r))|((WORD)((BYTE)(g))<<8)|(((DWORD)(BYTE)(b))<<16)))
#define GetRValue(c) ((BYTE)(c))
#define GetGValue(c) ((BYTE)(((WORD)(c)) >> 8))
#define GetBValue(c) ((BYTE)((c) >> 16))

// -----------------------------------------------------------------------
// MAKEINTRESOURCE stub
// -----------------------------------------------------------------------
#define MAKEINTRESOURCE(i) ((LPCSTR)((ULONG_PTR)((WORD)(i))))

// RT_* resource type stubs
#define RT_BITMAP  ((LPCSTR)2)
#define RT_RCDATA  ((LPCSTR)10)

// -----------------------------------------------------------------------
// LARGE_INTEGER
// -----------------------------------------------------------------------
typedef union _LARGE_INTEGER {
    struct { DWORD LowPart; LONG HighPart; };
    LONGLONG QuadPart;
} LARGE_INTEGER;

typedef union _ULARGE_INTEGER {
    struct { DWORD LowPart; DWORD HighPart; };
    ULONGLONG QuadPart;
} ULARGE_INTEGER;

// -----------------------------------------------------------------------
// POINT / SIZE / RECT
// -----------------------------------------------------------------------
typedef struct tagPOINT { LONG x; LONG y; } POINT;
typedef struct tagSIZE  { LONG cx; LONG cy; } SIZE;
typedef struct tagRECT  { LONG left; LONG top; LONG right; LONG bottom; } RECT;

// -----------------------------------------------------------------------
// WM_USER for Common.h
// -----------------------------------------------------------------------
#define WM_USER     0x0400
#define WM_KEYDOWN  0x0100
#define WM_KEYUP    0x0101
#define WM_CHAR     0x0102
#define WM_SYSKEYDOWN 0x0104
#define WM_SYSKEYUP   0x0105

// -----------------------------------------------------------------------
// Virtual key codes (stub)
// -----------------------------------------------------------------------
#define VK_LBUTTON  0x01
#define VK_RBUTTON  0x02
#define VK_BACK     0x08
#define VK_TAB      0x09
#define VK_RETURN   0x0D
#define VK_SHIFT    0x10
#define VK_CONTROL  0x11
#define VK_MENU     0x12
#define VK_CAPITAL  0x14
#define VK_ESCAPE   0x1B
#define VK_SPACE    0x20
#define VK_PRIOR    0x21
#define VK_NEXT     0x22
#define VK_END      0x23
#define VK_HOME     0x24
#define VK_LEFT     0x25
#define VK_UP       0x26
#define VK_RIGHT    0x27
#define VK_DOWN     0x28
#define VK_INSERT   0x2D
#define VK_DELETE   0x2E
#define VK_F1       0x70
#define VK_F2       0x71
#define VK_F3       0x72
#define VK_F4       0x73
#define VK_F5       0x74
#define VK_F6       0x75
#define VK_F7       0x76
#define VK_F8       0x77
#define VK_F9       0x78
#define VK_F10      0x79
#define VK_F11      0x7A
#define VK_F12      0x7B
#define VK_NUMPAD0  0x60
#define VK_NUMPAD1  0x61
#define VK_NUMPAD2  0x62
#define VK_NUMPAD3  0x63
#define VK_NUMPAD4  0x64
#define VK_NUMPAD5  0x65
#define VK_NUMPAD6  0x66
#define VK_NUMPAD7  0x67
#define VK_NUMPAD8  0x68
#define VK_NUMPAD9  0x69

// -----------------------------------------------------------------------
// CONST
// -----------------------------------------------------------------------
#ifndef CONST
#define CONST const
#endif

// -----------------------------------------------------------------------
// GetTickCount() - return milliseconds
// -----------------------------------------------------------------------
static inline DWORD GetTickCount(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (DWORD)(ts.tv_sec * 1000ULL + ts.tv_nsec / 1000000ULL);
}

// -----------------------------------------------------------------------
// GetLastError() - always success
// -----------------------------------------------------------------------
static inline DWORD GetLastError(void)
{
    return 0;
}

// -----------------------------------------------------------------------
// SetCurrentDirectory
// -----------------------------------------------------------------------
static inline BOOL SetCurrentDirectory(LPCSTR path)
{
    return (chdir(path) == 0) ? TRUE : FALSE;
}

// -----------------------------------------------------------------------
// GetCurrentDirectory
// -----------------------------------------------------------------------
static inline DWORD GetCurrentDirectory(DWORD nBufferLength, LPTSTR lpBuffer)
{
    if (getcwd(lpBuffer, nBufferLength) == NULL) return 0;
    return (DWORD)strlen(lpBuffer);
}

// -----------------------------------------------------------------------
// Registry stubs - all return failure (no-op)
// -----------------------------------------------------------------------
static inline LSTATUS RegOpenKeyEx(HKEY /*hKey*/, LPCSTR /*lpSubKey*/,
    DWORD /*ulOptions*/, DWORD /*samDesired*/, PHKEY /*phkResult*/)
{
    return ERROR_FILE_NOT_FOUND;
}

static inline LSTATUS RegCloseKey(HKEY /*hKey*/)
{
    return ERROR_SUCCESS;
}

static inline LSTATUS RegQueryValueEx(HKEY /*hKey*/, LPCSTR /*lpValueName*/,
    LPDWORD /*lpReserved*/, LPDWORD /*lpType*/, LPBYTE /*lpData*/, LPDWORD /*lpcbData*/)
{
    return ERROR_FILE_NOT_FOUND;
}

static inline LSTATUS RegCreateKeyEx(HKEY /*hKey*/, LPCSTR /*lpSubKey*/,
    DWORD /*Reserved*/, LPSTR /*lpClass*/, DWORD /*dwOptions*/,
    DWORD /*samDesired*/, LPSECURITY_ATTRIBUTES /*lpSecurityAttributes*/,
    PHKEY /*phkResult*/, LPDWORD /*lpdwDisposition*/)
{
    return ERROR_FILE_NOT_FOUND;
}

static inline LSTATUS RegSetValueEx(HKEY /*hKey*/, LPCSTR /*lpValueName*/,
    DWORD /*Reserved*/, DWORD /*dwType*/, CONST BYTE* /*lpData*/, DWORD /*cbData*/)
{
    return ERROR_FILE_NOT_FOUND;
}

static inline LSTATUS RegDeleteKey(HKEY /*hKey*/, LPCSTR /*lpSubKey*/)
{
    return ERROR_FILE_NOT_FOUND;
}

// -----------------------------------------------------------------------
// INI file stubs (used by Registry.cpp _ini:: path)
// -----------------------------------------------------------------------
static inline DWORD GetPrivateProfileString(LPCSTR /*lpAppName*/, LPCSTR /*lpKeyName*/,
    LPCSTR lpDefault, LPSTR lpReturnedString, DWORD nSize, LPCSTR /*lpFileName*/)
{
    if (lpDefault && lpReturnedString && nSize > 0)
    {
        strncpy(lpReturnedString, lpDefault, nSize - 1);
        lpReturnedString[nSize - 1] = '\0';
        return (DWORD)strlen(lpReturnedString);
    }
    if (lpReturnedString && nSize > 0)
        lpReturnedString[0] = '\0';
    return 0;
}

static inline BOOL WritePrivateProfileString(LPCSTR /*lpAppName*/, LPCSTR /*lpKeyName*/,
    LPCSTR /*lpString*/, LPCSTR /*lpFileName*/)
{
    return TRUE; // pretend success
}

// -----------------------------------------------------------------------
// Strsafe equivalents
// -----------------------------------------------------------------------
#include <errno.h>

typedef long HRESULT_STRSAFE;

static inline HRESULT StringCbCopy(LPTSTR pszDest, size_t cbDest, LPCTSTR pszSrc)
{
    if (!pszDest || cbDest == 0) return E_INVALIDARG;
    strncpy(pszDest, pszSrc ? pszSrc : "", cbDest - 1);
    pszDest[cbDest - 1] = '\0';
    return S_OK;
}

static inline HRESULT StringCbCat(LPTSTR pszDest, size_t cbDest, LPCTSTR pszSrc)
{
    if (!pszDest || cbDest == 0) return E_INVALIDARG;
    size_t existing = strlen(pszDest);
    if (existing >= cbDest - 1) return E_FAIL;
    strncat(pszDest, pszSrc ? pszSrc : "", cbDest - existing - 1);
    return S_OK;
}

static inline HRESULT StringCbPrintf(LPTSTR pszDest, size_t cbDest, LPCTSTR pszFormat, ...)
{
    if (!pszDest || cbDest == 0) return E_INVALIDARG;
    va_list args;
    va_start(args, pszFormat);
    vsnprintf(pszDest, cbDest, pszFormat, args);
    va_end(args);
    return S_OK;
}

static inline HRESULT StringCchCopy(LPTSTR pszDest, size_t cchDest, LPCTSTR pszSrc)
{
    return StringCbCopy(pszDest, cchDest, pszSrc);
}

static inline HRESULT StringCchCat(LPTSTR pszDest, size_t cchDest, LPCTSTR pszSrc)
{
    return StringCbCat(pszDest, cchDest, pszSrc);
}

// -----------------------------------------------------------------------
// QueryPerformanceCounter / QueryPerformanceFrequency
// -----------------------------------------------------------------------
static inline BOOL QueryPerformanceCounter(LARGE_INTEGER* lpPerformanceCount)
{
    if (!lpPerformanceCount) return FALSE;
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    lpPerformanceCount->QuadPart = (LONGLONG)ts.tv_sec * 1000000000LL + ts.tv_nsec;
    return TRUE;
}

static inline BOOL QueryPerformanceFrequency(LARGE_INTEGER* lpFrequency)
{
    if (!lpFrequency) return FALSE;
    lpFrequency->QuadPart = 1000000000LL; // nanosecond frequency
    return TRUE;
}

// -----------------------------------------------------------------------
// OutputDebugString - no-op
// -----------------------------------------------------------------------
static inline void OutputDebugString(LPCSTR /*lpOutputString*/)
{
    // no-op on Linux
}
static inline void OutputDebugStringA(LPCSTR /*lpOutputString*/)
{
    // no-op on Linux
}

// -----------------------------------------------------------------------
// File/path helpers
// -----------------------------------------------------------------------
#define MAX_PATH 260

static inline BOOL PathFileExists(LPCSTR /*pszPath*/)
{
    return FALSE;
}

// -----------------------------------------------------------------------
// VOID type
// -----------------------------------------------------------------------
#ifndef VOID
#define VOID void
#endif
#define CALLBACK

// -----------------------------------------------------------------------
// DWORD_PTR
// -----------------------------------------------------------------------
typedef uintptr_t DWORD_PTR;
typedef LONG_PTR  SSIZE_T;

// -----------------------------------------------------------------------
// DirectSound error codes
// -----------------------------------------------------------------------
#define DS_OK            S_OK
#define DSERR_BUFFERLOST ((HRESULT)0x88780096L)

// -----------------------------------------------------------------------
// Windows timer stubs
// -----------------------------------------------------------------------
typedef void (*TIMERPROC)(HWND, UINT, UINT_PTR, DWORD);
static inline UINT_PTR SetTimer(HWND /*hWnd*/, UINT_PTR /*nIDEvent*/, UINT /*uElapse*/, TIMERPROC /*lpTimerFunc*/) { return 0; }
static inline BOOL KillTimer(HWND /*hWnd*/, UINT_PTR /*uIDEvent*/) { return TRUE; }

// -----------------------------------------------------------------------
// Semaphore stubs
// -----------------------------------------------------------------------
static inline HANDLE CreateSemaphore(LPSECURITY_ATTRIBUTES /*lpSemaphoreAttributes*/, LONG /*lInitialCount*/, LONG /*lMaximumCount*/, LPCSTR /*lpName*/) { return INVALID_HANDLE_VALUE; }
static inline BOOL ReleaseSemaphore(HANDLE /*hSemaphore*/, LONG /*lReleaseCount*/, LPLONG /*lpPreviousCount*/) { return TRUE; }

// -----------------------------------------------------------------------
// COM stubs (IReferenceClock etc.)
// -----------------------------------------------------------------------
#define CLSCTX_INPROC 0x3
typedef void* LPVOID_COM;
static inline HRESULT CoCreateInstance(int /*rclsid*/, void* /*pUnkOuter*/, DWORD /*dwClsContext*/, int /*riid*/, void** ppv) { if (ppv) *ppv = NULL; return E_NOTIMPL; }

typedef LONGLONG REFERENCE_TIME;
typedef HANDLE   HSEMAPHORE;

struct IReferenceClock {
    virtual ~IReferenceClock() {}
    virtual HRESULT GetTime(REFERENCE_TIME* /*pTime*/) { return E_NOTIMPL; }
    virtual HRESULT AdviseTime(REFERENCE_TIME /*baseTime*/, REFERENCE_TIME /*streamTime*/, HANDLE /*hEvent*/, DWORD_PTR* /*pdwAdviseCookie*/) { return E_NOTIMPL; }
    virtual HRESULT AdvisePeriodic(REFERENCE_TIME /*startTime*/, REFERENCE_TIME /*periodTime*/, HSEMAPHORE /*hSemaphore*/, DWORD_PTR* /*pdwAdviseCookie*/) { return E_NOTIMPL; }
    virtual HRESULT Unadvise(DWORD_PTR /*dwAdviseCookie*/) { return E_NOTIMPL; }
    virtual ULONG AddRef() { return 1; }
    virtual ULONG Release() { return 0; }
};
// Dummy CLSID/IID constants
static const int CLSID_SystemClock = 0;
static const int IID_IReferenceClock = 0;

// -----------------------------------------------------------------------
// Misc Windows stubs
// -----------------------------------------------------------------------
#define LOWORD(l)  ((WORD)(l))
#define HIWORD(l)  ((WORD)(((DWORD)(l) >> 16) & 0xFFFF))
#define LOBYTE(w)  ((BYTE)(w))
#define HIBYTE(w)  ((BYTE)(((WORD)(w) >> 8) & 0xFF))
#define MAKELONG(low, high) ((LONG)(((WORD)(low)) | (((DWORD)((WORD)(high))) << 16)))
#define MAKEWORD(low, high) ((WORD)(((BYTE)(low)) | (((WORD)((BYTE)(high))) << 8)))

// SS_CARDTYPE is defined in Card.h, not here
// DONGLETYPE is defined in CopyProtectionDongles.h

// SLOT_AUX and GAME_IO_CONNECTOR are in CardManager.h

// -----------------------------------------------------------------------
// _strtoui64 alias
// -----------------------------------------------------------------------
#define _strtoui64 strtoull

// -----------------------------------------------------------------------
// MultiByteToWideChar / WideCharToMultiByte stubs
// -----------------------------------------------------------------------
#define CP_ACP  0
#define CP_UTF8 65001
#define MB_ERR_INVALID_CHARS 0x0008
// LPWSTR is already char* in this stub, so MultiByteToWideChar doesn't make sense
// We just stub them as no-ops
static inline int MultiByteToWideChar(UINT /*CodePage*/, DWORD /*dwFlags*/, const char* /*lpMultiByteStr*/, int /*cbMultiByte*/, WCHAR* /*lpWideCharStr*/, int /*cchWideChar*/) { return 0; }
static inline int WideCharToMultiByte(UINT /*CodePage*/, DWORD /*dwFlags*/, const WCHAR* /*lpWideCharStr*/, int /*cchWideChar*/, char* /*lpMultiByteStr*/, int /*cbMultiByte*/, const char* /*lpDefaultChar*/, int* /*lpUsedDefaultChar*/) { return 0; }

// -----------------------------------------------------------------------
// sprintf_s alias
// -----------------------------------------------------------------------
#define sprintf_s(buf, size, ...) snprintf(buf, size, __VA_ARGS__)
#define sscanf_s sscanf
#define fopen_s(pFile, filename, mode) ((*pFile = fopen(filename, mode)) ? 0 : errno)

// -----------------------------------------------------------------------
// _tzset alias
// -----------------------------------------------------------------------
#define _tzset tzset

// -----------------------------------------------------------------------
// ctime_s - MSVC safe version of ctime (not in POSIX)
// -----------------------------------------------------------------------
static inline errno_t ctime_s(char* buf, size_t bufSize, const time_t* timer)
{
    if (!buf || bufSize < 26 || !timer) return EINVAL;
    char* result = ctime_r(timer, buf);
    return result ? 0 : EINVAL;
}

// -----------------------------------------------------------------------
// File attributes
// -----------------------------------------------------------------------
#define FILE_ATTRIBUTE_NORMAL    0x00000080
#define FILE_ATTRIBUTE_READONLY  0x00000001
#define FILE_ATTRIBUTE_DIRECTORY 0x00000010
#define INVALID_FILE_ATTRIBUTES  ((DWORD)-1)

static inline DWORD GetFileAttributes(LPCSTR lpFileName)
{
    struct stat st;
    if (stat(lpFileName, &st) != 0) return INVALID_FILE_ATTRIBUTES;
    if (S_ISDIR(st.st_mode)) return FILE_ATTRIBUTE_DIRECTORY;
    return FILE_ATTRIBUTE_NORMAL;
}

// -----------------------------------------------------------------------
// File seek constants
// -----------------------------------------------------------------------
#define FILE_BEGIN   0
#define FILE_CURRENT 1
#define FILE_END     2

#define INVALID_SET_FILE_POINTER  ((DWORD)-1)
#define FILE_SHARE_READ  0x00000001
#define FILE_SHARE_WRITE 0x00000002

static inline DWORD SetFilePointer(HANDLE hFile, LONG lDistanceToMove,
    PLONG /*lpDistanceToMoveHigh*/, DWORD dwMoveMethod)
{
    int fd = (int)(intptr_t)hFile;
    if (fd < 0) return INVALID_SET_FILE_POINTER;
    int whence = (dwMoveMethod == 0) ? SEEK_SET : (dwMoveMethod == 1) ? SEEK_CUR : SEEK_END;
    off_t pos = lseek(fd, lDistanceToMove, whence);
    return (pos == (off_t)-1) ? INVALID_SET_FILE_POINTER : (DWORD)pos;
}

static inline BOOL DeleteFile(LPCSTR /*lpFileName*/) { return FALSE; }
static inline BOOL MoveFile(LPCSTR /*lpExistingFileName*/, LPCSTR /*lpNewFileName*/) { return FALSE; }
static inline BOOL CopyFile(LPCSTR /*lpExistingFileName*/, LPCSTR /*lpNewFileName*/, BOOL /*bFailIfExists*/) { return FALSE; }
static inline BOOL SetEndOfFile(HANDLE /*hFile*/) { return FALSE; }

// -----------------------------------------------------------------------
// _splitpath / _makepath (CRT extension)
// -----------------------------------------------------------------------
#define _MAX_DRIVE  3
#define _MAX_DIR    256
#define _MAX_FNAME  256
#define _MAX_EXT    256
#define _MAX_PATH   260

#include <libgen.h>
static inline void _splitpath(const char* path, char* drive, char* dir, char* fname, char* ext)
{
    if (drive && _MAX_DRIVE > 0) drive[0] = '\0';

    char tmp1[_MAX_PATH], tmp2[_MAX_PATH];
    strncpy(tmp1, path, _MAX_PATH - 1); tmp1[_MAX_PATH - 1] = '\0';
    strncpy(tmp2, path, _MAX_PATH - 1); tmp2[_MAX_PATH - 1] = '\0';

    if (dir)
    {
        char* d = dirname(tmp1);
        strncpy(dir, d, _MAX_DIR - 1);
        dir[_MAX_DIR - 1] = '\0';
        if (dir[0] != '\0') { // append slash if not empty
            size_t len = strlen(dir);
            if (len < (size_t)(_MAX_DIR - 2) && dir[len-1] != '/')
                { dir[len] = '/'; dir[len+1] = '\0'; }
        }
    }

    char* base = basename(tmp2);
    char* dot = strrchr(base, '.');
    if (fname)
    {
        if (dot) { size_t len = (size_t)(dot - base); strncpy(fname, base, len < _MAX_FNAME ? len : _MAX_FNAME - 1); fname[len < _MAX_FNAME ? len : _MAX_FNAME - 1] = '\0'; }
        else { strncpy(fname, base, _MAX_FNAME - 1); fname[_MAX_FNAME - 1] = '\0'; }
    }
    if (ext)
    {
        if (dot) { strncpy(ext, dot, _MAX_EXT - 1); ext[_MAX_EXT - 1] = '\0'; }
        else ext[0] = '\0';
    }
}

static inline void _makepath(char* path, const char* /*drive*/, const char* dir, const char* fname, const char* ext)
{
    path[0] = '\0';
    if (dir && dir[0]) { strcat(path, dir); }
    if (fname && fname[0]) { strcat(path, fname); }
    if (ext && ext[0]) { if (ext[0] != '.') strcat(path, "."); strcat(path, ext); }
}

// -----------------------------------------------------------------------
// strcpy_s / strcat_s
// -----------------------------------------------------------------------
#define strcpy_s(dst, size, src)  (strncpy(dst, src, size), (dst)[(size)-1] = '\0', 0)
#define strcat_s(dst, size, src)  (strncat(dst, src, (size) - strlen(dst) - 1), 0)

// -----------------------------------------------------------------------
// Character case functions (WinUser.h)
// -----------------------------------------------------------------------
#include <ctype.h>
static inline BOOL IsCharLower(char c) { return islower((unsigned char)c) != 0; }
static inline BOOL IsCharUpper(char c) { return isupper((unsigned char)c) != 0; }
static inline DWORD CharLowerBuff(LPSTR lpsz, DWORD cchLength)
{
    for (DWORD i = 0; i < cchLength; ++i)
        lpsz[i] = (char)tolower((unsigned char)lpsz[i]);
    return cchLength;
}
static inline DWORD CharUpperBuff(LPSTR lpsz, DWORD cchLength)
{
    for (DWORD i = 0; i < cchLength; ++i)
        lpsz[i] = (char)toupper((unsigned char)lpsz[i]);
    return cchLength;
}

// -----------------------------------------------------------------------
// Cursor stubs
// -----------------------------------------------------------------------
typedef void* HCURSOR;
#define IDC_WAIT  ((LPCSTR)32514)
#define IDC_ARROW ((LPCSTR)32512)
static inline HCURSOR LoadCursor(HINSTANCE /*hInstance*/, LPCSTR /*lpCursorName*/) { return NULL; }
static inline HCURSOR SetCursor(HCURSOR /*hCursor*/) { return NULL; }

// -----------------------------------------------------------------------
// Open File Dialog stub (no-op on libretro)
// -----------------------------------------------------------------------
#define OFN_PATHMUSTEXIST   0x00000800L
#define OFN_READONLY        0x00000001L
#define OFN_OVERWRITEPROMPT 0x00000002L
#define OFN_FILEMUSTEXIST   0x00001000L
#define OFN_HIDEREADONLY    0x00000004L
#define OFN_NOCHANGEDIR     0x00000008L

typedef struct tagOFNA {
    DWORD    lStructSize;
    HWND     hwndOwner;
    HINSTANCE hInstance;
    LPCSTR   lpstrFilter;
    LPSTR    lpstrCustomFilter;
    DWORD    nMaxCustFilter;
    DWORD    nFilterIndex;
    LPSTR    lpstrFile;
    DWORD    nMaxFile;
    LPSTR    lpstrFileTitle;
    DWORD    nMaxFileTitle;
    LPCSTR   lpstrInitialDir;
    LPCSTR   lpstrTitle;
    DWORD    Flags;
    WORD     nFileOffset;
    WORD     nFileExtension;
    LPCSTR   lpstrDefExt;
    LPARAM   lCustData;
    void*    lpfnHook;
    LPCSTR   lpTemplateName;
} OPENFILENAME;

static inline BOOL GetOpenFileName(OPENFILENAME* /*lpofn*/) { return FALSE; }
static inline BOOL GetSaveFileName(OPENFILENAME* /*lpofn*/) { return FALSE; }

// -----------------------------------------------------------------------
// MB_ICONEXCLAMATION alias
// -----------------------------------------------------------------------
#define MB_ICONEXCLAMATION  MB_ICONWARNING
#define MB_ICONASTERISK     MB_ICONINFORMATION
#define MB_DEFBUTTON1       0x00000000L
#define MB_DEFBUTTON2       0x00000100L

// -----------------------------------------------------------------------
// iofunction typedef used by Memory.h
// -----------------------------------------------------------------------
typedef BYTE (*iofunction)(WORD PC, WORD addr, BYTE bWrite, BYTE d, ULONG nExecutedCycles);

// -----------------------------------------------------------------------
// OVERLAPPED - Windows async I/O struct (stub) - defined early for use below
// -----------------------------------------------------------------------
typedef struct _OVERLAPPED {
    ULONG_PTR Internal;
    ULONG_PTR InternalHigh;
    union {
        struct { DWORD Offset; DWORD OffsetHigh; };
        LPVOID Pointer;
    };
    HANDLE hEvent;
} OVERLAPPED;
typedef OVERLAPPED* LPOVERLAPPED;

// -----------------------------------------------------------------------
// More message box flags
// -----------------------------------------------------------------------
#define MB_ICONSTOP          MB_ICONERROR
#define MB_SETFOREGROUND     0x00010000L
#define MB_TASKMODAL         0x00002000L
#define MB_APPLMODAL         0x00000000L

// -----------------------------------------------------------------------
// File I/O - POSIX implementation of Windows file API
// -----------------------------------------------------------------------
#define GENERIC_READ         0x80000000L
#define GENERIC_WRITE        0x40000000L
#define OPEN_EXISTING        3
#define CREATE_ALWAYS        2
#define CREATE_NEW           1
#define TRUNCATE_EXISTING    5
#define FILE_FLAG_OVERLAPPED     0x40000000

#include <fcntl.h>

// Pack fd into HANDLE (INVALID_HANDLE_VALUE == (void*)-1, fd==-1 maps naturally)
static inline HANDLE _fd_to_handle(int fd) { return (HANDLE)(intptr_t)fd; }
static inline int    _handle_to_fd(HANDLE h) { return (int)(intptr_t)h; }

static inline HANDLE CreateFile(LPCSTR lpFileName, DWORD dwDesiredAccess,
    DWORD /*dwShareMode*/, LPSECURITY_ATTRIBUTES /*lpSecurityAttributes*/,
    DWORD dwCreationDisposition, DWORD /*dwFlagsAndAttributes*/,
    HANDLE /*hTemplateFile*/)
{
    int flags = 0;
    bool canRead  = (dwDesiredAccess & GENERIC_READ)  != 0;
    bool canWrite = (dwDesiredAccess & GENERIC_WRITE) != 0;
    if (canRead && canWrite) flags = O_RDWR;
    else if (canWrite)       flags = O_WRONLY;
    else                     flags = O_RDONLY;

    switch (dwCreationDisposition) {
    case CREATE_NEW:       flags |= O_CREAT | O_EXCL;            break;
    case CREATE_ALWAYS:    flags |= O_CREAT | O_TRUNC;           break;
    case OPEN_EXISTING:    /* no extra flags */                   break;
    case TRUNCATE_EXISTING:flags |= O_TRUNC;                     break;
    default:               flags |= O_CREAT;                     break;
    }

    int fd = open(lpFileName, flags, 0666);
    return _fd_to_handle(fd);
}

static inline DWORD GetFileSize(HANDLE hFile, LPDWORD /*lpFileSizeHigh*/)
{
    int fd = _handle_to_fd(hFile);
    if (fd < 0) return 0xFFFFFFFF;
    struct stat st;
    if (fstat(fd, &st) != 0) return 0xFFFFFFFF;
    return (DWORD)st.st_size;
}

static inline DWORD GetFullPathName(LPCSTR lpFileName, DWORD nBufferLength,
    LPSTR lpBuffer, LPSTR* /*lpFilePart*/)
{
    if (lpBuffer && nBufferLength > 0)
    {
        strncpy(lpBuffer, lpFileName, nBufferLength - 1);
        lpBuffer[nBufferLength - 1] = '\0';
    }
    return lpFileName ? (DWORD)strlen(lpFileName) : 0;
}

static inline BOOL ReadFile(HANDLE hFile, LPVOID lpBuffer,
    DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead,
    LPOVERLAPPED /*lpOverlapped*/)
{
    int fd = _handle_to_fd(hFile);
    if (fd < 0) return FALSE;
    ssize_t n = read(fd, lpBuffer, nNumberOfBytesToRead);
    if (n < 0) { if (lpNumberOfBytesRead) *lpNumberOfBytesRead = 0; return FALSE; }
    if (lpNumberOfBytesRead) *lpNumberOfBytesRead = (DWORD)n;
    return TRUE;
}

static inline BOOL WriteFile(HANDLE hFile, LPCVOID lpBuffer,
    DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten,
    LPOVERLAPPED /*lpOverlapped*/)
{
    int fd = _handle_to_fd(hFile);
    if (fd < 0) return FALSE;
    ssize_t n = write(fd, lpBuffer, nNumberOfBytesToWrite);
    if (n < 0) { if (lpNumberOfBytesWritten) *lpNumberOfBytesWritten = 0; return FALSE; }
    if (lpNumberOfBytesWritten) *lpNumberOfBytesWritten = (DWORD)n;
    return TRUE;
}

// -----------------------------------------------------------------------
// Dialog / ComboBox / ListBox messages (stubs)
// -----------------------------------------------------------------------
#define CB_RESETCONTENT  0x014B
#define CB_ADDSTRING     0x0143
#define CB_SETCURSEL     0x014E
#define CB_GETCURSEL     0x0147
#define CB_ERR           (-1)
#define CB_OKAY          0
#define LB_ADDSTRING     0x0180
#define LB_RESETCONTENT  0x0184
#define LB_SETCURSEL     0x0186
#define LB_GETCURSEL     0x0188
#define LB_ERR           (-1)
#define BM_GETCHECK      0x00F0
#define BM_SETCHECK      0x00F1
#define BST_CHECKED      0x0001
#define BST_UNCHECKED    0x0000
#define EM_SETLIMITTEXT  0x00C5
#define WM_SETTEXT       0x000C
#define WM_GETTEXT       0x000D
#define SW_SHOW          5
#define SW_HIDE          0

static inline HWND GetDlgItem(HWND /*hDlg*/, int /*nIDDlgItem*/) { return NULL; }
static inline LRESULT SendDlgItemMessage(HWND /*hDlg*/, int /*nIDDlgItem*/, UINT /*Msg*/, WPARAM /*wParam*/, LPARAM /*lParam*/) { return 0; }
static inline BOOL CheckDlgButton(HWND /*hDlg*/, int /*nIDButton*/, UINT /*uCheck*/) { return FALSE; }
static inline UINT IsDlgButtonChecked(HWND /*hDlg*/, int /*nIDButton*/) { return BST_UNCHECKED; }
static inline BOOL SetDlgItemText(HWND /*hDlg*/, int /*nIDDlgItem*/, LPCSTR /*lpString*/) { return FALSE; }
static inline UINT GetDlgItemText(HWND /*hDlg*/, int /*nIDDlgItem*/, LPSTR /*lpString*/, int /*nMaxCount*/) { return 0; }
static inline BOOL GetDlgItemInt(HWND /*hDlg*/, int /*nIDDlgItem*/, BOOL* /*lpTranslated*/, BOOL /*bSigned*/) { return 0; }
static inline BOOL SetDlgItemInt(HWND /*hDlg*/, int /*nIDDlgItem*/, UINT /*uValue*/, BOOL /*bSigned*/) { return FALSE; }
static inline BOOL ShowWindow(HWND /*hWnd*/, int /*nCmdShow*/) { return FALSE; }
static inline BOOL EnableWindow(HWND /*hWnd*/, BOOL /*bEnable*/) { return FALSE; }
static inline BOOL IsWindowEnabled(HWND /*hWnd*/) { return FALSE; }
static inline BOOL SetFocus(HWND /*hWnd*/) { return FALSE; }
static inline int GetScrollPos(HWND /*hWnd*/, int /*nBar*/) { return 0; }
static inline BOOL SetScrollRange(HWND /*hWnd*/, int /*nBar*/, int /*nMinPos*/, int /*nMaxPos*/, BOOL /*bRedraw*/) { return FALSE; }
static inline BOOL SetScrollPos(HWND /*hWnd*/, int /*nBar*/, int /*nPos*/, BOOL /*bRedraw*/) { return FALSE; }

// -----------------------------------------------------------------------
// KF_EXTENDED (keyboard)
// -----------------------------------------------------------------------
#define KF_EXTENDED 0x0100
#define KF_ALTDOWN  0x2000
#define KF_REPEAT   0x4000
#define KF_UP       0x8000

// -----------------------------------------------------------------------
// Winsock stubs (for SerialComms.h)
// -----------------------------------------------------------------------
#ifndef _SYS_SOCKET_H
#include <sys/socket.h>
#endif
#ifndef _NETINET_IN_H
#include <netinet/in.h>
#endif
#ifndef SOCKET
typedef int SOCKET;
#define INVALID_SOCKET ((SOCKET)-1)
#define SOCKET_ERROR   (-1)
#endif

typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr*   LPSOCKADDR;
static inline int closesocket(SOCKET s) { return close(s); }

typedef struct tagWSADATA { WORD wVersion; char szDescription[257]; char szSystemStatus[129]; } WSADATA;
static inline int WSAStartup(WORD /*wVersionRequired*/, WSADATA* /*lpWSAData*/) { return 0; }
static inline int WSACleanup(void) { return 0; }
static inline int WSAGetLastError(void) { return 0; }

// -----------------------------------------------------------------------
// Serial port (COM) stubs
// -----------------------------------------------------------------------
#define DTR_CONTROL_DISABLE   0
#define DTR_CONTROL_ENABLE    1
#define DTR_CONTROL_HANDSHAKE 2
#define RTS_CONTROL_DISABLE   0
#define RTS_CONTROL_ENABLE    1
#define RTS_CONTROL_HANDSHAKE 2
#define RTS_CONTROL_TOGGLE    3

typedef struct _DCB {
    DWORD DCBlength;
    DWORD BaudRate;
    DWORD fBinary:1;
    DWORD fParity:1;
    DWORD fOutxCtsFlow:1;
    DWORD fOutxDsrFlow:1;
    DWORD fDtrControl:2;
    DWORD fDsrSensitivity:1;
    DWORD fTXContinueOnXoff:1;
    DWORD fOutX:1;
    DWORD fInX:1;
    DWORD fErrorChar:1;
    DWORD fNull:1;
    DWORD fRtsControl:2;
    DWORD fAbortOnError:1;
    DWORD fDummy2:17;
    WORD  wReserved;
    WORD  XonLim;
    WORD  XoffLim;
    BYTE  ByteSize;
    BYTE  Parity;
    BYTE  StopBits;
    char  XonChar;
    char  XoffChar;
    char  ErrorChar;
    char  EofChar;
    char  EvtChar;
    WORD  wReserved1;
} DCB;

typedef struct _COMMTIMEOUTS {
    DWORD ReadIntervalTimeout;
    DWORD ReadTotalTimeoutMultiplier;
    DWORD ReadTotalTimeoutConstant;
    DWORD WriteTotalTimeoutMultiplier;
    DWORD WriteTotalTimeoutConstant;
} COMMTIMEOUTS;

typedef struct _COMSTAT {
    DWORD fCtsHold:1;
    DWORD fDsrHold:1;
    DWORD fRlsdHold:1;
    DWORD fXoffHold:1;
    DWORD fXoffSent:1;
    DWORD fEof:1;
    DWORD fTxim:1;
    DWORD fReserved:25;
    DWORD cbInQue;
    DWORD cbOutQue;
} COMSTAT;

static inline BOOL GetCommState(HANDLE /*hFile*/, DCB* /*lpDCB*/) { return FALSE; }
static inline BOOL SetCommState(HANDLE /*hFile*/, DCB* /*lpDCB*/) { return FALSE; }
static inline BOOL SetCommTimeouts(HANDLE /*hFile*/, COMMTIMEOUTS* /*lpCommTimeouts*/) { return FALSE; }
static inline BOOL ClearCommError(HANDLE /*hFile*/, LPDWORD /*lpErrors*/, COMSTAT* /*lpStat*/) { return FALSE; }
static inline BOOL PurgeComm(HANDLE /*hFile*/, DWORD /*dwFlags*/) { return FALSE; }
static inline BOOL EscapeCommFunction(HANDLE /*hFile*/, DWORD /*dwFunc*/) { return FALSE; }
static inline BOOL SetupComm(HANDLE /*hFile*/, DWORD /*dwInQueue*/, DWORD /*dwOutQueue*/) { return FALSE; }

// Modem status flags
#define MS_CTS_ON   0x0010
#define MS_DSR_ON   0x0020
#define MS_RING_ON  0x0040
#define MS_RLSD_ON  0x0080

// Comm event flags
#define EV_RXCHAR   0x0001
#define EV_RXFLAG   0x0002
#define EV_TXEMPTY  0x0004
#define EV_CTS      0x0008
#define EV_DSR      0x0010
#define EV_RLSD     0x0020
#define EV_BREAK    0x0040
#define EV_ERR      0x0080
#define EV_RING     0x0100

// Winsock async event flags
#define FD_READ     0x01
#define FD_WRITE    0x02
#define FD_ACCEPT   0x08
#define FD_CONNECT  0x10
#define FD_CLOSE    0x20

static inline int WSAAsyncSelect(SOCKET /*s*/, HWND /*hWnd*/, unsigned int /*wMsg*/, long /*lEvent*/) { return 0; }
static inline BOOL GetCommModemStatus(HANDLE /*hFile*/, LPDWORD /*lpModemStat*/) { return FALSE; }

#define MAXDWORD 0xFFFFFFFF
#define INFINITE 0xFFFFFFFF
#define WAIT_OBJECT_0 0x00000000
#define STILL_ACTIVE  0x00000103
#define ERROR_IO_PENDING 997

// Comm mask / wait stubs
static inline BOOL SetCommMask(HANDLE /*hFile*/, DWORD /*dwEvtMask*/) { return FALSE; }
static inline BOOL WaitCommEvent(HANDLE /*hFile*/, LPDWORD /*lpEvtMask*/, LPOVERLAPPED /*lpOverlapped*/) { return FALSE; }
static inline DWORD WaitForMultipleObjects(DWORD /*nCount*/, const HANDLE* /*lpHandles*/, BOOL /*bWaitAll*/, DWORD /*dwMilliseconds*/) { return WAIT_OBJECT_0; }
static inline BOOL GetExitCodeThread(HANDLE /*hThread*/, LPDWORD /*lpExitCode*/) { return FALSE; }
static inline BOOL SetThreadPriority(HANDLE /*hThread*/, int /*nPriority*/) { return TRUE; }
static inline void Sleep(DWORD dwMilliseconds) { usleep(dwMilliseconds * 1000); }

#define THREAD_PRIORITY_TIME_CRITICAL 15
#define THREAD_PRIORITY_NORMAL        0

// Comm error codes
#define CE_RXOVER  0x0001
#define CE_OVERRUN 0x0002
#define CE_RXPARITY 0x0004
#define CE_FRAME   0x0008
#define CE_BREAK   0x0010
#define CE_TXFULL  0x0100

#define PURGE_TXABORT 0x0001
#define PURGE_RXABORT 0x0002
#define PURGE_TXCLEAR 0x0004
#define PURGE_RXCLEAR 0x0008
#define CLRDTR        6
#define CLRRTS        4
#define SETDTR        5
#define SETRTS        3

// -----------------------------------------------------------------------
// Serial port baud rate constants (stubs for SerialComms.h)
// -----------------------------------------------------------------------
#define CBR_110     110
#define CBR_300     300
#define CBR_600     600
#define CBR_1200    1200
#define CBR_2400    2400
#define CBR_4800    4800
#define CBR_9600    9600
#define CBR_14400   14400
#define CBR_19200   19200
#define CBR_38400   38400
#define CBR_56000   56000
#define CBR_57600   57600
#define CBR_115200  115200
#define CBR_128000  128000
#define CBR_256000  256000

// NOPARITY, ONESTOPBIT etc.
#define NOPARITY    0
#define ODDPARITY   1
#define EVENPARITY  2
#define MARKPARITY  3
#define SPACEPARITY 4
#define ONESTOPBIT  0
#define ONE5STOPBITS 1
#define TWOSTOPBITS 2

// -----------------------------------------------------------------------
// Thread function type
// -----------------------------------------------------------------------
typedef DWORD (*LPTHREAD_START_ROUTINE)(LPVOID lpParameter);

static inline HANDLE CreateThread(LPSECURITY_ATTRIBUTES /*lpThreadAttributes*/,
    SIZE_T /*dwStackSize*/, LPTHREAD_START_ROUTINE /*lpStartAddress*/,
    LPVOID /*lpParameter*/, DWORD /*dwCreationFlags*/, LPDWORD /*lpThreadId*/)
{
    return INVALID_HANDLE_VALUE; // stub - no threading in libretro port
}

static inline DWORD WaitForSingleObject(HANDLE /*hHandle*/, DWORD /*dwMilliseconds*/)
{
    return 0; // WAIT_OBJECT_0
}

static inline BOOL SetEvent(HANDLE /*hEvent*/) { return TRUE; }
static inline BOOL ResetEvent(HANDLE /*hEvent*/) { return TRUE; }
static inline BOOL CloseHandle(HANDLE hObject)
{
    int fd = _handle_to_fd(hObject);
    if (fd >= 0) close(fd);
    return TRUE;
}
static inline HANDLE CreateEvent(LPSECURITY_ATTRIBUTES /*lpEventAttributes*/,
    BOOL /*bManualReset*/, BOOL /*bInitialState*/, LPCSTR /*lpName*/)
{
    return INVALID_HANDLE_VALUE;
}

// -----------------------------------------------------------------------
// JOYINFOEX stub (for SNESMAX.h)
// -----------------------------------------------------------------------
typedef struct joyinfoex_tag {
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

#define JOY_RETURNX        0x00000001L
#define JOY_RETURNY        0x00000002L
#define JOY_RETURNZ        0x00000004L
#define JOY_RETURNR        0x00000008L
#define JOY_RETURNU        0x00000010L
#define JOY_RETURNV        0x00000020L
#define JOY_RETURNPOV      0x00000040L
#define JOY_RETURNBUTTONS  0x00000080L
#define JOY_RETURNRAWDATA  0x00000100L
#define JOY_RETURNPOVCTS   0x00000200L
#define JOY_RETURNCENTERED 0x00000400L
#define JOY_USEDEADZONE    0x00000800L
#define JOY_RETURNALL      (JOY_RETURNX | JOY_RETURNY | JOY_RETURNZ | JOY_RETURNR | JOY_RETURNU | JOY_RETURNV | JOY_RETURNPOV | JOY_RETURNBUTTONS)

static inline DWORD joyGetPosEx(UINT /*uJoyID*/, JOYINFOEX* /*pji*/) { return 0xFFFF; } // JOYERR_NOERROR would be 0

#define JOYERR_NOERROR 0

// JOYINFO (older, simpler joystick info)
typedef struct tagJOYINFO {
    UINT wXpos;
    UINT wYpos;
    UINT wZpos;
    UINT wButtons;
} JOYINFO;

#define JOYSTICKID1  0
#define JOYSTICKID2  1

#define JOY_BUTTON1  0x0001
#define JOY_BUTTON2  0x0002
#define JOY_BUTTON3  0x0004
#define JOY_BUTTON4  0x0008

#define JOY_RETURNZ  0x00000004L
#define JOY_RETURNR  0x00000008L

typedef DWORD MMRESULT;

static inline MMRESULT joyGetPos(UINT /*uJoyID*/, JOYINFO* /*pji*/) { return 0xFFFF; }

// JOYCAPS stub
typedef struct tagJOYCAPS {
    WORD wMid;
    WORD wPid;
    char szPname[32];
    UINT wXmin, wXmax;
    UINT wYmin, wYmax;
    UINT wZmin, wZmax;
    UINT wNumButtons;
    UINT wPeriodMin, wPeriodMax;
    UINT wRmin, wRmax;
    UINT wUmin, wUmax;
    UINT wVmin, wVmax;
    UINT wCaps;
    UINT wMaxAxes;
    UINT wNumAxes;
    UINT wMaxButtons;
    char szRegKey[32];
    char szOEMVxD[260];
} JOYCAPS;

static inline UINT joyGetNumDevs(void) { return 0; }
static inline MMRESULT joyGetDevCaps(UINT /*uJoyID*/, JOYCAPS* /*pjc*/, UINT /*cbjc*/) { return 0xFFFF; }

// GetAsyncKeyState stub - always returns not-pressed
static inline short GetAsyncKeyState(int /*vKey*/) { return 0; }
// GetKeyState stub
static inline short GetKeyState(int /*nVirtKey*/) { return 0; }

// Additional VK codes
#define VK_CLEAR    0x0C
#define VK_DECIMAL  0x6E
#define VK_DIVIDE   0x6F
#define VK_MULTIPLY 0x6A
#define VK_SUBTRACT 0x6D
#define VK_ADD      0x6B
#define VK_LSHIFT   0xA0
#define VK_RSHIFT   0xA1
#define VK_LCONTROL 0xA2
#define VK_RCONTROL 0xA3
#define VK_LMENU    0xA4
#define VK_RMENU    0xA5

// Additional VK codes
#define VK_CANCEL   0x03
#define VK_SCROLL   0x91
#define VK_OEM_1    0xBA
#define VK_OEM_2    0xBF
#define VK_OEM_3    0xC0
#define VK_OEM_4    0xDB
#define VK_OEM_5    0xDC
#define VK_OEM_6    0xDD
#define VK_OEM_7    0xDE
#define VK_OEM_8    0xDF
#define VK_OEM_PLUS  0xBB
#define VK_OEM_COMMA 0xBC
#define VK_OEM_MINUS 0xBD
#define VK_OEM_PERIOD 0xBE
#define VK_OEM_102  0xE2

// PostMessage / SendMessage stubs
static inline BOOL PostMessage(HWND /*hWnd*/, UINT /*Msg*/, WPARAM /*wParam*/, LPARAM /*lParam*/) { return TRUE; }
static inline LRESULT SendMessage(HWND /*hWnd*/, UINT /*Msg*/, WPARAM /*wParam*/, LPARAM /*lParam*/) { return 0; }

// Clipboard stubs
#define CF_TEXT  1
static inline BOOL IsClipboardFormatAvailable(UINT /*format*/) { return FALSE; }
static inline BOOL OpenClipboard(HWND /*hWndNewOwner*/) { return FALSE; }
static inline BOOL CloseClipboard(void) { return TRUE; }
static inline HANDLE GetClipboardData(UINT /*uFormat*/) { return NULL; }
static inline LPVOID GlobalLock(HANDLE /*hMem*/) { return NULL; }
static inline BOOL GlobalUnlock(HANDLE /*hMem*/) { return TRUE; }
static inline HGLOBAL GlobalAlloc(UINT /*uFlags*/, SIZE_T /*dwBytes*/) { return NULL; }
static inline HGLOBAL GlobalFree(HGLOBAL /*hMem*/) { return NULL; }
static inline BOOL SetClipboardData(UINT /*uFormat*/, HANDLE /*hMem*/) { return FALSE; }
static inline BOOL EmptyClipboard(void) { return FALSE; }
#define GMEM_MOVEABLE  0x0002

// ExitProcess stub
static inline void ExitProcess(UINT /*uExitCode*/) { exit(1); }

// SYSTEMTIME / GetLocalTime
typedef struct _SYSTEMTIME {
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
} SYSTEMTIME;

static inline void GetLocalTime(SYSTEMTIME* st)
{
    time_t t = time(NULL);
    struct tm* tm = localtime(&t);
    if (st && tm) {
        st->wYear         = (WORD)(tm->tm_year + 1900);
        st->wMonth        = (WORD)(tm->tm_mon + 1);
        st->wDayOfWeek    = (WORD)tm->tm_wday;
        st->wDay          = (WORD)tm->tm_mday;
        st->wHour         = (WORD)tm->tm_hour;
        st->wMinute       = (WORD)tm->tm_min;
        st->wSecond       = (WORD)tm->tm_sec;
        st->wMilliseconds = 0;
    }
}

// timeGetTime - millisecond timer
static inline DWORD timeGetTime(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (DWORD)(ts.tv_sec * 1000ULL + ts.tv_nsec / 1000000ULL);
}

// MessageBox stub
static inline int MessageBox(HWND /*hWnd*/, LPCSTR lpText, LPCSTR lpCaption, UINT /*uType*/)
{
    fprintf(stderr, "[AppleWin] %s: %s\n", lpCaption ? lpCaption : "", lpText ? lpText : "");
    return IDOK;
}
static inline int MessageBoxA(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType)
{
    return MessageBox(hWnd, lpText, lpCaption, uType);
}

// min/max using std
#include <algorithm>
using std::min;
using std::max;
