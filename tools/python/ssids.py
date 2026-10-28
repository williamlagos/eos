#!/usr/bin/env python3
"""
Display all visible Wi-Fi SSIDs via NetworkManager.
"""

import NetworkManager  # type: ignore


def list_ssids() -> None:
    for dev in NetworkManager.NetworkManager.GetDevices():
        if dev.DeviceType != NetworkManager.NM_DEVICE_TYPE_WIFI:
            continue
        for ap in dev.SpecificDevice().GetAccessPoints():
            print(f"{ap.Ssid:<30s} {ap.Frequency}MHz {ap.Strength}%")


if __name__ == "__main__":
    list_ssids()
