# EOS Tools

Utilities, cross-compilation configurations, and service stacks for EOS IoT and Cloud environments.

## Directory Structure

- `cross/`: `crosstool-NG` configurations for Arch Linux ARM targets:
  - `xtools-dotconfig-v6`: ARMv6 hard-float (`armv6h` — Raspberry Pi 1 / Zero / Zero W).
  - `xtools-dotconfig-v7`: ARMv7 hard-float (`armv7h` — Raspberry Pi 2 / 3 32-bit).
  - `xtools-dotconfig-v8`: ARMv8 64-bit (`aarch64` — Raspberry Pi 3 / 4 / 5 & ARM Cloud VMs).
- `python/`: Python 3 hardware, network, audio, and chroot scripts installed to `/opt/eos/bin` on EOS IoT targets:
  - `connect.py`, `network.py`, `ssids.py`: NetworkManager Wi-Fi provisioning and fallback AP supervision.
  - `buttons.py`, `gamepad.py`, `rebind.py`, `key-rebind.py`: USB input / controller udev rebind and event hooks.
  - `access.py`, `retro.py`, `switcher.py`, `sound.py`, `splash.py`, `btsync.py`: System chroot, service switching, audio, and framebuffer utilities.
- `markt/`: Containerized web stack (`docker-compose.yml`) for EOS Cloud reverse-proxy deployments.
