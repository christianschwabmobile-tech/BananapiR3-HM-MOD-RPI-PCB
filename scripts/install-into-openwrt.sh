#!/bin/sh

set -eu

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 /path/to/openwrt"
    exit 1
fi

OPENWRT_DIR="$(realpath "$1")"
PROJECT_DIR="$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)"

if [ ! -f "$OPENWRT_DIR/rules.mk" ] || [ ! -d "$OPENWRT_DIR/package" ]; then
    echo "ERROR: '$OPENWRT_DIR' does not look like an OpenWrt source tree."
    exit 1
fi

echo "Project: $PROJECT_DIR"
echo "OpenWrt: $OPENWRT_DIR"

echo
echo "Installing Homematic packages..."

mkdir -p "$OPENWRT_DIR/package/homematic"

for pkg in \
    eq3-char-loop \
    generic-raw-uart \
    hmlangw \
    mtk-raw-uart \
    multimacd-native
do
    echo "  -> $pkg"

    rm -rf "$OPENWRT_DIR/package/homematic/$pkg"
    cp -a \
        "$PROJECT_DIR/package/homematic/$pkg" \
        "$OPENWRT_DIR/package/homematic/"
done

echo
echo "Detecting MediaTek kernel patch directory..."

PATCH_DIR=""

for dir in "$OPENWRT_DIR"/target/linux/mediatek/patches-*; do
    if [ -d "$dir" ]; then
        if [ -n "$PATCH_DIR" ]; then
            echo "ERROR: multiple MediaTek kernel patch directories found."
            exit 1
        fi
        PATCH_DIR="$dir"
    fi
done

if [ -z "$PATCH_DIR" ]; then
    echo "ERROR: no target/linux/mediatek/patches-* directory found."
    exit 1
fi

echo "Kernel patches: $PATCH_DIR"

echo
echo "Installing Homematic kernel patches..."

for patchfile in "$PROJECT_DIR"/patches/openwrt/*.patch; do
    [ -e "$patchfile" ] || continue

    echo "  -> $(basename "$patchfile")"

    cp -f \
        "$patchfile" \
        "$PATCH_DIR/$(basename "$patchfile")"
done

echo
echo "Installation completed."
echo
echo "Next step:"
echo "  cd $OPENWRT_DIR"
echo "  make defconfig"
