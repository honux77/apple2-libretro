# blueMSX-libretro

A libretro port of [blueMSX](http://www.bluemsx.com/), a cycle-accurate MSX emulator.

## Supported Systems

- MSX / MSX2 / MSX2+ / MSXturboR
- SEGA SG-1000 / SC-3000 / SF-7000
- SVI Spectravideo SVI-318 / SVI-328 / SVI-328 MK2
- ColecoVision

## Supported File Extensions

| Extension | Description |
|-----------|-------------|
| `.rom` `.ri` `.mx1` `.mx2` | ROM cartridge images |
| `.dsk` | Floppy disk images |
| `.col` | ColecoVision ROM images |
| `.sg` `.sc` `.sf` | SEGA ROM images |
| `.cas` | Cassette tape images |
| `.m3u` | Multi-disk playlist |

## BIOS / System Files

This core requires the **Machines/** and **Databases/** folders from the standalone blueMSX application. Place them inside your frontend's system directory under `system/bluemsx/`.

```
system/
└── bluemsx/
    ├── Machines/
    │   ├── Shared Roms/
    │   │   ├── MSX.rom
    │   │   ├── MSX2.rom
    │   │   ├── MSX2EXT.rom
    │   │   ├── MSX2P.rom
    │   │   ├── MSX2PEXT.rom
    │   │   ├── MSXTR.ROM
    │   │   ├── SUNRISEIDE.rom      ← Required for Sunrise IDE HDD support
    │   │   └── (other shared ROMs)
    │   ├── MSX/
    │   ├── MSX2/
    │   ├── MSX2+/
    │   ├── MSXturboR/
    │   └── (other machine folders)
    └── Databases/
        └── (database files)
```

You can obtain these files from the [official blueMSX Windows release](http://www.bluemsx.com/).

## Sunrise IDE / HDD Support

The Sunrise IDE interface allows loading hard disk images (`.dsk`) on MSX2 or later machines.

### Required ROM

`Machines/Shared Roms/SUNRISEIDE.rom` must be present in your system directory.

### Setup

1. Enable **SunriseIDE Enable (Restart)** in the core options.
2. Load a hard disk image (`.dsk` format).
3. The SUNRISEIDE.rom cartridge is automatically inserted into slot 1.
4. The disk image is mounted as an IDE hard drive (not a floppy drive).

> **Note:** Sunrise IDE support requires MSX2 or later. A restart is required when toggling this option.

## Machine Type ROM Files

When selecting a specific machine type, the corresponding ROM files must be present
in your system directory under `Machines/<machine-name>/`.

| Machine | Required ROM files | Status |
|---------|-------------------|--------|
| MSX / MSX2 / MSX2+ / MSXturboR | See `Machines/Shared Roms/` | Bundled with blueMSX |
| MSXturboR - Panasonic FS-A1GT | See `Machines/MSXturboR - Panasonic FS-A1GT/` | Bundled with blueMSX |
| MSX2+ - Panasonic FS-A1WSX | See `Machines/MSX2+ - Panasonic FS-A1WSX/` | Bundled with blueMSX |
| MSX - Daewoo DPC-100 | `100bios.rom`, `100han.rom` | Available in openMSX system ROMs |
| MSX - Daewoo DPC-180 | `180bios.rom`, `180han.rom` | Available in openMSX system ROMs |
| MSX - Daewoo DPC-200 | `200bios.rom`, `200han.rom` | Available in openMSX system ROMs |
| MSX - Daewoo DPC-200E | `dpc200ebios.rom` | ROMs not widely available |
| MSX2 - Daewoo CPC-300 | `300bios.rom`, `300han.rom`, `300ext.rom` | Available in openMSX system ROMs |
| MSX2 - Daewoo CPC-300E | `300ebios.rom`, `300ehan.rom`, `300eext.rom` | ROMs not widely available |
| MSX2 - Daewoo CPC-400 | `400kfn.rom`, `400bios.rom`, `400han.rom`, `400ext.rom`, `400disk.rom` | ROMs not widely available |
| MSX2 - Daewoo CPC-400S | `400skfn.rom`, `400sbios.rom`, `400shan.rom`, `400sext.rom`, `400sdisk.rom` | Available in openMSX system ROMs |
| MSX2 - Daewoo CPC-61 Zemmix Super V | `61bios.rom`, `61han.rom`, `61sub.rom` | Available in openMSX system ROMs |

> **Note:** If a machine's ROM files are missing, the core will fail to load with an error message
> instead of freezing. Use `Auto` or a machine with available ROMs.

## Core Options

| Option | Description | Default |
|--------|-------------|---------|
| **Machine Type (Restart)** | Select the emulated machine. `Auto` detects from the loaded content. | Auto |
| **Crop Overscan** | Crops overscan borders. `MSX2` crops only on MSX2 and later. | disabled |
| **VDP Sync Type (Restart)** | Sets VDP frequency to match region (50Hz / 60Hz). | Auto |
| **No Sprite Limit** | Removes the 4-sprite-per-line hardware limit to reduce flicker. | OFF |
| **Sound YM2413 Enable (Restart)** | Enable YM2413 (MSX-MUSIC / OPLL) FM sound chip. | enabled |
| **Cart Mapper Type (Restart)** | Override automatic ROM mapper detection. | Auto |
| **Additional MSX Sound Cartridge (Restart)** | Insert an additional SCC or SCC+ sound cartridge. | disabled |
| **Auto Rewind Cassette** | Automatically rewind cassette tape on load. | ON |
| **I/O Sound Enable** | Enable floppy disk access sound effects. | disabled |
| **SunriseIDE Enable (Restart)** | Enable Sunrise IDE hard drive support. Disk images are loaded as IDE HDDs and SUNRISEIDE.rom is automatically inserted. | disabled |
| **Colecovision Keyboard Mapping** | Map keyboard keys to ColecoVision controller buttons. | disabled |
| **Patch Colecovision ROM Fast Boot** | Patch NTSC ColecoVision ROM for fast boot (~3.3s). | disabled |
| **Use F12 to hard reset** | Use F12 to hard reset and restart emulation like blueMSX standalone. | disabled |

## Multi-Disk Games

Use `.m3u` playlists to load multi-disk games. Create a text file with one disk image path per line:

```
Disk1.dsk
Disk2.dsk
Disk3.dsk
```

Then load the `.m3u` file in your frontend. Use the disk control interface to swap disks during gameplay.

## Building

```bash
make -f Makefile.libretro
```

For Android:

```bash
ndk-build
```

## Sources

- blueMSX upstream: https://sourceforge.net/p/bluemsx/code/HEAD/tree/trunk/blueMSX/
- SVN: `svn://svn.code.sf.net/p/bluemsx/code/trunk`
