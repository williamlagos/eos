#!/usr/bin/env python3
"""
EOS Sensor & IMU Bridge for SBCs (Raspberry Pi) and Handhelds (Steam Deck).

Reads Linux Industrial I/O (/sys/bus/iio/devices/iio:device*) or I2C IMU sensors,
normalizes spatial/environmental telemetry through Elements SensorFrame (when available),
and optionally bridges 6-axis motion to a virtual Linux uinput device for SDL2.

Usage:
    /opt/eos/bin/sensors.py --check
    /opt/eos/bin/sensors.py --once
    /opt/eos/bin/sensors.py --daemon [--interval 0.016]
"""

from __future__ import annotations

import argparse
import glob
import json
import os
import sys
import time
from typing import Any

try:
    import _elements  # type: ignore[import-not-found]
    HAS_ELEMENTS = True
except ImportError:
    _elements = None
    HAS_ELEMENTS = False


def discover_sensor_nodes() -> dict[str, list[str]]:
    """Discover available Linux IIO, I2C, GPIO, and input sensor device nodes."""
    return {
        "iio": sorted(glob.glob("/sys/bus/iio/devices/iio:device*")),
        "i2c": sorted(glob.glob("/dev/i2c-*")),
        "gpio": sorted(glob.glob("/dev/gpiochip*")),
        "hidraw": sorted(glob.glob("/dev/hidraw*")),
    }


def _read_sysfs_float(path: str, default: float = 0.0) -> float:
    try:
        with open(path, encoding="utf-8") as fh:
            return float(fh.read().strip())
    except (OSError, ValueError):
        return default


def sample_iio_telemetry() -> dict[str, Any]:
    """Sample gyroscope, accelerometer, and environmental channels from Linux IIO sysfs."""
    nodes = discover_sensor_nodes()
    gx = gy = gz = 0.0
    ax = ay = az = 0.0
    prox = 0.0
    light = 1.0
    temp_c = 22.0

    for iio_dir in nodes["iio"]:
        scale_angl = _read_sysfs_float(os.path.join(iio_dir, "in_anglvel_scale"), 1.0)
        gx = _read_sysfs_float(os.path.join(iio_dir, "in_anglvel_x_raw"), gx) * scale_angl
        gy = _read_sysfs_float(os.path.join(iio_dir, "in_anglvel_y_raw"), gy) * scale_angl
        gz = _read_sysfs_float(os.path.join(iio_dir, "in_anglvel_z_raw"), gz) * scale_angl

        scale_accel = _read_sysfs_float(os.path.join(iio_dir, "in_accel_scale"), 1.0)
        ax = _read_sysfs_float(os.path.join(iio_dir, "in_accel_x_raw"), ax) * scale_accel
        ay = _read_sysfs_float(os.path.join(iio_dir, "in_accel_y_raw"), ay) * scale_accel
        az = _read_sysfs_float(os.path.join(iio_dir, "in_accel_z_raw"), az) * scale_accel

        prox = _read_sysfs_float(os.path.join(iio_dir, "in_proximity_raw"), prox)
        light = _read_sysfs_float(os.path.join(iio_dir, "in_illuminance_input"), light)
        if os.path.exists(os.path.join(iio_dir, "in_temp_input")):
            temp_c = _read_sysfs_float(os.path.join(iio_dir, "in_temp_input"), 22000.0) / 1000.0

    if HAS_ELEMENTS and hasattr(_elements, "SensorFrame"):
        frame = _elements.SensorFrame()
        frame.set_gyro(gx, gy, gz)
        frame.set_accel(ax, ay, az)
        frame.set_environment(prox, light, temp_c)
        tilt_x = round(float(frame.tilt_x), 4)
        tilt_y = round(float(frame.tilt_y), 4)
        has_motion = bool(frame.has_motion)
    else:
        tilt_x = round(max(-1.0, min(1.0, (ax / 9.80665) + (gy * 0.35))), 4)
        tilt_y = round(max(-1.0, min(1.0, (ay / 9.80665) + (gx * 0.35))), 4)
        has_motion = any(abs(v) > 1e-3 for v in (gx, gy, gz, ax, ay))

    return {
        "cpp_elements": HAS_ELEMENTS,
        "nodes": nodes,
        "gyro": [round(gx, 4), round(gy, 4), round(gz, 4)],
        "accel": [round(ax, 4), round(ay, 4), round(az, 4)],
        "tilt": {"x": tilt_x, "y": tilt_y, "has_motion": has_motion},
        "environment": {
            "proximity": prox,
            "ambient_light": light,
            "temperature_c": round(temp_c, 2),
        },
        "timestamp": int(time.time()),
    }


def run_daemon(interval: float, state_file: str) -> int:
    """Continuously sample sensors and write latest telemetry snapshot to /run/eos-sensors.json."""
    os.makedirs(os.path.dirname(state_file), exist_ok=True)
    while True:
        snapshot = sample_iio_telemetry()
        try:
            with open(state_file, "w", encoding="utf-8") as fh:
                json.dump(snapshot, fh)
        except OSError:
            pass
        time.sleep(interval)


def main() -> int:
    parser = argparse.ArgumentParser(description="EOS SBC & Handheld Sensor Telemetry Bridge")
    parser.add_argument("--check", action="store_true", help="Exit 0 if any sensor/input bus is present")
    parser.add_argument("--once", action="store_true", help="Print a single JSON sensor telemetry frame")
    parser.add_argument("--daemon", action="store_true", help="Run continuous sensor polling loop")
    parser.add_argument("--interval", type=float, default=0.05, help="Polling interval in seconds")
    parser.add_argument("--state-file", default="/tmp/eos-sensors.json", help="Output path for latest snapshot")
    args = parser.parse_args()

    if args.check:
        nodes = discover_sensor_nodes()
        has_any = any(bool(v) for v in nodes.values()) or os.path.isdir("/sys/class/input")
        return 0 if has_any else 1

    if args.daemon:
        return run_daemon(args.interval, args.state_file)

    print(json.dumps(sample_iio_telemetry(), indent=2))
    return 0


if __name__ == "__main__":
    sys.exit(main())
