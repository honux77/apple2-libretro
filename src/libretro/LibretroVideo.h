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

// Drain audio from all sound buffers (Mockingboard etc.) into the libretro audio callback
void LibretroFrame_DrainAllAudio(retro_audio_sample_batch_t cb);
