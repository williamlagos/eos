#!/usr/bin/env python3
"""
Gamepad Module
Daniel J. Gonzalez
dgonz@mit.edu

Based off code from: http://robots.dacloughb.com/project-1/logitech-game-pad/
"""

import subprocess
import sys
import time

import pygame

pygame.init()
if pygame.joystick.get_count() == 0:
    print("No joystick detected", file=sys.stderr)
    sys.exit(0)

j = pygame.joystick.Joystick(0)
j.init()
print(f"Initialized Joystick : {j.get_name()}")

"""
Returns a vector of the following form:
[LThumbstickX, LThumbstickY, Unknown Coupled Axis???,
RThumbstickX, RThumbstickY,
Button 1/X, Button 2/A, Button 3/B, Button 4/Y,
Left Bumper, Right Bumper, Left Trigger, Right Trigger,
Select, Start, Left Thumb Press, Right Thumb Press]

Note:
No D-Pad.
Triggers are switches, not variable.
Your controller may be different
"""


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


def test():
    while True:
        buttons = get()
        print(buttons)
        if buttons[0] < 0:
            subprocess.run(["kodi-send", "--action=Left"], check=False)
        if buttons[0] > 0:
            subprocess.run(["kodi-send", "--action=Right"], check=False)
        if buttons[1] < 0:
            subprocess.run(["kodi-send", "--action=Up"], check=False)
        if buttons[1] > 0:
            subprocess.run(["kodi-send", "--action=Down"], check=False)
        if buttons[5] > 0:
            subprocess.run(["kodi-send", "--action=Back"], check=False)
        if buttons[6] > 0:
            subprocess.run(["kodi-send", "--action=Select"], check=False)
        time.sleep(1)


if __name__ == "__main__":
    test()
