# apple2-libretro

Apple II 에뮬레이터 libretro 코어. [AppleWin](https://github.com/AppleWin/AppleWin) 1.31 소스를 기반으로 하며, Linux 포팅 참고는 [linapple](https://github.com/linappleii/linapple)을 따랐습니다.

## 지원 기기

| 기기 | 기본값 |
|------|--------|
| Apple II | |
| Apple II+ | |
| Apple IIe | |
| Apple IIe Enhanced | ✓ (현재 기본값) |

## 지원 디스크 포맷

`.dsk` `.do` `.po` `.nib` `.woz` `.2mg` `.img` `.gz` `.hdv`

---

## 빌드

### 의존성

```bash
sudo apt install build-essential libz-dev libpcap-dev
```

### 빌드

```bash
make -f Makefile.libretro
# 결과: apple2_libretro.so
```

플랫폼 지정 (Raspberry Pi 등):

```bash
make -f Makefile.libretro platform=rpi3
```

---

## 필요한 ROM 파일

libretro 프론트엔드의 **system 디렉토리** 아래 `apple2/` 폴더에 ROM 파일을 배치해야 합니다.

```
<system_dir>/
└── apple2/
    ├── Apple2e_Enhanced.rom          ← 필수 (Apple IIe Enhanced 메인 ROM, 16KB)
    ├── Apple2e_Enhanced_Video.rom    ← 필수 (비디오 ROM, 2KB)
    ├── DISK2.rom                     ← 필수 (Disk II 펌웨어, 256B)
    ├── Apple2e.rom                   ← Apple IIe 사용 시
    ├── Apple2_Plus.rom               ← Apple II+ 사용 시
    ├── Apple2.rom                    ← Apple II 사용 시
    ├── Apple2_Video.rom              ← Apple II 비디오 ROM
    ├── DISK2-13sector.rom            ← 13섹터 디스크 사용 시
    ├── Hddrvr.bin                    ← 하드디스크 드라이버
    ├── Hddrvr-v2.bin                 ← 하드디스크 드라이버 v2
    ├── HDC-SmartPort.bin             ← SmartPort HDD 컨트롤러
    ├── Mockingboard-D.rom            ← 목킹보드 사운드 카드
    ├── SSC.rom                       ← Super Serial Card ROM
    ├── Parallel.rom                  ← 병렬 프린터 카드 ROM
    ├── MouseInterface.rom            ← 마우스 인터페이스 카드 ROM
    └── BootSector/
        └── bootsector.bin            ← 부트섹터
```

> ROM 파일은 저작권상 별도로 구해야 합니다.
> AppleWin 윈도우 버전에서 추출하거나 MAME ROM 세트에서 구할 수 있습니다.

---

## 사용법 (RetroArch 기준)

```bash
retroarch -L apple2_libretro.so game.dsk
```

---

## 프로젝트 구조

```
apple2-libretro/
├── libretro.cpp                  # libretro API 진입점
├── Makefile.libretro             # 빌드 스크립트
├── Makefile.common               # 소스 목록 및 인클루드 경로
├── link.T                        # libretro 심볼 export 스크립트
├── src/
│   ├── libretro/                 # libretro 어댑터 레이어
│   │   ├── libretro_bridge.cpp   # Apple2Core 네임스페이스 (핵심 연결부)
│   │   ├── libretro_bridge.h
│   │   ├── LibretroFrame.cpp     # FrameBase 구현, ROM 로더, SoundBuffer 스텁
│   │   ├── LibretroFrame.h
│   │   ├── LibretroVideo.cpp     # GetVideo/GetFrame/GetPropertySheet 싱글톤
│   │   ├── LibretroVideo.h
│   │   ├── LibretroPropertySheet.cpp
│   │   ├── LibretroPropertySheet.h
│   │   ├── libretro_audio.cpp    # 오디오 출력 (미구현)
│   │   └── libretro_input.cpp    # 입력 처리 (미구현)
│   └── apple2core/               # AppleWin 1.31 에뮬레이터 코어
│       ├── CPU.cpp / CPU.h       # 6502/65C02 CPU
│       ├── Memory.cpp / Memory.h # 메모리 맵, ROM 로딩
│       ├── NTSC.cpp / NTSC.h     # NTSC 비디오 스캔라인 렌더러
│       ├── Video.cpp / Video.h   # 비디오 모드 (텍스트/LR/HR/DHR)
│       ├── Disk.cpp / Disk.h     # Disk II 인터페이스 카드
│       ├── Speaker.cpp / Speaker.h  # 스피커 비프음
│       ├── Mockingboard.cpp      # 목킹보드 (AY-3-8910 2개)
│       ├── Harddisk.cpp          # 하드디스크 (ProDOS)
│       ├── Keyboard.cpp          # 키보드
│       ├── Joystick.cpp          # 조이스틱 / 패들
│       └── ...                   # 기타 카드 및 유틸리티
├── libretro-common/              # libretro 공통 헤더
├── AppleWin-1.31.0.0/            # AppleWin 원본 소스 (참고용)
│   ├── libyaml/                  # YAML 파서 (SaveState에 사용)
│   └── minizip/                  # ZIP 압축 (디스크 이미지)
└── linapple-1.0.2/               # linapple 소스 (Linux 포팅 참고용)
```

---

## 구현 현황

| 단계 | 내용 | 상태 |
|------|------|------|
| Phase 1 | 빌드 시스템 구성, Linux 스텁 추가 | ✅ 완료 |
| Phase 2 | 에뮬레이터 코어 연결 (CPU/메모리/비디오/디스크) | ✅ 완료 |
| Phase 3 | 키보드 입력 | 🔲 미구현 |
| Phase 4 | 조이스틱/패들 입력 | 🔲 미구현 |
| Phase 5 | 오디오 출력 (스피커 / 목킹보드) | 🔲 미구현 |
| Phase 6 | Save State | 🔲 미구현 |

---

## 참고 프로젝트

- [AppleWin](https://github.com/AppleWin/AppleWin) — Windows용 Apple II 에뮬레이터 (에뮬레이션 코어 출처)
- [linapple](https://github.com/linappleii/linapple) — AppleWin Linux 포팅 (SDL 기반, 초기화/실행 흐름 참고)
- [libretro-bluemsx](https://github.com/libretro/bluemsx-libretro) — libretro 코어 구조 참고
