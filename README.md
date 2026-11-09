# EOS

**EOS** is a modular GNU/Linux base platform and SDK for **IoT / Edge devices**, **Interactive SBCs, A/V & Handheld Consoles (Steam Deck / Raspberry Pi)**, and **Cloud VMs & Containers**, built on **Arch Linux** (`x86_64`) and **Arch Linux ARM** (`armv6h` for Raspberry Pi 1/Zero, `armv7h` for Pi 2/3, and `aarch64` for Pi 3/4/5, ARM handhelds, and ARM Cloud).

At its core, EOS unifies four projects into a cohesive runtime and development kit:

1. **EOS (`eos`)** — Multi-stage container definitions (`Dockerfile`, `docker-compose.yml`), multi-machine VM topologies (`Vagrantfile`), declarative Ansible provisioning (`playbooks/`), Arch `PKGBUILD`s (`packages/`), CMake ARM cross-toolchains (`tools/cross/`), and Python 3 edge/sensor/gamepad/devkit tools (`tools/python/`).
2. **[Synapse](https://github.com/williamlagos/synapse)** — Lightweight reactive process supervisor and sensor engine written in C11 on top of `libuv`. Acts as PID 1 / service supervisor in containers or a systemd-managed node agent in VMs.
3. **[libsockets](https://github.com/williamlagos/libsockets)** — Asynchronous, non-blocking IPv4/IPv6 TCP socket library (`libuv_sockets` / `uv_sockets.h`) integrated into Synapse to provide remote TCP fleet & devkit control (`:9460`) alongside local Unix domain socket IPC (`/tmp/synapse.access`).
4. **[Elements](https://github.com/williamlagos/elements)** — C++17 spatial, sensor (`SensorFrame`), and game engine primitives (`libelements_core.a` + `pybind11` `_elements` Python bindings) pre-installed across all EOS profiles to power native SDL2 games ([`dinamo`](https://github.com/williamlagos/dinamo)), interactive SBC installations, and [`elements-server`](https://github.com/williamlagos/elements-server).

---

## Architecture & Profiles

| Profile | Target Stage / Machine | Purpose | Included Components |
|---|---|---|---|
| **Base** | `base` / `eos-base` | Minimal supervisor & engine base image / VM | Arch Linux base, `libuv`, `libsockets`, `synapse`, `synapse-client`, `libimpulse.so`, `elements_core` (`_elements`) |
| **IoT** | `iot` / `eos-iot` | Edge gateways, sensors & Raspberry Pi (`armv6h`+) | Base + `NetworkManager`, `avahi`, `bluez`, `i2c-tools`, `libgpiod`, `iio-sensor-proxy`, `pyudev`, `sensors.py`, `/opt/eos/bin/*.py` |
| **Runtime** | `runtime` / `eos-runtime` | Interactive SBCs, A/V, Steam Deck & Game runtime | IoT + `sdl2` (KMSDRM/Wayland), `libpng`, `mesa`/`vulkan-icd-loader`, `pipewire`, `gamescope`, `mgba-sdl`, `gamepad.py`, `switcher.py` |
| **SDK** | `sdk` / `eos-sdk` | Multi-arch C++17/Python/SDL2 development kit | Runtime + `base-devel`, `cmake`, `ninja`, `clang`, `gdb`, `pybind11`, `uv`, `/opt/eos/cross/toolchain-*.cmake`, `eos-devkit` |
| **Cloud** | `cloud` / `eos-cloud` | Cloud microservices, multiplayer & reverse proxy | Base + `nginx`, `redis`, `nodejs`, `python`, `uv`, `elements-server`, `tools/markt` stack |

---

## Quick Start

### 1. Containers (Docker / Compose)

Build any of the multi-stage targets (`base`, `iot`, `runtime`, `sdk`, `cloud`):

```sh
# Build and run the minimal EOS Base image
docker compose up -d eos-base

# Build and run the IoT, Runtime, SDK, or Cloud profiles
docker compose --profile iot up -d
docker compose --profile runtime up -d
docker compose --profile sdk up -d
docker compose --profile cloud up -d
```

Query the running **Synapse** supervisor locally inside the container, or deploy & hot-reload games/interactive apps remotely via `eos-devkit` (`libsockets` `:9460`):

```sh
# Local Unix pipe (/tmp/synapse.access)
docker exec eos-base synapse-client status
docker exec eos-base synapse-client list

# Remote TCP via libsockets (uv_sockets) or eos-devkit
synapse-client -h 127.0.0.1 -p 9460 status
./tools/python/devkit.py deploy ./dist/dinamo-steamdeck/ eos@steamdeck:/opt/eos/app/ --reload
```

### 2. Virtual Machines (Vagrant + Ansible)

Bring up any profile (`eos-base`, `eos-iot`, `eos-runtime`, `eos-sdk`, `eos-cloud`) with Docker or a VM provider (`virtualbox`, `libvirt`):

```sh
# Start the default base environment
vagrant up eos-base

# Start IoT, Runtime, SDK, or Cloud environments
vagrant up eos-iot
vagrant up eos-runtime
vagrant up eos-sdk
vagrant up eos-cloud

# Run Ansible provisioning against a VM
vagrant provision eos-runtime --provision-with ansible
```

Or run the Ansible playbooks directly against bare-metal Raspberry Pi (`armv6h`, `armv7h`, `aarch64`) or Cloud VMs:

```sh
ansible-playbook -i playbooks/inventory.ini playbooks/site.yml
```

### 3. Google Cloud Showcase (Terraform)

Provision the EOS Cloud & IoT Fleet Hub architecture on Google Cloud (Artifact Registry, VPC + zero-cost IAP TCP tunnels for `:22` and `:9460`, configurable `x86_64`/`arm64` Compute Engine VM, and optional scale-to-zero Cloud Run service). See [`solution-architecture-guide.md`](solution-architecture-guide.md) and [`terraform/README.md`](terraform/README.md):

```sh
cd terraform
cp terraform.tfvars.example terraform.tfvars
terraform init -backend=false
terraform validate
```

### 4. Native Arch Packages (`PKGBUILD`s)

Build and install `libsockets`, `synapse`, `elements`, `dinamo`, QEMU static user emulation, or `distccd` cross-compilers (`armv6h`, `armv7h`, `armv8`) directly on Arch Linux / SteamOS:

```sh
cd packages/libsockets && makepkg -si
cd ../synapse && makepkg -si
cd ../elements && makepkg -si
cd ../dinamo && makepkg -si
```
