#!/bin/sh

# Banana Pi BPI-R3 Homematic UART diagnostics
#
# Read-only diagnostic collection for OpenWrt systems.
# This script does not modify the system configuration.

set -u

OUTPUT="${1:-homematic-bpi-r3-diagnostics-$(date +%Y%m%d-%H%M%S).txt}"

section()
{
    echo
    echo "======================================================================"
    echo "$1"
    echo "======================================================================"
}

run()
{
    echo
    echo "\$ $*"
    "$@" 2>&1 || true
}

{
    section "DIAGNOSTIC INFORMATION"

    echo "Generated: $(date -Iseconds 2>/dev/null || date)"
    echo "Hostname:  $(hostname 2>/dev/null || echo unknown)"

    section "OPENWRT RELEASE"

    if [ -f /etc/openwrt_release ]; then
        cat /etc/openwrt_release
    fi

    if [ -f /etc/os-release ]; then
        echo
        cat /etc/os-release
    fi

    section "SYSTEM"

    run uname -a
    run cat /proc/cpuinfo
    run cat /proc/meminfo

    section "DEVICE TREE"

    if [ -r /sys/firmware/devicetree/base/model ]; then
        echo
        printf "Model: "
        tr -d '\000' < /sys/firmware/devicetree/base/model
        echo
    fi

    run find /sys/firmware/devicetree/base \
        -maxdepth 4 \
        -type d \
        -iname '*serial*'

    section "DEVICE TREE - UART / HOMEMATIC"

    for node in \
        /sys/firmware/devicetree/base/soc/serial@11003000 \
        /sys/firmware/devicetree/base/soc/serial@11004000 \
        /sys/firmware/devicetree/base/soc/serial@11005000
    do
        if [ -d "$node" ]; then
            echo
            echo "----- $node -----"

            for prop in \
                compatible \
                status \
                reg \
                interrupts \
                clock-frequency \
                pivccu,reset-gpios
            do
                if [ -r "$node/$prop" ]; then
                    echo
                    echo "[$prop]"
                    od -An -tx1 "$node/$prop" 2>/dev/null || true
                    printf "ASCII: "
                    tr -d '\000' < "$node/$prop" 2>/dev/null || true
                    echo
                fi
            done
        fi
    done

    echo
    echo "Compatible nodes containing pivccu / homematic:"
    grep -RIl -E 'pivccu|homematic' \
        /sys/firmware/devicetree/base 2>/dev/null || true

    section "KERNEL MODULES"

    run lsmod

    echo
    echo "Relevant modules:"
    lsmod 2>/dev/null |
        grep -Ei 'raw_uart|raw-uart|eq3|mtk|serial|8250' || true

    section "MODULE INFORMATION"

    for module in \
        generic_raw_uart \
        mtk_raw_uart \
        eq3_char_loop
    do
        echo
        echo "----- $module -----"
        modinfo "$module" 2>&1 || true
    done

    section "MODULE PARAMETERS"

    for module in generic_raw_uart mtk_raw_uart eq3_char_loop; do
        paramdir="/sys/module/$module/parameters"

        echo
        echo "----- $module -----"

        if [ -d "$paramdir" ]; then
            for param in "$paramdir"/*; do
                [ -e "$param" ] || continue

                printf "%s=" "$(basename "$param")"
                cat "$param" 2>/dev/null || echo "<unreadable>"
            done
        else
            echo "No module parameter directory."
        fi
    done

    section "DEVICE NODES"

    run ls -la /dev

    echo
    echo "Relevant device nodes:"
    find /dev -maxdepth 1 \
        \( -name 'raw-uart*' \
        -o -name 'mmd_*' \
        -o -name 'eq3loop*' \
        -o -name 'ttyS*' \
        -o -name 'ttyAMA*' \) \
        -ls 2>/dev/null || true

    section "TTY / SERIAL"

    run cat /proc/tty/driver/serial

    section "INSTALLED HOMEMATIC PACKAGES"

    if command -v apk >/dev/null 2>&1; then
        apk list --installed 2>/dev/null |
            grep -Ei 'eq3|homematic|raw-uart|hmlangw|multimacd' || true
    elif command -v opkg >/dev/null 2>&1; then
        opkg list-installed 2>/dev/null |
            grep -Ei 'eq3|homematic|raw-uart|hmlangw|multimacd' || true
    else
        echo "No supported package manager found."
    fi

    section "PROCESSES"

    run ps w

    echo
    echo "Relevant processes:"
    ps w 2>/dev/null |
        grep -Ei '[m]ultimacd|[h]mlangw|[r]fd|[H]MServer' || true

    section "MULTIMACD CONFIGURATION"

    for file in \
        /etc/multimacd.conf \
        /etc/config/multimacd
    do
        if [ -f "$file" ]; then
            echo
            echo "----- $file -----"
            cat "$file"
        fi
    done

    section "HMLANGW CONFIGURATION"

    if [ -f /etc/config/hmlangw ]; then
        cat /etc/config/hmlangw
    fi

    section "INIT SCRIPTS"

    for service in multimacd hmlangw; do
        echo
        echo "----- $service -----"

        if [ -x "/etc/init.d/$service" ]; then
            "/etc/init.d/$service" enabled 2>&1 || true
            "/etc/init.d/$service" status 2>&1 || true
        else
            echo "/etc/init.d/$service not found"
        fi
    done

    section "NETWORK"

    run ip addr
    run ip route

    section "LISTENING SOCKETS"

    if command -v ss >/dev/null 2>&1; then
        run ss -lntup
    elif command -v netstat >/dev/null 2>&1; then
        run netstat -lntup
    fi

    section "KERNEL LOG - HOMEMATIC / UART"

    dmesg 2>/dev/null |
        grep -Ei 'homematic|eq3|raw.?uart|mtk.*uart|serial|ttyS|ttyAMA|multimacd' ||
        true

    section "SYSTEM LOG - HOMEMATIC"

    if command -v logread >/dev/null 2>&1; then
        logread 2>/dev/null |
            grep -Ei 'homematic|eq3|raw.?uart|hmlangw|multimacd' ||
            true
    fi

    section "FULL DMESG"

    dmesg 2>&1 || true

    section "END"

    echo
    echo "Diagnostic collection completed."

} > "$OUTPUT"

echo "Diagnostics written to:"
echo "$OUTPUT"
