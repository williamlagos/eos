# EOS

**EOS** is a modular GNU/Linux base platform for **IoT / Edge devices** and **Cloud VMs & Containers**, built on **Arch Linux** (`x86_64`) and **Arch Linux ARM** (`armv6h` for Raspberry Pi 1/Zero, `armv7h` for Pi 2/3, and `aarch64` for Pi 3/4/5 and ARM Cloud).

At its core, EOS unifies three projects into a cohesive runtime:

1. **EOS (`eos`)** — Multi-stage container definitions (`Dockerfile`, `docker-compose.yml`), multi-machine VM topologies (`Vagrantfile`), declarative Ansible provisioning (`playbooks/`), Arch `PKGBUILD`s (`packages/`), and Python 3 edge/hardware tools (`tools/python/`).
2. **[Synapse](https://github.com/williamlagos/synapse)** — Lightweight reactive process supervisor and sensor engine written in C11 on top of `libuv`. Acts as PID 1 / service supervisor in containers or a systemd-managed node agent in VMs.
3. **[libsockets](https://github.com/williamlagos/libsockets)** — Asynchronous, non-blocking IPv4/IPv6 TCP socket library (`libuv_sockets` / `uv_sockets.h`) integrated into Synapse to provide remote TCP fleet control (`:9460`) alongside local Unix domain socket IPC (`/tmp/synapse.access`).

---

## Architecture & Profiles

| Profile | Target Stage / Machine | Purpose | Included Components |
|---|---|---|---|
| **Base** | `base` / `eos-base` | Minimal supervisor base image / VM | Arch Linux base, `libuv`, `libsockets`, `synapse`, `synapse-client`, `libimpulse.so` |
| **IoT** | `iot` / `eos-iot` | Edge gateways & Raspberry Pi (`armv6h`+) | Base + `NetworkManager`, `avahi`, `bluez`, `i2c-tools`, `pyudev`, `/opt/eos/bin/*.py` |
| **Cloud** | `cloud` / `eos-cloud` | Cloud microservices & reverse proxy | Base + `nginx`, `redis`, `nodejs`, `python`, `tools/markt` stack |

---

## Quick Start

### 1. Containers (Docker / Compose)

Build any of the multi-stage targets (`base`, `iot`, `cloud`):

```sh
# Build and run the minimal EOS Base image
docker compose up -d eos-base

# Build and run the IoT or Cloud profiles
docker compose --profile iot up -d
docker compose --profile cloud up -d
```

Query the running **Synapse** supervisor locally inside the container or remotely over TCP (`libsockets`):

```sh
# Local Unix pipe (/tmp/synapse.access)
docker exec eos-base synapse-client status
docker exec eos-base synapse-client list

# Remote TCP via libsockets (uv_sockets)
synapse-client -h 127.0.0.1 -p 9460 status
synapse-client -h 127.0.0.1 -p 9460 ping
```

### 2. Virtual Machines (Vagrant + Ansible)

Bring up any profile (`eos-base`, `eos-iot`, `eos-cloud`) with Docker or a VM provider (`virtualbox`, `libvirt`):

```sh
# Start the default base environment
vagrant up eos-base

# Start IoT or Cloud environments
vagrant up eos-iot
vagrant up eos-cloud

# Run Ansible provisioning against a VM
vagrant provision eos-iot --provision-with ansible
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

Build and install `libsockets`, `synapse`, QEMU static user emulation, or `distccd` cross-compilers (`armv6h`, `armv7h`, `armv8`) directly on Arch Linux:

```sh
cd packages/libsockets && makepkg -si
cd ../synapse && makepkg -si
```
