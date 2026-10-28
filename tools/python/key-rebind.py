#!/usr/bin/env python3
"""
Rebind the first USB input device (/dev/input/event0) via sysfs.

When running on bash:
/opt/eos/bin/key-rebind.py
"""

import sys
import pyudev as udev


def rebind_input_device(devname: str = "/dev/input/event0") -> int:
    context = udev.Context()
    dev = None
    for d in context.list_devices(subsystem="input", DEVNAME=devname):
        dev = d

    if dev is None:
        return 0

    usb_id = None
    for part in dev["DEVPATH"].split("/"):
        if "-" in part and "." in part:
            usb_id = part
            break

    if not usb_id:
        return 0

    for action in ("unbind", "bind"):
        with open(f"/sys/bus/usb/drivers/usb/{action}", "w", encoding="utf-8") as sysfs:
            sysfs.write(usb_id)

    return 0


if __name__ == "__main__":
    target = sys.argv[1] if len(sys.argv) > 1 else "/dev/input/event0"
    sys.exit(rebind_input_device(target))
