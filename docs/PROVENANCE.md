# Source provenance and licensing

This repository integrates several upstream components for using Homematic
radio modules with OpenWrt on the Banana Pi BPI-R3.

The goal is to keep upstream provenance explicit and to avoid presenting
third-party code as original work of this project.

## Components

### eq3-char-loop

Files:

- `package/homematic/eq3-char-loop/src/eq3_char_loop.c`
- `package/homematic/eq3-char-loop/src/stack_protector.include`

Origin:

- eQ-3 OCCU / piVCCU ecosystem
- Upstream `eq3_char_loop.c` is available in the eQ-3 OCCU repository.

Copyright:

- eQ-3 Entwicklung GmbH
- Original author: Lars Reemts

License:

- GNU General Public License, version 2 or later

Local purpose:

Provides `/dev/eq3loop` and the loop devices used by `multimacd`.


### generic-raw-uart

Files:

- `package/homematic/generic-raw-uart/src/generic_raw_uart.c`
- `package/homematic/generic-raw-uart/src/generic_raw_uart.h`
- `package/homematic/generic-raw-uart/src/pl011_raw_uart.c`
- `package/homematic/generic-raw-uart/src/devkey.inc`
- `package/homematic/generic-raw-uart/src/stack_protector.include`

Origin:

- piVCCU
- https://github.com/alexreinert/piVCCU

Copyright:

- Alexander Reinert
- Parts derived from eQ-3 `bcm2835_raw_uart.c`

License:

- GNU General Public License, version 2 or later

Local purpose:

Provides the generic low-latency raw UART interface required by Homematic
radio modules.

The upstream PL011 backend is retained as reference and compatibility code.
The Banana Pi BPI-R3 uses the separate MediaTek backend.


### mtk-raw-uart

Files:

- `package/homematic/mtk-raw-uart/src/mtk_raw_uart.c`

Origin:

- This project

Conceptually based on:

- piVCCU raw UART backends
- Linux MediaTek 8250 UART driver

License:

- GPL-2.0

Local purpose:

Hardware-specific raw UART backend for the MediaTek MT7986 UART controller
used by the Banana Pi BPI-R3.


### hmlangw

Files:

- `package/homematic/hmlangw/src/hmlangw.cpp`
- `package/homematic/hmlangw/src/hmframe.cpp`
- `package/homematic/hmlangw/src/hmframe.h`
- `package/homematic/hmlangw/src/LICENSE`

Copyright:

- Oliver Kastl
- Jens Maus
- Jérôme Pech

License:

- MIT

Local purpose:

Provides Homematic BidCoS LAN Gateway emulation.


### multimacd-native

Source is not redistributed in this repository.

Upstream:

- https://github.com/OpenCCU/OpenCCU-Base.git

Pinned commit:

- `5e9d5a4f59aeea8f951dacd1ba29edc60fc314b6`

The OpenWrt package downloads the source directly from upstream during the
build process.

Local changes are limited to OpenWrt build integration and compatibility
patches required for musl/OpenWrt.


## Project-specific changes

The main project-specific components are:

- MediaTek MT7986 raw UART backend
- Banana Pi BPI-R3 device-tree integration
- OpenWrt packaging
- OpenCCU/multimacd OpenWrt integration
- runtime configuration and diagnostics

Existing upstream copyright and license notices must not be removed.
