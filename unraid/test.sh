#!/bin/bash
# Test script for Linux Thermaltake RGB on UNRAID

echo "Testing unraid-ttriing installation..."
echo "=========================================="

# Check if service is running
if systemctl is-active --quiet unraid-ttriing.service; then
    echo "✓ Service is running"
else
    echo "✗ Service is not running"
    echo "Trying to start service..."
    sudo systemctl start unraid-ttriing.service
    sleep 2
    if systemctl is-active --quiet unraid-ttriing.service; then
        echo "✓ Service started successfully"
    else
        echo "✗ Service failed to start"
        echo "Checking logs:"
        sudo journalctl -u unraid-ttriing.service --no-pager -n 20
        exit 1
    fi
fi

# Check USB devices
echo ""
echo "Checking for Thermaltake USB devices..."
if lsusb | grep -q "264d"; then
    echo "✓ Thermaltake devices found:"
    lsusb | grep "264d"
else
    echo "⚠ No Thermaltake devices detected"
    echo "  Make sure your controller is connected"
fi

# Check configuration
echo ""
echo "Checking configuration..."
if [ -f /boot/config/plugins/unraid-ttriing/config/config.yml ]; then
    echo "✓ Configuration file found at /boot/config/plugins/unraid-ttriing/config/config.yml"
elif [ -f /etc/unraid_ttriing/config.yml ]; then
    echo "✓ Configuration file found at /etc/unraid_ttriing/config.yml"
    if [ -L /etc/unraid_ttriing/config.yml ]; then
        echo "  → Linked to: $(readlink /etc/unraid_ttriing/config.yml)"
    fi
else
    echo "✗ Configuration file not found"
    exit 1
fi

# Check Python module
echo ""
echo "Checking Python module..."
if /usr/local/emhttp/plugins/dynamix.docker.manager/scripts/python -c "import unraid_ttriing" 2>/dev/null; then
    echo "✓ Python module installed successfully"
else
    echo "✗ Python module not found"
    exit 1
fi

# Test temperature sensors
echo ""
echo "Available temperature sensors:"
sensors 2>/dev/null | grep -E "(Adapter|Core|Package|temp)" | head -20

echo ""
echo "Test complete! If everything looks good, you can now:"
echo "1. Edit your configuration: /boot/config/plugins/linux-thermaltake-rgb/config/config.yml"
echo "2. Restart the service: sudo systemctl restart linux-thermaltake-rgb.service"
echo "3. Monitor logs: sudo journalctl -u linux-thermaltake-rgb.service -f"
