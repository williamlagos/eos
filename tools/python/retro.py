#!/usr/bin/env python3
"""
Run a command inside the /opt/retro chroot environment.
"""

import subprocess
import sys

CHROOT_CMD = "chroot"
RETRO_PATH = "/opt/retro"

if __name__ == "__main__":
    if len(sys.argv) > 1:
        sys.exit(
            subprocess.run(
                [CHROOT_CMD, RETRO_PATH, "bash", "-c", sys.argv[1]],
                check=False,
            ).returncode
        )
