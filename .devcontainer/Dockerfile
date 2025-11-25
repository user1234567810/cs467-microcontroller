# .devcontainer/Dockerfile
FROM mcr.microsoft.com/devcontainers/base:ubuntu

# Install Pico SDK toolchain + build tools
RUN apt-get update && apt-get install -y \
    gcc-arm-none-eabi \
    libnewlib-arm-none-eabi \
    cmake \
    build-essential \
    ninja-build \
    python3 python3-pip python3-venv \
    git \
 && rm -rf /var/lib/apt/lists/*

# Clone the Raspberry Pi Pico SDK into a fixed location in the image
RUN git clone -b master https://github.com/raspberrypi/pico-sdk.git /opt/pico-sdk && \
    cd /opt/pico-sdk && \
    git submodule update --init