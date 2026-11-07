# EOS Tools

Utilities, cross-compilation configurations, and service stacks for EOS IoT and Cloud environments.

## Directory Structure

- `cross/`: `crosstool-NG` configurations and CMake toolchain files for Arch Linux ARM targets:
  - `xtools-dotconfig-v6` & `toolchain-armv6h.cmake`: ARMv6 hard-float (`armv6h` — Raspberry Pi 1 / Zero / Zero W).
  - `xtools-dotconfig-v7` & `toolchain-armv7h.cmake`: ARMv7 hard-float (`armv7h` — Raspberry Pi 2 / 3 32-bit).
  - `xtools-dotconfig-v8` & `toolchain-aarch64.cmake`: ARMv8 64-bit (`aarch64` — Raspberry Pi 3 / 4 / 5, ARM handhelds & Cloud VMs).
- `python/`: Python 3 hardware, sensor, network, audio, chroot, and devkit scripts installed to `/opt/eos/bin` on EOS IoT, Runtime, and SDK targets:
  - `connect.py`, `network.py`, `ssids.py`: NetworkManager Wi-Fi provisioning and fallback AP supervision.
  - `sensors.py`: Linux IIO / I2C 6-axis IMU (gyroscope/accelerometer) and environmental sensor telemetry bridge (integrates with `elements` `SensorFrame`).
  - `buttons.py`, `gamepad.py`, `rebind.py`, `key-rebind.py`: USB input / controller udev rebind and `Select+Start` runtime service chord switcher.
  - `access.py`, `retro.py`, `switcher.py`, `sound.py`, `splash.py`, `btsync.py`: System chroot, `.gb`/`.pocket` ROM runner (`mgba-sdl`), foreground service switching, PipeWire/PulseAudio healthcheck, and framebuffer utilities.
  - `devkit.py`: Remote artifact deployment (`rsync`) and live service hot-reload (`synapse-client -p 9460 reload`) for Steam Deck and Raspberry Pi development.
- `markt/`: Containerized web stack (`docker-compose.yml`) for EOS Cloud reverse-proxy deployments.
