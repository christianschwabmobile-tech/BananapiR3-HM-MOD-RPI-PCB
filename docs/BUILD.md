# Building with OpenWrt

## Requirements

A supported OpenWrt build environment and a clean OpenWrt source tree are
required.

The exact tested OpenWrt revision, target and build configuration should be
recorded for every verified project release.

## Integrating the project

From the project repository:

    ./scripts/install-into-openwrt.sh /path/to/openwrt

The script performs two operations:

1. installs the Homematic packages under:

    package/homematic/

2. applies the patches stored under:

    patches/openwrt/

## Configuration

After integration:

    cd /path/to/openwrt
    make menuconfig

Select the required Homematic packages.

The exact package selection will later be documented as a reproducible OpenWrt
configuration seed.

## Build

Build OpenWrt normally, for example:

    make -j"$(nproc)"

For debugging:

    make -j1 V=s

## Reproducibility

A problem report should always include:

- Banana Pi BPI-R3 hardware variant
- radio module type
- OpenWrt release
- OpenWrt Git revision
- OpenWrt target/subtarget
- kernel version
- project Git revision
- selected Homematic packages
- relevant device-tree state

A diagnostic collection script will be provided by this project.
