#pragma once

// Provides the Interface.h required implementations:
//   Video& GetVideo()
//   IPropertySheet& GetPropertySheet()
//   FrameBase& GetFrame()

#include "StdAfx.h"
#include "LibretroFrame.h"
#include "LibretroPropertySheet.h"

void LibretroInterface_Init(const std::string& systemDir);
void LibretroInterface_Destroy();

LibretroFrame& GetLibretroFrame();
