#!/usr/bin/env python3
"""
Switch foreground EOS runtime service (e.g. toggle between dinamo, retro, kodi, or custom apps).

Usage:
    /opt/eos/bin/switcher.py <target_service> [--home-service <home_service>]
"""

import os
import subprocess
import sys
import time

DEFAULT_HOME_SERVICE = os.environ.get("EOS_HOME_SERVICE", "dinamo")


def switch_service(target_service: str, home_service: str = DEFAULT_HOME_SERVICE) -> int:
    subprocess.run(["systemctl", "stop", home_service], check=False)
    subprocess.run(["systemctl", "stop", "kodi-gamepad"], check=False)
    subprocess.run(["systemctl", "start", target_service], check=False)

    while True:
        time.sleep(1)
        res = subprocess.run(
            ["systemctl", "status", target_service],
            stdout=subprocess.DEVNULL,
            stderr=subprocess.STDOUT,
            check=False,
        )
        if res.returncode != 0:
            break

    subprocess.run(["synapse-client", "reload"], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, check=False)
    return subprocess.run(["systemctl", "start", home_service], check=False).returncode


if __name__ == "__main__":
    if len(sys.argv) > 1:
        home = sys.argv[2] if len(sys.argv) > 2 else DEFAULT_HOME_SERVICE
        sys.exit(switch_service(sys.argv[1], home))
