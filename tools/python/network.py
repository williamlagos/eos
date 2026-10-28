#!/usr/bin/env python3
"""
Monitor known NetworkManager Wi-Fi connections and start the fallback AP (create_ap)
when no known network is reachable.

When running in bash:
/opt/eos/bin/network.py
"""

import os
import subprocess
import time

import NetworkManager  # type: ignore

CONNECTIONS_DIR = "/etc/NetworkManager/system-connections"


def monitor_network(poll_interval: int = 10) -> None:
    while True:
        ssids = set()
        for dev in NetworkManager.NetworkManager.GetDevices():
            if dev.DeviceType != NetworkManager.NM_DEVICE_TYPE_WIFI:
                continue
            for ap in dev.SpecificDevice().GetAccessPoints():
                ssids.add(ap.Ssid)

        connections = (
            os.listdir(CONNECTIONS_DIR) if os.path.isdir(CONNECTIONS_DIR) else []
        )

        matched = any(
            con.removesuffix(".nmconnection") in ssids for con in connections
        )
        if not matched:
            subprocess.run(["systemctl", "start", "create_ap"], check=False)

        time.sleep(poll_interval)


if __name__ == "__main__":
    monitor_network()
