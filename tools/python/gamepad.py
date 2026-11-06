#!/usr/bin/env python3
"""
EOS Gamepad & Chord Daemon for A/V (Kodi), SDL2 Games (Dinamo), and Interactive Runtimes.

Supports D-Pad / thumbstick navigation and a Select+Start controller chord
to switch active EOS runtime services via /opt/eos/bin/switcher.py.
"""

import os
import subprocess
import sys
import time

import pygame

CHORD_TARGET_SERVICE = os.environ.get("EOS_CHORD_SERVICE", "dinamo")

try:
    pygame.init()
    if pygame.joystick.get_count() == 0:
        raise pygame.error("No joysticks found")
    j = pygame.joystick.Joystick(0)
    j.init()
    print(f"Initialized Joystick : {j.get_name()}")
except pygame.error:
    print("Initialized without joysticks")
    sys.exit(0)


def get():
    out = [0] * 17
    it = 0
    pygame.event.pump()

    # Read input from the two joysticks
    for i in range(j.get_numaxes()):
        if it < len(out):
            out[it] = j.get_axis(i)
            it += 1

    # Read input from buttons
    for i in range(j.get_numbuttons()):
        if it < len(out):
            out[it] = j.get_button(i)
            it += 1

    return out


def capture():
    while True:
        btns = get()
        # Select + Start chord switches foreground runtime service
        if btns[5] > 0 and btns[6] > 0:
            subprocess.run(["/opt/eos/bin/switcher.py", CHORD_TARGET_SERVICE], check=False)
            time.sleep(0.5)
            continue

        if btns[0] < 0:
            subprocess.run(["kodi-send", "--action=Left"], check=False)
        elif btns[0] > 0:
            subprocess.run(["kodi-send", "--action=Right"], check=False)
        elif btns[1] < 0:
            subprocess.run(["kodi-send", "--action=Up"], check=False)
        elif btns[1] > 0:
            subprocess.run(["kodi-send", "--action=Down"], check=False)
        elif btns[5] > 0:
            subprocess.run(["kodi-send", "--action=Back"], check=False)
        elif btns[6] > 0:
            subprocess.run(["kodi-send", "--action=Select"], check=False)
        time.sleep(0.05)


if __name__ == "__main__":
    capture()
