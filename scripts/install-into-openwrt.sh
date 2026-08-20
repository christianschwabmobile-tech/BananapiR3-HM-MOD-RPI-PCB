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
echo "Applying OpenWrt patches..."

for patchfile in "$PROJECT_DIR"/patches/openwrt/*.patch; do
    [ -e "$patchfile" ] || continue

    echo "  -> $(basename "$patchfile")"

    patch -d "$OPENWRT_DIR" \
        -p1 \
        --forward \
        --batch \
        < "$patchfile"
done

echo
echo "Installation completed."
