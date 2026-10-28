#!/usr/bin/env python3
"""
Add a Wi-Fi connection to NetworkManager via python-networkmanager (or nmcli fallback).
"""

import argparse
import subprocess
import sys
import uuid


def add_wifi_connection(ssid: str, psk: str) -> None:
    try:
        import NetworkManager  # type: ignore

        conn = {
            "802-11-wireless": {
                "security": "802-11-wireless-security",
                "ssid": ssid,
                "name": "802-11-wireless",
                "mode": "infrastructure",
            },
            "connection": {
                "autoconnect": True,
                "type": "802-11-wireless",
                "id": ssid,
                "name": "connection",
                "uuid": str(uuid.uuid4()),
            },
            "ipv6": {"method": "ignore", "name": "ipv6"},
            "ipv4": {"method": "auto", "name": "ipv4"},
            "802-11-wireless-security": {
                "key-mgmt": "wpa-psk",
                "name": "802-11-wireless-security",
                "psk": psk,
            },
        }
        NetworkManager.Settings.AddConnection(conn)
    except ImportError:
        subprocess.run(
            ["nmcli", "device", "wifi", "connect", ssid, "password", psk],
            check=True,
        )


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Add a Wi-Fi connection to NetworkManager.")
    parser.add_argument("--ssid", default="EOS-IoT", help="Wi-Fi SSID")
    parser.add_argument("--psk", default="changeme123", help="WPA PSK passphrase")
    args = parser.parse_args()
    add_wifi_connection(args.ssid, args.psk)
    sys.exit(0)
