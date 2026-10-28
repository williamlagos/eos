#!/usr/bin/env python3
"""
Query or initialize PulseAudio / PipeWire audio server status.
"""

import subprocess
import sys

if __name__ == "__main__":
    sys.exit(
        subprocess.run(
            ["pactl", "info"],
            stdout=subprocess.DEVNULL,
            stderr=subprocess.DEVNULL,
            check=False,
        ).returncode
    )
