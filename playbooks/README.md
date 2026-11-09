# EOS Ansible Playbooks

Declarative Ansible playbooks for provisioning EOS Virtual Machines, bare-metal Raspberry Pi devices (`armv6h`, `armv7h`, `aarch64`), and containers (`x86_64`, `aarch64`).

## Playbooks

- [`site.yml`](site.yml): Master playbook that orchestrates `base.yml`, `iot.yml`, `runtime.yml`, and `cloud.yml`.
- [`base.yml`](base.yml): Installs core packages (`packages/pkglists/base.txt`), compiles and installs [`libsockets`](https://github.com/williamlagos/libsockets), [`synapse`](https://github.com/williamlagos/synapse), and [`elements`](https://github.com/williamlagos/elements) via CMake, and configures the `synapse.service` systemd unit.
- [`iot.yml`](iot.yml): Installs IoT & edge sensor packages (`packages/pkglists/iot.txt`), deploys Python 3 hardware/sensor/network tools to `/opt/eos/bin/`, and configures Synapse IoT sensors.
- [`runtime.yml`](runtime.yml): Installs interactive SBC, A/V, and SDL2/KMSDRM/Vulkan runtime packages (`packages/pkglists/runtime.txt`), configures DRM/input/IIO/GPIO udev rules, links `eos-devkit`, and configures Synapse supervision for audio, sensors, and gamepads.
- [`cloud.yml`](cloud.yml): Installs Cloud server packages (`packages/pkglists/cloud.txt`) and configures Synapse supervision for Nginx, Redis, and `elements-server`.

## Usage

```sh
# Provision all inventory groups
ansible-playbook -i playbooks/inventory.ini playbooks/site.yml

# Provision only IoT nodes (e.g. Raspberry Pi armv6h/armv7h/aarch64)
ansible-playbook -i playbooks/inventory.ini playbooks/base.yml playbooks/iot.yml --limit eos_iot

# Provision only Cloud VMs
ansible-playbook -i playbooks/inventory.ini playbooks/base.yml playbooks/cloud.yml --limit eos_cloud
```
