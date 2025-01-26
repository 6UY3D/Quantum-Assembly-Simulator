<#
.SYNOPSIS
    install_deps_windows.ps1

.DESCRIPTION
    This script checks for the existence of Chocolatey on Windows,
    installs it if missing, and then installs or updates dependencies
    needed for the Quantum Assembly Simulator.

    Usage:
        .\install_deps_windows.ps1
#>

# Check if running as Administrator
If (-Not ([Security.Principal.WindowsPrincipal] [Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltinRole] "Administrator")) {
    Write-Host "[Windows] Please run this script from an elevated PowerShell (Run as Administrator)."
    Exit 1
}

# Check if Chocolatey is installed
if (-not (Get-Command choco -ErrorAction SilentlyContinue)) {
    Write-Host "[Windows] Chocolatey not found. Installing Chocolatey..."
    Set-ExecutionPolicy Bypass -Scope Process -Force
    [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072
    Invoke-Expression (New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1')
}

Write-Host "[Windows] Upgrading Chocolatey..."
choco upgrade chocolatey -y

Write-Host "[Windows] Installing/upgrading dependencies..."
choco install -y `
    visualstudio2022buildtools `
    gcc-arm-embedded `
    nasm `
    cmake `
    git `
    make

Write-Host "[Windows] All set! Dependencies installed/updated."
