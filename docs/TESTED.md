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

Runtime verification on the Banana Pi BPI-R3 should be recorded separately
from build verification.
