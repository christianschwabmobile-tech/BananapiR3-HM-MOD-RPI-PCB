# Tested configurations

This document records OpenWrt and project combinations that have been
successfully built and tested.

## OpenWrt 25.12.5 / Banana Pi BPI-R3

### Platform

- Board: Banana Pi BPI-R3
- SoC: MediaTek MT7986
- OpenWrt target: `mediatek/filogic`
- OpenWrt device: `bananapi_bpi-r3`
- Kernel: Linux 6.12.94

### OpenWrt

- Release/tag: `v25.12.5`
- Git commit: `f0a60eee2fe051741c643ea6118718aae1ef17fb`

### Project

- Project commit: `b3b2ead`
- Project tag: `openwrt-integration-working`

### Enabled Homematic packages

- `kmod-eq3-char-loop`
- `kmod-generic-raw-uart`
- `kmod-mtk-raw-uart`
- `hmlangw`
- `multimacd-native`

### Build results

| Component | Result |
|---|---|
| OpenWrt package discovery | PASS |
| MediaTek kernel patch integration | PASS |
| Linux kernel build | PASS |
| eq3-char-loop | PASS |
| generic-raw-uart | PASS |
| mtk-raw-uart | PASS |
| hmlangw | PASS |
| multimacd-native | PASS |
| Full OpenWrt image build | PASS |

### Notes

The Homematic packages were enabled explicitly in the OpenWrt configuration
before running `make defconfig`.

The complete OpenWrt build completed successfully including package
installation, target installation, package index generation, image metadata
generation and checksum generation.


## Runtime verification

The OpenWrt 25.12.5 build documented above was installed and tested on a
physical Banana Pi BPI-R3 with a Homematic RF module connected through the
MT7986 UART implementation.

### Runtime results

| Component | Result |
|---|---|
| Banana Pi BPI-R3 boot | PASS |
| Device-tree UART configuration | PASS |
| `pivccu,mt7986-uart` detection | PASS |
| `mtk_raw_uart` module | PASS |
| `generic_raw_uart` module | PASS |
| `eq3_char_loop` module | PASS |
| `/dev/raw-uart` creation | PASS |
| `/dev/eq3loop` creation | PASS |
| `/dev/mmd_bidcos` creation | PASS |
| `/dev/mmd_hmip` creation | PASS |
| Homematic RF module reset/initialization | PASS |
| RF module detection | PASS |
| `multimacd` runtime | PASS |
| `hmlangw` runtime | PASS |
| BidCos LAN connection | PASS |
| Keepalive connection | PASS |

### Verified UART

The MediaTek raw UART driver successfully initialized the UART at:

    mapbase = 0x11003000
    IRQ     = 137
    baudclk = 26000000
    busclk  = 26000000

The kernel reported successful radio-module reset and registration of the
raw UART device.

`eq3_char_loop` subsequently provided the Homematic communication devices,
including `/dev/mmd_bidcos` and `/dev/mmd_hmip`.

### LAN gateway verification

`hmlangw` successfully used `/dev/mmd_bidcos`.

A Homematic client successfully established both the BidCos connection and
the corresponding keepalive connection to the Banana Pi BPI-R3 gateway.

This verifies the complete runtime path:

    MT7986 UART
        -> mtk_raw_uart
        -> generic_raw_uart
        -> raw UART device
        -> eq3_char_loop
        -> mmd_bidcos
        -> hmlangw
        -> Homematic LAN client

Runtime result: **PASS**
