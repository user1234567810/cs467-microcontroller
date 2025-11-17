FROM mcr.microsoft.com/devcontainers/base:ubuntu

RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y \
        git tar build-essential cmake ninja-build \
    && rm -rf /var/lib/apt/lists/*