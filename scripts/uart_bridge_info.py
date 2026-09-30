"""Inject the USB-UART bridge chip's identity into the firmware.

The ESP32 cannot query the bridge at runtime - it sits on the other side of
the TX/RX lines. But the bridge describes itself over USB, so we read its
descriptor (VID:PID, firmware/bcdDevice version, product string) from sysfs
at build time and pass it to the firmware as UART_BRIDGE_USB_INFO.

Usage: extra_scripts = pre:scripts/uart_bridge_info.py
"""
Import("env")  # noqa: F821 - provided by PlatformIO/SCons

import glob
import os

# Well-known USB-serial chip IDs, keyed by the VID:PID the chip reports
# in its own descriptor.
PID_NAMES = {
    "1a86:55d3": "WCH CH343",
    "1a86:55d2": "WCH CH342",
    "1a86:7523": "WCH CH340",
    "1a86:5523": "WCH CH341",
    "1a86:55d4": "WCH CH9102",
    "10c4:ea60": "Silicon Labs CP2102",
    "0403:6001": "FTDI FT232R",
    "303a:1001": "Espressif USB-Serial/JTAG (native USB, no external bridge)",
    "303a:4001": "Espressif USB CDC (native USB, no external bridge)",
}


def find_port():
    """Upload port if configured, otherwise first USB serial device."""
    explicit = env.GetProjectOption("upload_port", None)  # noqa: F821
    if explicit and not explicit.startswith("$"):
        return explicit
    for pattern in ("/dev/ttyACM*", "/dev/ttyUSB*"):
        ports = sorted(glob.glob(pattern))
        if ports:
            return ports[0]
    return None


def usb_descriptor(port):
    """Walk from the tty node up to its USB device directory in sysfs."""
    node = "/sys/class/tty/%s/device" % os.path.basename(port)
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


info = None
port = find_port()
if port:
    descriptor = usb_descriptor(port)
    if descriptor:
        key = "%s:%s" % (descriptor["vid"], descriptor["pid"])
        name = PID_NAMES.get(key) or descriptor["product"] or "USB serial device"
        info = "%s (USB %s, fw v%s)" % (name, key.upper(), bcd_version(descriptor["bcd"]))
        print("UART bridge detected on %s: %s" % (port, info))

if info:
    env.Append(  # noqa: F821
        CPPDEFINES=[("UART_BRIDGE_USB_INFO", env.StringifyMacro(info))]  # noqa: F821
    )
else:
    print("UART bridge: not detected (build with the kit connected); "
          "firmware will print the fallback text")
