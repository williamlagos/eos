# EOS Packages

Arch Linux (`x86_64`) and Arch Linux ARM (`armv6h`, `armv7h`, `aarch64`) package definitions (`PKGBUILD`) and package manifests (`pkglists/`) for EOS containers and virtual machines.

## Custom Packages (`PKGBUILD`s)

| Directory | Package | Architectures | Description |
|---|---|---|---|
| `libsockets/` | `libsockets-git` | `armv6h`, `armv7h`, `aarch64`, `x86_64` | C11 TCP/UDP socket library with `libuv` async API (`uv_sockets`). |
| `synapse/` | `synapse-git` | `armv6h`, `armv7h`, `aarch64`, `x86_64` | Reactive process control & sensor daemon (`synapse`, `synapse-client`, `libimpulse.so`, and systemd unit). |
| `elements/` | `elements-git` | `armv6h`, `armv7h`, `aarch64`, `x86_64` | C++17 spatial, sensor (`SensorFrame`), and game engine primitives (`libelements_core.a` + `pybind11` `_elements`). |
| `dinamo/` | `dinamo-git` | `armv6h`, `armv7h`, `aarch64`, `x86_64` | Native SDL2 + `elements_core` arcade space shooter and canonical `shared/` assets for Steam Deck & EOS Runtime. |
| `binfmt-qemu-static-all-arch/` | `binfmt-qemu-static-all-arch` | `armv6h`, `armv7h`, `aarch64`, `x86_64` | Registers `binfmt.d` rules for transparent multi-arch container/chroot emulation. |
| `qemu-user-static/` | `qemu-user-static` | `x86_64`, `aarch64` | Statically linked QEMU user-mode emulation binaries. |
| `distccd-arm/` | `distccd-alarm-*` | `x86_64` | Distributed cross-compiler (`x-tools`) services for `armv6h` (Pi 1/Zero), `armv7h` (Pi 2/3), and `armv8` (`aarch64`). |

## Package Lists (`pkglists/`)

- `pkglists/base.txt`: Core minimal packages required across all EOS environments.
- `pkglists/iot.txt` / `pkglists/eos.txt`: Packages for IoT, edge gateways, and Raspberry Pi (`armv6h`+).
- `pkglists/runtime.txt`: Packages for interactive SBCs, A/V appliances, handhelds, and SDL2/KMSDRM/Vulkan game runtimes.
- `pkglists/sdk.txt`: Compilers, build tools, headers, and cross-compilation dependencies for the EOS SDK container/environment.
- `pkglists/cloud.txt` / `pkglists/pkglist.cloud.txt`: Packages for Cloud application servers, multiplayer backends, and reverse proxies.
