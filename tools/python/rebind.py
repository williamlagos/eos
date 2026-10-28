#!/usr/bin/env python3
"""
Verify if a joystick is connected and rebind the first USB controller (/dev/input/js0).
"""

import sys
import pygame
import pyudev as udev


def rebind_joystick(devname: str = "/dev/input/js0") -> int:
    pygame.init()
    if pygame.joystick.get_count() == 0:
        return 0

    j = pygame.joystick.Joystick(0)
    j.init()

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
    sys.exit(rebind_joystick())
