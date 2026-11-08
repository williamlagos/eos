# syntax=docker/dockerfile:1
ARG BASE_IMAGE=archlinux:latest

# =============================================================================
# Stage 1: Builder — Compiles libsockets & synapse from upstream repositories
# =============================================================================
FROM ${BASE_IMAGE} AS builder

ARG LIBSOCKETS_REPO=https://github.com/williamlagos/libsockets.git
ARG LIBSOCKETS_REF=master
ARG SYNAPSE_REPO=https://github.com/williamlagos/synapse.git
ARG SYNAPSE_REF=master
ARG ELEMENTS_REPO=https://github.com/williamlagos/elements.git
ARG ELEMENTS_REF=master

RUN pacman -Syu --noconfirm --needed \
        base-devel \
        cmake \
        git \
        libuv \
        pkgconf \
        pybind11 \
        python \
    && pacman -Scc --noconfirm

WORKDIR /src

# Build and install libsockets (libuv_sockets + legacy libsockets)
RUN git clone --branch "${LIBSOCKETS_REF}" --depth 1 "${LIBSOCKETS_REPO}" libsockets \
    && cmake -B libsockets/build -S libsockets \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX=/usr \
        -DSOCKETS_BUILD_EXAMPLES=OFF \
    && cmake --build libsockets/build --parallel \
    && cmake --install libsockets/build \
    && DESTDIR=/opt/eos-dist cmake --install libsockets/build

# Build and install synapse (linked against libsockets + libuv)
RUN git clone --branch "${SYNAPSE_REF}" --depth 1 "${SYNAPSE_REPO}" synapse \
    && cmake -B synapse/build -S synapse \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX=/usr \
        -DCMAKE_INSTALL_SYSCONFDIR=/etc \
    && cmake --build synapse/build --parallel \
    && DESTDIR=/opt/eos-dist cmake --install synapse/build

# Build and install elements (C++17 elements_core + pybind11 _elements bindings)
RUN git clone --branch "${ELEMENTS_REF}" --depth 1 "${ELEMENTS_REPO}" elements \
    && cmake -B elements/build -S elements \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX=/usr \
    && cmake --build elements/build --parallel \
    && DESTDIR=/opt/eos-dist cmake --install elements/build \
    && PY_SITE="$(python -c 'import site; print(site.getsitepackages()[0])')" \
    && mkdir -p "/opt/eos-dist${PY_SITE}" \
    && if compgen -G "elements/build/_elements*.so" > /dev/null; then \
         cp elements/build/_elements*.so "/opt/eos-dist${PY_SITE}/"; \
       fi

# =============================================================================
# Stage 2: EOS Base — Minimal runtime with Synapse + libsockets supervisor
# =============================================================================
FROM ${BASE_IMAGE} AS base

LABEL org.opencontainers.image.title="EOS Base" \
      org.opencontainers.image.description="Arch Linux base image with Synapse + libsockets supervisor for IoT & Cloud" \
      org.opencontainers.image.source="https://github.com/williamlagos/eos"

RUN pacman -Syu --noconfirm --needed \
        ca-certificates \
        curl \
        iproute2 \
        iputils \
        libuv \
        openssl \
        procps-ng \
        python \
        sudo \
    && pacman -Scc --noconfirm

COPY --from=builder /opt/eos-dist/ /
COPY packages/synapse/synapse.service /usr/lib/systemd/system/synapse.service

EXPOSE 9460/tcp

HEALTHCHECK --interval=15s --timeout=5s --start-period=5s --retries=3 \
    CMD ["/usr/bin/synapse-client", "ping"]

ENTRYPOINT ["/usr/bin/synapse", "-f", "-c", "/etc/synapse/default.cfg", "-l", "-", "-p", "9460"]

# =============================================================================
# Stage 3: EOS IoT — Edge & Raspberry Pi (armv6h / armv7h / aarch64 / x86_64)
# =============================================================================
FROM base AS iot

LABEL org.opencontainers.image.title="EOS IoT" \
      org.opencontainers.image.description="EOS IoT & Edge container image with hardware/network scripts and Synapse sensor supervision"

RUN pacman -Syu --noconfirm --needed \
        alsa-utils \
        avahi \
        bluez \
        bluez-utils \
        dhcpcd \
        hostapd \
        i2c-tools \
        iio-sensor-proxy \
        iw \
        libgpiod \
        net-tools \
        networkmanager \
        nss-mdns \
        python-dbus \
        python-evdev \
        python-gobject \
        python-pyudev \
        python-smbus \
        usbutils \
        wireless_tools \
        wpa_supplicant \
    && pacman -Scc --noconfirm

COPY tools/python/ /opt/eos/bin/
RUN chmod +x /opt/eos/bin/*.py \
    && ln -sf /opt/eos/bin/devkit.py /usr/local/bin/eos-devkit \
    && printf "[network]\ncommand=/usr/bin/python3 /opt/eos/bin/ssids.py\nstate=test -d /sys/class/net\n\n[audio]\ncommand=/usr/bin/python3 /opt/eos/bin/sound.py\nstate=test -d /dev/snd\n\n[sensors]\ncommand=/usr/bin/python3 /opt/eos/bin/sensors.py --once\nstate=/usr/bin/python3 /opt/eos/bin/sensors.py --check\n" > /etc/synapse/default.cfg

ENV PATH="/opt/eos/bin:${PATH}"

# =============================================================================
# Stage 4: EOS Runtime — Interactive SBCs, A/V, Steam Deck & Game Runtime
# =============================================================================
FROM iot AS runtime

LABEL org.opencontainers.image.title="EOS Runtime" \
      org.opencontainers.image.description="EOS Interactive, A/V, and SDL2/KMSDRM/Vulkan game runtime for Steam Deck, Raspberry Pi, and SBCs"

RUN pacman -Syu --noconfirm --needed \
        evtest \
        fbi \
        gamescope \
        hidapi \
        libdrm \
        libpng \
        mesa \
        mgba-sdl \
        pipewire \
        pipewire-alsa \
        pipewire-pulse \
        python-pygame \
        sdl2 \
        vulkan-icd-loader \
        wayland \
        wireplumber \
    && pacman -Scc --noconfirm \
    && printf "[network]\ncommand=/usr/bin/python3 /opt/eos/bin/ssids.py\nstate=test -d /sys/class/net\n\n[audio]\ncommand=/usr/bin/python3 /opt/eos/bin/sound.py\nstate=test -d /dev/snd\n\n[sensors]\ncommand=/usr/bin/python3 /opt/eos/bin/sensors.py --once\nstate=/usr/bin/python3 /opt/eos/bin/sensors.py --check\n\n[gamepad]\ncommand=/usr/bin/python3 /opt/eos/bin/gamepad.py\nstate=test -d /dev/input\n" > /etc/synapse/default.cfg

# =============================================================================
# Stage 5: EOS SDK — Multi-Arch Game, Interactive & IoT Development Kit
# =============================================================================
FROM runtime AS sdk

LABEL org.opencontainers.image.title="EOS SDK" \
      org.opencontainers.image.description="EOS Multi-Arch C++17/Python/SDL2 SDK with Elements Core, CMake, Ninja, Clang, and ARM cross-toolchains"

RUN pacman -Syu --noconfirm --needed \
        base-devel \
        clang \
        cmake \
        distcc \
        gdb \
        git \
        glslang \
        lld \
        ninja \
        pkgconf \
        pybind11 \
        python-pip \
        python-pytest \
        python-setuptools \
        rsync \
        uv \
        vulkan-headers \
    && pacman -Scc --noconfirm

COPY tools/cross/ /opt/eos/cross/

# =============================================================================
# Stage 6: EOS Cloud — Cloud Application, Multiplayer & Microservice Runtime
# =============================================================================
FROM base AS cloud

LABEL org.opencontainers.image.title="EOS Cloud" \
      org.opencontainers.image.description="EOS Cloud container image with Nginx, Redis, Node.js, Python/uv, Elements C++ core, and Synapse supervision"

RUN pacman -Syu --noconfirm --needed \
        logrotate \
        nginx \
        nodejs \
        npm \
        python-pip \
        redis \
        rsync \
        uv \
    && pacman -Scc --noconfirm \
    && printf "[nginx]\ncommand=/usr/bin/nginx -g daemon\\ off;\nstate=test -f /etc/nginx/nginx.conf\n\n[redis]\ncommand=/usr/bin/redis-server /etc/redis/redis.conf\nstate=test -f /etc/redis/redis.conf\n" > /etc/synapse/default.cfg

EXPOSE 80/tcp 443/tcp 8000/tcp 9460/tcp
