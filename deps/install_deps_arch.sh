#!/usr/bin/env bash
#
# install_deps_arch.sh
#
# Installs essential dependencies on Arch Linux using pacman.

if [[ $EUID -ne 0 ]]; then
   echo "[Arch] Please run as root (e.g., sudo ./install_deps_arch.sh)."
   exit 1
fi

echo "[Arch] Updating package database..."
pacman -Syu --noconfirm

echo "[Arch] Installing dependencies..."
pacman -S --noconfirm base-devel gcc make cmake nasm git

echo "[Arch] All set! Dependencies installed/updated."
