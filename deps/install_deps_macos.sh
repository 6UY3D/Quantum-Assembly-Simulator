#!/usr/bin/env bash
#
# install_deps_macos.sh
#
# Checks and installs essential dependencies on macOS using Homebrew.
# Usage:
#   ./install_deps_macos.sh

# Verify if Homebrew is installed
if ! command -v brew &> /dev/null
then
    echo "[macOS] Homebrew not found. Installing Homebrew..."
    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
    # After installation, ensure brew is in PATH
    eval "$(/opt/homebrew/bin/brew shellenv)" 2>/dev/null || true
    eval "$(/usr/local/bin/brew shellenv)" 2>/dev/null || true
fi

echo "[macOS] Updating Homebrew..."
brew update

echo "[macOS] Installing/upgrading packages..."
brew install \
    gcc \
    make \
    nasm \
    cmake \
    git

# You may optionally install clang if needed (it's often included with Xcode Command Line Tools).
# brew install llvm

echo "[macOS] All set! Dependencies installed/updated."
