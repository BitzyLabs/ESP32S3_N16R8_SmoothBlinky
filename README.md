# ESP32-S3 Onboard NeoPixel Smooth Rainbow + White

This project demonstrates a **smooth rainbow and white blending effect** on the **onboard NeoPixel LED** of an ESP32-S3 N16R8 devkit. It includes a **hardware information printout** on the serial monitor before starting the LED effect.

---

## Table of Contents

- [Features](#features)
- [Hardware Requirements](#hardware-requirements)
- [Software Requirements](#software-requirements)
- [PlatformIO Configuration](#platformio-configuration)
- [Pin Configuration](#pin-configuration)
- [Usage](#usage)
- [How It Works](#how-it-works)
- [Customization](#customization)
- [License](#license)

---

## Features

- Friendly **boot greeting** on the serial monitor:
  - `Hellooo, fellow maker!!`
  - `Thanks heaps for your ESP32 N16R8 Kit Purchase from Bitzy Labs!!`
  - `This is what you've got :`
- **Static hardware details block** - printed once at boot and never re-sent,
  so it stays put while the color feed runs below it:
  - Chip model and silicon revision (actual values read live from the chip)
  - CPU cores and CPU frequency
  - Flash size (read from the flash chip ID) and PSRAM size (from the heap)
  - Kit check: confirms the N16R8 profile (16 MB flash + 8 MB PSRAM)
  - MAC address, console UART, firmware build date, free heap
  - USB-UART bridge chip type and firmware version (e.g. `WCH CH343,
    fw v4.45`) - the bridge sits on the PC side of the USB link, so the
    firmware cannot query it. When built on Linux with the kit plugged in,
    `scripts/uart_bridge_info.py` reads the actual USB descriptor and
    injects it as `UART_BRIDGE_USB_INFO` - but only if the device is the
    kit's own bridge (`1A86:55D3`), so an unrelated serial dongle on the
    build machine can never mislabel a kit. On Windows/macOS, or whenever
    the kit is not plugged in during the build, the firmware falls back to
    the kit's shipping spec, so the line is correct on every OS.
- **Header recall**: press **Enter** in any serial monitor and the full
  greeting + static block is reprinted instantly - useful when you attach
  the monitor after boot (most monitors do not reset the board on connect).
  `scripts/monitor_with_header.sh` (and the VS Code task
  *PIO Monitor (prints kit header)*) does this automatically for the
  PlatformIO monitor.
- **Real-time color feed**: the current LED color is printed on a single
  self-updating line (`\r`), so the static header never scrolls past.
  Set `LIVE_SINGLE_LINE` to `0` for one-line-per-update scrolling output.
- Smooth **rainbow color transitions** across the full hue spectrum.
- **White blending** using a soft sine wave for a cinematic effect.
- Adjustable **brightness and speed**.
- Runs on a single onboard NeoPixel (RGB LED).

---

## Hardware Requirements

- **ESP32-S3 N16R8 devkit** (dual USB-C version tested)
- Built-in onboard **addressable NeoPixel LED** (pin 48)
- USB-C cable for programming and serial output

---

## Software Requirements

- [PlatformIO](https://platformio.org/)
- Visual Studio Code (VSCode)
- [Adafruit NeoPixel library](https://github.com/adafruit/Adafruit_NeoPixel)

---

## PlatformIO Configuration

Include the following `platformio.ini` in your project:

```ini
[env:esp32s3dev]
platform = espressif32
board = esp32-s3-devkitc-1
framework = arduino
lib_deps = adafruit/Adafruit NeoPixel@^1.10.6
monitor_speed = 115200
; N16R8 kit: quad-flash (qio) + 8 MB octal PSRAM (opi)
board_build.arduino.memory_type = qio_opi
; esp32-hal-psram.c never includes sdkconfig.h in this core, so psramFound()
; and friends compile to always-false stubs unless these are passed on the
; command line. Values match qio_opi's sdkconfig.h exactly (IDF auto-inits
; PSRAM at boot, Arduino then just marks it as found).
build_flags =
    -DCONFIG_SPIRAM=1
    -DCONFIG_SPIRAM_BOOT_INIT=1
; Read the USB-UART bridge chip's identity (type + firmware version) from the
; PC side at build time and inject it as UART_BRIDGE_USB_INFO
extra_scripts = pre:scripts/uart_bridge_info.py
```

---

## Pin Configuration

LED_PIN: 48 (onboard NeoPixel)

NUM_LEDS: 1

BRIGHTNESS: 0-255

DELAY_MS: Delay between color updates (default 20ms)

INFO_DELAY: Pause in milliseconds before starting the LED effect (default 2000ms)

---

## Usage

Open the project in VSCode with PlatformIO.

Connect your ESP32-S3 devkit via USB-C.

Build and upload the firmware:
platformio run --target upload

Open the serial monitor at 115200 baud.

The greeting and the hardware details block print at boot, followed by a 2
second pause. Then the LED starts the smooth rainbow + white effect while the
serial monitor streams the live color feed on a single line below the static
header.

### Opening the monitor after the board is already running

Most serial monitors (including `pio device monitor`) attach **without
resetting the board**, so if you open the monitor mid-run you will only see
the live color feed - the boot header has already been printed. Bring it back
with either of:

- **Press Enter** in the serial monitor - the firmware reprints the full
  greeting + static block immediately (works in any serial monitor, on any
  OS).
- **Run `./scripts/monitor_with_header.sh`** (or the VS Code task
  *Tasks → Run Task → PIO Monitor (prints kit header)*) - it attaches the
  PlatformIO monitor and automatically triggers the reprint for you. Port
  and environment can be overridden with `MONITOR_PORT=/dev/ttyUSB0` and
  `PIO_ENV=esp32s3dev`. After a fresh upload, use
  `./scripts/monitor_with_header.sh --upload` (VS Code task
  *PIO Upload & Monitor (prints kit header)*) to build, upload, monitor and
  show the header in one step.
- **Reset the board** (EN/RST button or replug) - the header prints on boot
  as usual.

---

## How It Works

Hardware Info Print:
Prints basic ESP32-S3 details via the serial monitor, giving insight into the chip configuration.

Smooth Rainbow + White LED Effect:

Uses HSV to RGB conversion for smooth color transitions.

Gamma correction is applied for perceptual brightness smoothing.

White blending is achieved using a sine wave, which gently adds brightness to the rainbow colors.

The LED updates continuously with DELAY_MS controlling update speed and hue increment controlling the color flow speed.

Loop Mechanics:
The loop increments the hue and white phase continuously, setting the LED color for each step to achieve a smooth, cinematic visual effect.

---

## Customization

You can modify the following parameters:

Parameter	Description
BRIGHTNESS	Overall LED brightness (0-255)
DELAY_MS	Delay between LED updates (lower = faster)
hue increment	Controls speed of color flow (lower = slower)
whitePhase increment	Controls speed of white blending
NUM_LEDS	If using additional LEDs in a strip
LED_PIN	Change if your NeoPixel is on a different pin
LIVE_SINGLE_LINE	1 = live feed overwrites one line so the header stays visible; 0 = one full line per update (scrolls)
LIVE_MS_SINGLE / LIVE_MS_SCROLL	How often the live color line refreshes (ms)

You can also replace the single LED with a NeoPixel strip by adjusting NUM_LEDS and adding strip effects in the loop.

---

## License

This project is released under the MIT License.

---

##
Author: RajindraAbeywick
Website: BitzyLabs
