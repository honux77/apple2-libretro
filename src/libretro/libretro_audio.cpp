// Audio bridge stub - Phase 5 implementation
#include "libretro_audio.h"

namespace LibretroAudio {
    void Init() {}
    void Destroy() {}
    size_t Flush(retro_audio_sample_batch_t batch_cb) { (void)batch_cb; return 0; }
}
