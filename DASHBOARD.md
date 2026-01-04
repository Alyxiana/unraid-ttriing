# UNRAID Dashboard Panel

The Linux Thermaltake RGB plugin includes a comprehensive dashboard panel for UNRAID that provides real-time monitoring and control of your Thermaltake RGB devices.

## Features

### Real-time Status Monitoring
- Visual status indicator showing if the daemon is running (green) or stopped (red)
- Automatic refresh of device status
- Service uptime information

### Device Information
- Lists all connected Thermaltake USB controllers
- Shows device types and connection details
- Displays number of devices per controller

### Fan Control Status
- Real-time fan speed display for all connected fans
- Shows current speed as percentage
- Identifies fans by controller unit and port number

### Lighting Information
- Current lighting mode (e.g., full, pulse, flow, spectrum)
- RGB color values when applicable
- Speed settings for dynamic effects

### Service Controls
- Start/Stop service buttons directly from dashboard
- View service logs without leaving the dashboard
- Quick access to configuration page

## Accessing the Dashboard

1. Install the plugin using the `.plg` file
2. Navigate to your UNRAID Dashboard tab
3. Look for the "Thermaltake RGB Controller" panel
4. The panel will appear automatically after installation

## Troubleshooting Dashboard Issues

### Panel Not Showing
- Verify the plugin is installed: Settings → Plugins
- Check if the service is running: Settings → Users → Services
- Refresh your browser cache

### Status Not Updating
- Check if the HTTP server is running on port 5334
- Verify firewall isn't blocking localhost connections
- Check service logs for errors

### No Device Information
- Verify USB devices are connected
- Check udev rules are properly installed
- Run `lsusb | grep 264d` to verify device detection

## API Endpoints

The dashboard uses the following API endpoints provided by the daemon:

### GET /status
Returns complete status information in JSON format:
```json
{
  "timestamp": 1641234567,
  "running": true,
  "controllers": [
    {
      "unit": 1,
      "type": "g3",
      "devices": [
        {
          "id": "1",
          "model": "Riing Plus",
          "type": "fan"
        }
      ]
    }
  ],
  "fans": [
    {
      "id": "1:1",
      "model": "Riing Plus",
      "speed": 75
    }
  ],
  "lighting": {
    "mode": "pulse",
    "color": {
      "r": 255,
      "g": 0,
      "b": 0
    }
  }
}
```

### GET /health
Simple health check for monitoring systems:
```json
{
  "status": "ok"
}
```

## Customization

The dashboard panel can be customized by editing `/usr/local/emhttp/plugins/linux-thermaltake-rgb/dashboard.inc` on your UNRAID server.

### Adding New Features
The panel is built with PHP, HTML, CSS, and JavaScript. You can:
- Add new status displays
- Implement control buttons
- Create custom visualizations
- Add alerting capabilities

### Styling
The panel uses UNRAID's color scheme:
- Background: `#2c3e50`
- Cards: `#34495e`
- Text: `#ecf0f1`
- Success: `#27ae60`
- Danger: `#e74c3c`
- Info: `#3498db`

## Security Considerations

- The HTTP server only listens on localhost (127.0.0.1)
- No authentication is required for local access
- CORS headers allow access from UNRAID web interface
- No sensitive configuration is exposed through the API

## Performance Impact

- The HTTP server runs in a lightweight thread
- Status requests are handled asynchronously
- Minimal CPU usage (< 1%)
- Memory usage typically < 10MB
