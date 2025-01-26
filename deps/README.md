# Dependency Installation Scripts

This folder contains operating system–specific scripts to **check, update, and install** all necessary dependencies for building and running the Quantum Assembly Simulator. We support:

1. **macOS** – `install_deps_macos.sh`
2. **Windows** – `install_deps_windows.ps1`
3. **Ubuntu Linux** – `install_deps_ubuntu.sh`
4. **Arch Linux** – `install_deps_arch.sh`
5. **Gentoo Linux** – `install_deps_gentoo.sh`
6. **Fedora Linux** – `install_deps_fedora.sh`

## Usage

1. **Navigate** to this `deps/` directory in a terminal or PowerShell (Windows).
2. **Run** the script for your operating system. For example, on Ubuntu:
   ```bash
   chmod +x install_deps_ubuntu.sh
   ./install_deps_ubuntu.sh

# How to Use
1. Set Permissions (on Unix-like systems)
   ```bash
   chmod +x deps/*.sh
   ```
2. Run the script corresponding to your OS. For example, on Fedora:
   ```bash
   cd deps
   sudo ./install_deps_fedora.sh
   ```
3. Verify installations with commands like gcc --version, nasm -v, etc.

On Windows, right-click on PowerShell and “Run as administrator”, then navigate to the deps/ folder and run:
   ```powershell
   .\install_deps_windows.ps1
   ```
