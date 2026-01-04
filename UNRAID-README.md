# Linux Thermaltake RGB - UNRAID Installation Guide

This guide will help you install the Thermaltake Riing RGB controller software on your UNRAID server.

## What's New in v1.4.0

- **NEW: UNRAID Dashboard Panel for real-time monitoring**
- Added REST API for status monitoring
- Improved device status reporting
- Enhanced troubleshooting capabilities
- Removed Docker dependencies for cleaner installation

## Installation Options

### Option 1: UNRAID Plugin (Recommended)

1. **Download the plugin file:**
   - Get `linux-thermaltake-rgb.plg` from the releases page

2. **Install in UNRAID:**
   - Go to UNRAID web UI → Settings → Plugins
   - Click "Install Plugin"
   - Browse to and select the `.plg` file
   - Click "Install"

3. **Configure your devices:**
   - Edit `/boot/config/plugins/linux-thermaltake-rgb/config/config.yml`
   - See configuration section below

4. **Start the service:**
   - The plugin will automatically start the service
   - Check status: `Settings → Users → Services` or run `systemctl status linux-thermaltake-rgb.service`

## Dashboard Panel Features

After installation, you'll see a new "Thermaltake RGB Controller" panel on your UNRAID dashboard that provides:

- **Real-time Status**: See if the service is running at a glance
- **Device Information**: View connected Thermaltake devices
- **Fan Speeds**: Monitor current fan speeds for all connected fans
- **Lighting Status**: See current lighting mode and colors
- **Service Controls**: Start/stop the service directly from the dashboard
- **Live Logs**: View recent service logs without leaving the dashboard
- **Quick Access**: Easy access to configuration and detailed logs

The dashboard updates automatically and provides a comprehensive view of your RGB controller setup.

### Option 2: Manual Installation

1. **Download the files:**
   ```bash
   wget https://github.com/chestm007/linux_thermaltake_riing/archive/master.zip
   unzip master.zip
   cd linux_thermaltake_riing-master/unraid
   ```

2. **Run the installation script:**
   ```bash
   chmod +x install.sh
   ./install.sh
   ```

## Configuration

Edit your configuration file at:
- Plugin: `/boot/config/plugins/linux-thermaltake-rgb/config/config.yml`
- Manual: `/etc/linux_thermaltake_rgb/config.yml`

### Basic Configuration Example

```yaml
controllers:
  - unit: 1
    type: g3
    devices:
      1: Riing Plus
      2: Riing Plus
      3: Riing Plus
      4: Riing Plus
      5: Floe Riing RGB

fan_manager:
  model: locked_speed
  speed: 50

lighting_manager:
  model: full
  r: 40
  g: 0
  b: 0
```

### Finding Temperature Sensors

Run this command in UNRAID terminal to find available temperature sensors:
```bash
sensors
```

Common sensor names in UNRAID:
- `coretemp-isa-0000` (Intel CPUs)
- `k10temp-pci-00c3` (AMD CPUs)
- `drivetemp-scsi-0-0` (Drive temperatures)

## Troubleshooting

### Service Won't Start

1. Check the logs:
   ```bash
   journalctl -u linux-thermaltake-rgb.service -f
   ```

2. Check USB device permissions:
   ```bash
   ls -la /dev/bus/usb/*/
   ```

3. Verify configuration:
   ```bash
   python3 -c "import yaml; yaml.safe_load(open('/etc/linux_thermaltake_rgb/config.yml'))"
   ```

### Devices Not Detected

1. Check if devices are connected:
   ```bash
   lsusb | grep 264d
   ```

2. Reload udev rules:
   ```bash
   sudo udevadm control --reload-rules
   sudo udevadm trigger
   ```

3. Check service logs for specific errors

### Fan Speed Not Changing

1. Verify the device type in config matches your hardware
2. Check that the controller unit number is correct
3. Ensure fans are properly connected to the controller

## Supported Devices

- Flow Riing RGB
- Lumi Plus LED Strip
- Pacific PR22-D5 Plus
- Pacific Rad Plus LED Panel
- Pacific V-GTX 1080Ti Plus GPU Waterblock
- Pacific W4 Plus CPU Waterblock
- Riing Plus
- Riing Trio

## Getting Help

- GitHub Issues: https://github.com/chestm007/linux_thermaltake_riing/issues
- UNRAID Forums: Search for "Thermaltake RGB" or create a new post

## Advanced Configuration

### Fan Control Modes

1. **Locked Speed**: Fixed fan speed
   ```yaml
   fan_manager:
     model: locked_speed
     speed: 50  # 0-100%
   ```

2. **Temperature Target**: Adjusts based on temperature
   ```yaml
   fan_manager:
     model: temp_target
     target: 38  # Target temperature in Celsius
     sensor_name: coretemp-isa-0000
     multiplier: 5
   ```

3. **Fan Curve**: Custom speed curve
   ```yaml
   fan_manager:
     model: curve
     points:
       - [0, 0]    # [Temp(°C), Speed(%)]
       - [50, 30]
       - [70, 100]
     sensor_name: coretemp-isa-0000
   ```

### Lighting Effects

Available lighting modes:
- `full`: Static color
- `flow`: RGB flow effect
- `spectrum`: Color spectrum
- `ripple`: Ripple effect
- `blink`: Blinking
- `pulse`: Pulsing
- `temperature`: Changes color based on temperature
- `alternating`: Alternates between two colors

Example:
```yaml
lighting_manager:
  model: pulse
  speed: normal  # slow, normal, fast, extreme
  r: 255
  g: 0
  b: 0
```
