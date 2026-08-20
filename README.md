# Banana Pi BPI-R3 HM-MOD-RPI-PCB for OpenWrt as LAN Gateway

OpenWrt integration for using Homematic RF modules with the Banana Pi BPI-R3.

The project provides a low-latency raw UART implementation for the MediaTek
MT7986 UART controller together with the required Homematic/OpenCCU support
packages.

## Status

Current development status:

- Banana Pi BPI-R3: working
- MediaTek MT7986 raw UART backend: working
- generic_raw_uart integration: working
- Homematic radio communication: working
- OpenWrt package build: working
- multimacd native OpenWrt build: working

The Git tag `baseline-working` identifies the first known working project state.

## Architecture

The UART implementation is split into two layers:

    Homematic radio module
            |
            v
    generic_raw_uart
            |
            v
    mtk_raw_uart
            |
            v
    MediaTek MT7986 UART
            |
            v
    Banana Pi BPI-R3

`generic_raw_uart` provides the common Homematic raw UART interface.

`mtk_raw_uart` implements the MediaTek MT7986-specific backend used by the
Banana Pi BPI-R3.

## Repository layout

    package/homematic/
        eq3-char-loop/
        generic-raw-uart/
        hmlangw/
        mtk-raw-uart/
        multimacd-native/

    patches/openwrt/
        OpenWrt / device-tree integration

    scripts/
        helper scripts

    docs/
        project documentation

## OpenWrt integration

Start with a clean OpenWrt source tree.
https://openwrt.org/docs/guide-developer/toolchain/use-buildsystem

Clone this repository separately and run:

    ./scripts/install-into-openwrt.sh /path/to/openwrt

The script installs the Homematic packages and applies the required OpenWrt
patches.

Afterwards configure OpenWrt using `make menuconfig` or an existing build
configuration.

## Source provenance

This project contains code originating from several upstream projects,
including piVCCU, eQ-3/OCCU and OpenCCU.

Detailed source provenance and licensing information is documented in:

    docs/PROVENANCE.md

Existing upstream copyright and license notices must be preserved.

## Project goals

The project aims to provide:

- reproducible OpenWrt integration
- minimal modifications to upstream OpenWrt
- clear separation between generic and MediaTek-specific code
- explicit upstream provenance
- maintainability across future OpenWrt releases
- useful diagnostics for problem reports


## License

This repository contains components under different compatible open-source
licenses.

See `docs/PROVENANCE.md` and the individual source file headers for details.
