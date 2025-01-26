#!/usr/bin/env bash
#
# install_deps_gentoo.sh
#
# Installs essential dependencies on Gentoo Linux using emerge.

if [[ $EUID -ne 0 ]]; then
   echo "[Gentoo] Please run as root (e.g., sudo ./install_deps_gentoo.sh)."
   exit 1
fi

echo "[Gentoo] Syncing Portage tree..."
emerge --sync

echo "[Gentoo] Updating packages..."
emerge -uDNav @world

echo "[Gentoo] Installing required packages..."
emerge -av sys-devel/gcc sys-devel/make dev-util/cmake dev-lang/nasm dev-vcs/git

echo "[Gentoo] All set! Dependencies installed/updated."
