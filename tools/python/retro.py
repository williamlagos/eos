#!/usr/bin/env python3
"""
Run a Game Boy / Analogue Pocket ROM (.gb, .gbc, .pocket) via mgba-sdl
or execute a command inside the /opt/retro chroot environment.

Usage:
    /opt/eos/bin/retro.py --rom /usr/share/dinamo/gbgame.gb
    /opt/eos/bin/retro.py "<chroot_command>"
"""

import shutil
import subprocess
import sys

CHROOT_CMD = "chroot"
RETRO_PATH = "/opt/retro"
ROM_EXTENSIONS = (".gb", ".gbc", ".pocket", ".gba")


def run_rom(rom_path: str) -> int:
    emulator = shutil.which("mgba-sdl") or shutil.which("mgba") or "mgba-sdl"
    return subprocess.run([emulator, "-f", rom_path], check=False).returncode


if __name__ == "__main__":
    if len(sys.argv) > 2 and sys.argv[1] == "--rom":
        sys.exit(run_rom(sys.argv[2]))
    elif len(sys.argv) > 1 and sys.argv[1].lower().endswith(ROM_EXTENSIONS):
        sys.exit(run_rom(sys.argv[1]))
    elif len(sys.argv) > 1:
        sys.exit(
            subprocess.run(
                [CHROOT_CMD, RETRO_PATH, "bash", "-c", sys.argv[1]],
                check=False,
            ).returncode
        )
