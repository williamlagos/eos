#!/usr/bin/env python3
"""
Display OEM boot splash image on the framebuffer (/boot/oemsplash.png).
"""

import subprocess
import sys

IMG_PATH = "/boot/oemsplash.png"

if __name__ == "__main__":
    img = sys.argv[1] if len(sys.argv) > 1 else IMG_PATH
    sys.exit(
        subprocess.run(
            ["/usr/bin/fbi", "-T", "1", "-noverbose", img],
            stdout=subprocess.DEVNULL,
            stderr=subprocess.DEVNULL,
            check=False,
        ).returncode
    )
