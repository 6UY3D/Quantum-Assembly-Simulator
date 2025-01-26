#!/usr/bin/env bash
#
# install_deps_fedora.sh
#
# Installs essential dependencies on Fedora Linux using dnf.

if [[ $EUID -ne 0 ]]; then
   echo "[Fedora] Please run as root (e.g., sudo ./install_deps_fedora.sh)."
   exit 1
fi

echo "[Fedora] Updating system..."
dnf -y update

echo "[Fedora] Installing dependencies..."
dnf -y install gcc gcc-c++ make cmake nasm git

echo "[Fedora] All set! Dependencies installed/updated."
