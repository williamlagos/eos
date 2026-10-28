#!/usr/bin/env python3
"""
Switch foreground systemd service (e.g. pause kodi while another service runs).

When running in bash:
/opt/eos/bin/switcher.py <service_name>
"""

import subprocess
import sys
import time


def switch_service(target_service: str) -> int:
    subprocess.run(["systemctl", "stop", "kodi"], check=False)
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

    return subprocess.run(["systemctl", "start", "kodi"], check=False).returncode


if __name__ == "__main__":
    if len(sys.argv) > 1:
        sys.exit(switch_service(sys.argv[1]))
