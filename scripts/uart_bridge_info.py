"""Inject the USB-UART bridge chip's identity into the firmware.

The ESP32 cannot query the bridge at runtime - it sits on the other side of
the TX/RX lines. But the bridge describes itself over USB, so on Linux we
read its descriptor (VID:PID, firmware/bcdDevice version) from sysfs at
build time and pass it to the firmware as UART_BRIDGE_USB_INFO.

On Windows/macOS (no /sys/class/tty) or when the kit is not plugged in,
nothing is injected and the firmware prints its built-in Bitzy Labs
kit-spec line instead - so the output is correct on every OS.

Usage: extra_scripts = pre:scripts/uart_bridge_info.py
"""
Import("env")  # noqa: F821 - provided by PlatformIO/SCons

import glob
import os

# USB-serial chips the kit may ship with, keyed by the VID:PID the chip
# reports in its own descriptor.
PID_NAMES = {
    "1a86:55d3": "WCH CH343",
    "1a86:55d2": "WCH CH342",
    "1a86:7523": "WCH CH340",
    "1a86:5523": "WCH CH341",
    "1a86:55d4": "WCH CH9102",
    "10c4:ea60": "Silicon Labs CP2102",
    "0403:6001": "FTDI FT232R",
}

# Only descriptors from the kit's own bridge may be injected. This stops a
# stray CH340/CP2102 dongle on the build machine from mislabeling the kit -
# if the kit is absent (or uses an unknown chip) the firmware falls back to
# the kit-spec line, which is true for every unit we ship.
# (If a future hardware batch changes the bridge chip, add its PID here and
# update the fallback string in src/main.cpp.)
KIT_BRIDGE_PIDS = {"1a86:55d3"}

TTY_PATTERNS = ("/dev/ttyACM*", "/dev/ttyUSB*")
SYSFS_TTY = "/sys/class/tty"


def candidate_ports(env):
    """Upload port if configured, otherwise every USB serial device."""
    explicit = env.GetProjectOption("upload_port", None)  # noqa: F821
    if explicit and not explicit.startswith("$"):
        return [explicit]
    ports = []
    for pattern in TTY_PATTERNS:
        ports.extend(sorted(glob.glob(pattern)))
    return ports


def usb_descriptor(port):
    """Walk from the tty node up to its USB device directory in sysfs."""
    node = os.path.join(SYSFS_TTY, os.path.basename(port), "device")
    if not os.path.exists(node):
        return None
    directory = os.path.realpath(node)
    while directory and directory != "/":
        if os.path.exists(os.path.join(directory, "idVendor")):

            def read(field, default=""):
                try:
                    with open(os.path.join(directory, field)) as handle:
                        return handle.read().strip()
                except OSError:
                    return default

            return {
                "vid": read("idVendor", "????"),
                "pid": read("idProduct", "????"),
                "bcd": read("bcdDevice", "????"),
                "product": read("product"),
            }
        directory = os.path.dirname(directory)
    return None


def bcd_version(bcd):
    """Decode a raw-hex bcdDevice ('0445') into a version string ('4.45')."""
    try:
        value = int(bcd, 16)
        hi, lo = (value >> 8) & 0xFF, value & 0xFF
        major = ((hi >> 4) * 10) + (hi & 0x0F)
        minor = ((lo >> 4) * 10) + (lo & 0x0F)
        return "%d.%02d" % (major, minor)
    except ValueError:
        return bcd or "unknown"


def find_kit_bridge(env):
    """First plugged device that is the kit's bridge chip (never a stranger)."""
    for port in candidate_ports(env):
        descriptor = usb_descriptor(port)
        if not descriptor:
            continue
        key = "%s:%s" % (descriptor["vid"], descriptor["pid"])
        if key in KIT_BRIDGE_PIDS:
            return port, descriptor, key
    return None, None, None


def detect_and_inject(env):
    port, descriptor, key = find_kit_bridge(env)
    if not port:
        print("UART bridge: kit bridge not found (Windows/macOS build, or "
              "kit unplugged); firmware will print the kit-spec line")
        return

    name = PID_NAMES.get(key) or "USB serial device"
    info = "%s (USB %s, fw v%s)" % (name, key.upper(), bcd_version(descriptor["bcd"]))
    print("UART bridge detected on %s: %s" % (port, info))
    env.Append(  # noqa: F821
        CPPDEFINES=[("UART_BRIDGE_USB_INFO", env.StringifyMacro(info))]  # noqa: F821
    )


detect_and_inject(env)
