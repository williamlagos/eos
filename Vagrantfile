# -*- mode: ruby -*-
# vi: set ft=ruby :

Vagrant.configure("2") do |config|
  # Default VM box when running with libvirt / virtualbox / vmware providers
  config.vm.box = "archlinux/archlinux"
  config.vm.synced_folder ".", "/vagrant"

  # ---------------------------------------------------------------------------
  # 1. eos-base (Default machine)
  # ---------------------------------------------------------------------------
  config.vm.define "eos-base", primary: true do |base|
    base.vm.hostname = "eos-base"
    base.vm.network "forwarded_port", guest: 9460, host: 9460, host_ip: "127.0.0.1", auto_correct: true

    base.vm.provider "docker" do |d|
      d.build_dir = "."
      d.build_args = ["--target", "base"]
      d.remains_running = true
    end

    base.vm.provider "virtualbox" do |vb|
      vb.memory = "512"
      vb.cpus = 1
    end

    base.vm.provision "ansible", run: "never" do |ansible|
      ansible.playbook = "playbooks/base.yml"
    end
  end

  # ---------------------------------------------------------------------------
  # 2. eos-iot (IoT & Edge profile)
  # ---------------------------------------------------------------------------
  config.vm.define "eos-iot", autostart: false do |iot|
    iot.vm.hostname = "eos-iot"
    iot.vm.network "forwarded_port", guest: 9460, host: 9461, host_ip: "127.0.0.1", auto_correct: true

    iot.vm.provider "docker" do |d|
      d.build_dir = "."
      d.build_args = ["--target", "iot"]
      d.remains_running = true
    end

    iot.vm.provider "virtualbox" do |vb|
      vb.memory = "512"
      vb.cpus = 1
    end

    iot.vm.provision "ansible", run: "never" do |ansible|
      ansible.playbook = "playbooks/site.yml"
      ansible.limit = "eos_iot"
    end
  end

  # ---------------------------------------------------------------------------
  # 3. eos-cloud (Cloud Application & Proxy profile)
  # ---------------------------------------------------------------------------
  config.vm.define "eos-cloud", autostart: false do |cloud|
    cloud.vm.hostname = "eos-cloud"
    cloud.vm.network "forwarded_port", guest: 80, host: 8080, host_ip: "127.0.0.1", auto_correct: true
    cloud.vm.network "forwarded_port", guest: 9460, host: 9462, host_ip: "127.0.0.1", auto_correct: true

    cloud.vm.provider "docker" do |d|
      d.build_dir = "."
      d.build_args = ["--target", "cloud"]
      d.remains_running = true
    end

    cloud.vm.provider "virtualbox" do |vb|
      vb.memory = "1024"
      vb.cpus = 2
    end

    cloud.vm.provision "ansible", run: "never" do |ansible|
      ansible.playbook = "playbooks/site.yml"
      ansible.limit = "eos_cloud"
    end
  end
end
