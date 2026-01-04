# unraid-ttriing - UNRAID Installation Guide

This guide will help you install the Thermaltake Riing RGB controller software on your UNRAID server.

**Maintained by [Alyxiana](https://github.com/Alyxiana) - Original work by [Max Chesterfield](https://github.com/chestm007)**

## What's New in v1.3.0

- Updated for UNRAID compatibility
- Removed GObject dependency (no longer required)
- USB device permissions automatically configured
- Configuration stored on flash drive for persistence
- Docker alternative for easier deployment

## Installation Options

### Option 1: UNRAID Plugin (Recommended)

1. **Download the plugin file:**
   - Get `unraid-ttriing.plg` from the releases page

2. **Install in UNRAID:**
   - Go to UNRAID web UI → Settings → Plugins
   - Click "Install Plugin"
   - Browse to and select the `.plg` file
   - Click "Install"

3. **Configure your devices:**
   - Edit `/boot/config/plugins/unraid-ttriing/config/config.yml`
   - See configuration section below

4. **Start the service:**
   - The plugin will automatically start the service
   - Check status: `Settings → Users → Services` or run `systemctl status unraid-ttriing.service`

### Option 2: Manual Installation

1. **Download the files:**
   ```bash
   wget https://github.com/Alyxiana/unraid-ttriing/archive/master.zip
   unzip master.zip
   cd unraid-ttriing-master/unraid
   ```

2. **Run the installation script:**
   ```bash
   chmod +x install.sh
   ./install.sh
   ```

## Configuration

The daemon automatically detects configuration in this order:
1. **UNRAID Flash Storage** (preferred): `/boot/config/plugins/unraid-ttriing/config/config.yml`
2. **System Config**: `/etc/unraid_ttriing/config.yml` (symlink from UNRAID location)
3. **Environment Variable**: `THERMALTAKE_CONFIG_DIR` (if set)
4. **Repository Config**: `unraid_ttriing/assets/config.yml` (fallback)

**For UNRAID installations**, always edit: `/boot/config/plugins/unraid-ttriing/config/config.yml`

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
   journalctl -u unraid-ttriing.service -f
   ```

2. Check USB device permissions:
   ```bash
   ls -la /dev/bus/usb/*/
   ```

3. Verify configuration:
   ```bash
   python3 -c "import yaml; yaml.safe_load(open('/boot/config/plugins/unraid-ttriing/config/config.yml'))"
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
