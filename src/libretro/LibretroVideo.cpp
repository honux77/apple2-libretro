// LibretroVideo.cpp - implements Interface.h singletons for libretro port

#include "StdAfx.h"
#include "LibretroVideo.h"
#include "Interface.h"

// Concrete singletons
static Video                  g_video;
static LibretroPropertySheet  g_propertySheet;
static LibretroFrame          g_frame;

// Interface.h implementations
Video& GetVideo(void)
{
    return g_video;
}

IPropertySheet& GetPropertySheet(void)
{
    return g_propertySheet;
}

FrameBase& GetFrame(void)
{
    return g_frame;
}

LibretroFrame& GetLibretroFrame()
{
    return g_frame;
}

void LibretroInterface_Init(const std::string& systemDir)
{
    g_frame.SetSystemDir(systemDir);
}

void LibretroInterface_Destroy()
{
    // nothing to do here
}
