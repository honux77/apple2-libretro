/*****************************************************************************
** $Source: /cygdrive/d/Private/_SVNROOT/bluemsx/blueMSX/Src/Memory/RomLoader.c,v $
**
** $Revision: 1.6 $
**
** $Date: 2008-03-30 18:38:42 $
**
** More info: http://www.bluemsx.com
**
** Copyright (C) 2003-2006 Daniel Vik
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
** 
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
**
******************************************************************************
*/
#include "RomLoader.h"
#include "ziphelper.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#ifdef __linux__
#include <dirent.h>
#include <strings.h>
#endif
#ifdef __LIBRETRO__
#include <libretro.h>
extern retro_log_printf_t log_cb;
#endif

// PacketFileSystem.h Need to be included after all other includes
#include "PacketFileSystem.h"

#ifdef __linux__
/* Resolve a path case-insensitively on Linux, component by component.
   Returns 1 on success (resolved is populated), 0 on failure. */
static int resolveCaseInsensitivePath(const char* path, char* resolved, size_t resolvedSize)
{
    char component[256];
    const char* src;
    char* slash;
    DIR* dp;
    struct dirent* de;
    size_t compLen;
    size_t resolvedLen;
    int found;

    if (!path || !path[0]) return 0;

    /* Start resolved path */
    if (path[0] == '/') {
        resolved[0] = '/';
        resolved[1] = '\0';
        src = path + 1;
    } else {
        resolved[0] = '\0';
        src = path;
    }

    while (*src) {
        /* Extract next component */
        slash = strchr(src, '/');
        if (slash) {
            compLen = (size_t)(slash - src);
        } else {
            compLen = strlen(src);
        }

        if (compLen == 0) { src++; continue; }
        if (compLen >= sizeof(component)) compLen = sizeof(component) - 1;

        strncpy(component, src, compLen);
        component[compLen] = '\0';
        src += compLen;
        if (*src == '/') src++;

        /* Search for component case-insensitively in current resolved dir */
        resolvedLen = strlen(resolved);
        dp = opendir(resolvedLen > 0 ? resolved : ".");
        found = 0;
        if (dp) {
            while ((de = readdir(dp)) != NULL) {
                if (strcasecmp(de->d_name, component) == 0) {
                    if (resolvedLen > 0 && resolved[resolvedLen - 1] != '/') {
                        strncat(resolved, "/", resolvedSize - resolvedLen - 1);
                        resolvedLen++;
                    }
                    strncat(resolved, de->d_name, resolvedSize - resolvedLen - 1);
                    found = 1;
                    break;
                }
            }
            closedir(dp);
        }

        if (!found) {
            /* Not found - append as-is and continue (may fail at fopen) */
            resolvedLen = strlen(resolved);
            if (resolvedLen > 0 && resolved[resolvedLen - 1] != '/') {
                strncat(resolved, "/", resolvedSize - resolvedLen - 1);
                resolvedLen++;
            }
            strncat(resolved, component, resolvedSize - resolvedLen - 1);
        }
    }

    return resolved[0] != '\0';
}

static FILE* fopenCaseInsensitive(const char* filename, const char* mode)
{
    FILE* file;
    char resolved[1280];

    file = fopen(filename, mode);
    if (file) return file;

    if (resolveCaseInsensitivePath(filename, resolved, sizeof(resolved))) {
        if (strcmp(resolved, filename) != 0)
            return fopen(resolved, mode);
    }

    return NULL;
}
#endif

UInt8* romLoad(const char *fileName, const char *fileInZipFile, int* size)
{
    UInt8* buf = NULL;
    FILE *file;

    if (!fileName || strlen(fileName) == 0)
        goto error;

    if (fileInZipFile && strlen(fileInZipFile) == 0)
        fileInZipFile = NULL;

    if (fileInZipFile)
    {
        buf = (UInt8*)zipLoadFile(fileName, fileInZipFile, size);
        if (!buf)
           goto error;
        return buf;
    }

#ifdef __linux__
    file = fopenCaseInsensitive(fileName, "rb");
#else
    file = fopen(fileName, "rb");
#endif
    if (!file)
        goto error;

    fseek(file, 0, SEEK_END);
    *size = ftell(file);
    if (*size == 0) {
        fclose(file);
        return (UInt8*)malloc(1);
    }

    fseek(file, 0, SEEK_SET);

    buf = (UInt8*)malloc(*size);
    
    *size = fread(buf, 1, *size, file);
    fclose(file);

    return buf;

error:
#ifdef __LIBRETRO__
    if (fileName && fileName[0] && log_cb)
        log_cb(RETRO_LOG_WARN, "[RomLoader] Failed to load ROM: %s\n", fileName);
#else
    if (fileName && fileName[0])
        fflush(stdout);
#endif
    return NULL;
}
