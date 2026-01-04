# Dashboard Panel Troubleshooting

## Issue: Dashboard Panel Not Showing

### Step 1: Verify Plugin Installation
Check if the plugin files were installed correctly:

```bash
# Check if dashboard.inc exists
ls -la /usr/local/emhttp/plugins/linux-thermaltake-rgb/dashboard.inc

# Check plugin directory
ls -la /usr/local/emhttp/plugins/linux-thermaltake-rgb/

# Check config files
ls -la /boot/config/plugins/linux-thermaltake-rgb/
```

### Step 2: Check Service Status
Verify the daemon is running:

```bash
# Check service status
systemctl status linux-thermaltake-rgb.service

# Check if service is enabled
systemctl is-enabled linux-thermaltake-rgb.service

# View service logs
journalctl -u linux-thermaltake-rgb.service -n 20
```

### Step 3: Test HTTP Server
Check if the status API is working:

```bash
# Test status endpoint
curl http://localhost:5334/status

# Test health endpoint
curl http://localhost:5334/health

# Check if port is listening
netstat -tlnp | grep 5334
```

### Step 4: Check UNRAID Dashboard
Debug the dashboard integration:

```bash
# Check UNRAID web server logs
tail -f /var/log/nginx/access.log

# Check for PHP errors
tail -f /var/log/php_errors.log

# Check UNRAID system logs
tail -f /var/log/syslog | grep -i thermaltake
```

### Step 5: Manual Dashboard Test
Test the dashboard PHP file directly:

```bash
# Test PHP syntax
php -l /usr/local/emhttp/plugins/linux-thermaltake-rgb/dashboard.inc

# Check file permissions
ls -la /usr/local/emhttp/plugins/linux-thermaltake-rgb/dashboard.inc
```

### Step 6: Browser Debugging
Check browser console for errors:

1. Open UNRAID web UI
2. Press F12 to open developer tools
3. Go to Console tab
4. Look for JavaScript errors
5. Go to Network tab and refresh dashboard
6. Look for failed requests

### Step 7: Restart Services
Try restarting relevant services:

```bash
# Restart UNRAID web GUI
/etc/rc.d/rc.nginx restart

# Restart the daemon
systemctl restart linux-thermaltake-rgb.service

# Restart UNRAID (if necessary)
reboot
```

## Common Dashboard Issues

### 1. File Permissions
```bash
# Fix permissions if needed
chmod 644 /usr/local/emhttp/plugins/linux-thermaltake-rgb/dashboard.inc
chown root:root /usr/local/emhttp/plugins/linux-thermaltake-rgb/dashboard.inc
```

### 2. Missing Dependencies
```bash
# Check if required PHP modules are installed
php -m | grep -E "(curl|json)"

# Install missing modules if needed
# (UNRAID usually includes these by default)
```

### 3. Service Not Running
```bash
# Start the service manually
systemctl start linux-thermaltake-rgb.service

# Enable auto-start
systemctl enable linux-thermaltake-rgb.service
```

### 4. Port Conflicts
```bash
# Check if port 5334 is in use
netstat -tlnp | grep 5334

# Kill conflicting processes if needed
sudo kill -9 <PID>
```

## Manual Dashboard Test

Create a simple test file to verify dashboard integration:

```bash
# Create test dashboard
cat > /usr/local/emhttp/plugins/linux-thermaltake-rgb/test-dashboard.inc << 'EOF'
<?php
echo "<div class='panel'>";
echo "<div class='panel-header'>";
echo "<span class='panel-title'>Test Panel</span>";
echo "</div>";
echo "<div class='panel-body'>";
echo "<p>If you can see this, dashboard integration is working!</p>";
echo "</div>";
echo "</div>";
?>
EOF

# Test in browser by navigating to UNRAID dashboard
```

## Getting Help

If the panel still doesn't show after these steps:

1. Collect logs from all the above commands
2. Check UNRAID forums for similar issues
3. Create a GitHub issue with:
   - Plugin version
   - UNRAID version
   - Error logs
   - Steps taken

## Expected Behavior

When working correctly:
- Dashboard panel appears on main UNRAID dashboard
- Shows "Thermaltake RGB Controller" title
- Displays service status (green/red indicator)
- Shows device information when service is running
- Provides start/stop controls
- Displays recent logs
