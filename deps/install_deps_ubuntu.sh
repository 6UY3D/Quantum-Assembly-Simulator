#!/usr/bin/env bash
#
# install_deps_ubuntu.sh
#
# Checks and installs essential dependencies on Ubuntu Linux using apt.

# You might need sudo
if [[ $EUID -ne 0 ]]; then
   echo "[Ubuntu] Please run as root (e.g., sudo ./install_deps_ubuntu.sh)."
   exit 1
fi

echo "[Ubuntu] Updating apt..."
apt update -y
apt upgrade -y

echo "[Ubuntu] Installing build essentials..."
apt install -y build-essential gcc g++ make cmake nasm git

echo "[Ubuntu] All set! Dependencies installed/updated."
