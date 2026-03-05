#pragma once
#include <stdint.h>

namespace Apple2Core {
    bool Init(const char* system_dir);
    void Destroy();
    bool LoadDisk(const char* path);
    void RunFrame();
    const uint32_t* GetFramebuffer();
    unsigned GetWidth();
    unsigned GetHeight();
}
