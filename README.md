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

- Prints **basic hardware information** on the serial monitor:
  - Chip model
  - CPU cores
  - CPU frequency
  - Flash size
  - Free heap memory
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

The hardware info will print for 2 seconds, then the LED will start the smooth rainbow + white effect.

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

You can also replace the single LED with a NeoPixel strip by adjusting NUM_LEDS and adding strip effects in the loop.

---

## License

This project is released under the MIT License.

---

##
Author: RajindraAbeywick
Website: BitzyLabs
