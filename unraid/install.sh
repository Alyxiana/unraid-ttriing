#!/bin/bash
# Installation script for Linux Thermaltake RGB on UNRAID
# This script installs the daemon and necessary dependencies

set -e

PLUGIN_NAME="unraid-ttriing"
VERSION="1.3.0"
PLUGIN_DIR="/usr/local/emhttp/plugins/${PLUGIN_NAME}"
CONFIG_DIR="/boot/config/plugins/${PLUGIN_NAME}"
SYSTEM_CONFIG_DIR="/etc/unraid_ttriing"

echo "Installing unraid-ttriing v${VERSION} for UNRAID..."
echo "=================================================="

# Check if running as root
if [ "$EUID" -ne 0 ]; then
    echo "Please run as root"
    exit 1
fi

# Check if UNRAID
if [ ! -f /etc/unraid-version ]; then
    echo "Warning: This doesn't appear to be an UNRAID system"
    read -p "Continue anyway? (y/N): " -n 1 -r
    echo
    if [[ ! $REPLY =~ ^[Yy]$ ]]; then
        exit 1
    fi
fi

# Create directories
echo "Creating directories..."
mkdir -p "${CONFIG_DIR}/config"
mkdir -p "${SYSTEM_CONFIG_DIR}"
mkdir -p "${PLUGIN_DIR}"

# Install Python dependencies if not already installed
echo "Checking Python dependencies..."
if ! /usr/local/emhttp/plugins/dynamix.docker.manager/scripts/python pip3 list | grep -q "pyyaml"; then
    echo "Installing Python dependencies..."
    /usr/local/emhttp/plugins/dynamix.docker.manager/scripts/python pip3 install pyyaml psutil pyusb numpy
fi

# Install the package
echo "Installing unraid-ttriing package..."
/usr/local/emhttp/plugins/dynamix.docker.manager/scripts/python pip3 install "unraid_ttriing==${VERSION}"

# Install udev rules
echo "Installing USB device rules..."
cp 99-thermaltake-usb.rules /etc/udev/rules.d/
udevadm control --reload-rules
udevadm trigger

# Install systemd service
echo "Installing systemd service..."
cp unraid-ttriing.service /usr/lib/systemd/system/
systemctl daemon-reload

# Install configuration
if [ ! -f "${CONFIG_DIR}/config/config.yml" ]; then
    echo "Installing default configuration..."
    cp ../unraid_ttriing/assets/config.yml "${CONFIG_DIR}/config/config.yml"
else
    echo "Existing configuration found, preserving it"
fi

# Create symlink for config
ln -sf "${CONFIG_DIR}/config/config.yml" "${SYSTEM_CONFIG_DIR}/config.yml"

# Enable and start service
echo "Enabling and starting service..."
systemctl enable unraid-ttriing.service
systemctl start unraid-ttriing.service

# Check status
sleep 2
if systemctl is-active --quiet unraid-ttriing.service; then
    echo "✓ Service is running successfully!"
else
    echo "✗ Service failed to start. Check logs with: journalctl -u unraid-ttriing.service"
fi

echo ""
echo "Installation complete!"
echo "Configuration file location: ${CONFIG_DIR}/config/config.yml"
echo "View service status: systemctl status unraid-ttriing.service"
echo "View logs: journalctl -u unraid-ttriing.service -f"
echo ""
echo "To configure your devices, edit the config file and restart the service:"
echo "  systemctl restart unraid-ttriing.service"
