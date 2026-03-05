#pragma once
#include "../../libretro-common/include/libretro.h"
#include <stddef.h>

namespace LibretroAudio {
    void Init();
    void Destroy();
    size_t Flush(retro_audio_sample_batch_t batch_cb);
}
