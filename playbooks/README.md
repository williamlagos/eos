# EOS Ansible Playbooks

Declarative Ansible playbooks for provisioning EOS Virtual Machines, bare-metal Raspberry Pi devices (`armv6h`, `armv7h`, `aarch64`), and containers (`x86_64`, `aarch64`).

## Playbooks

- [`site.yml`](site.yml): Master playbook that orchestrates `base.yml`, `iot.yml`, and `cloud.yml`.
- [`base.yml`](base.yml): Installs core packages (`packages/pkglists/base.txt`), compiles and installs [`libsockets`](https://github.com/williamlagos/libsockets) and [`synapse`](https://github.com/williamlagos/synapse) via CMake, and configures the `synapse.service` systemd unit.
- [`iot.yml`](iot.yml): Installs IoT & edge packages (`packages/pkglists/iot.txt`), deploys Python 3 hardware/network tools to `/opt/eos/bin/`, and configures Synapse IoT sensors.
- [`cloud.yml`](cloud.yml): Installs Cloud server packages (`packages/pkglists/cloud.txt`) and configures Synapse supervision for Nginx and Redis.

## Usage

```sh
# Provision all inventory groups
ansible-playbook -i playbooks/inventory.ini playbooks/site.yml

# Provision only IoT nodes (e.g. Raspberry Pi armv6h/armv7h/aarch64)
ansible-playbook -i playbooks/inventory.ini playbooks/base.yml playbooks/iot.yml --limit eos_iot

# Provision only Cloud VMs
ansible-playbook -i playbooks/inventory.ini playbooks/base.yml playbooks/cloud.yml --limit eos_cloud
```
