# Dashboard Panel Debugging Guide

## Quick Troubleshooting Steps

### 1. Check if Files Were Installed
```bash
# Check if dashboard.inc exists in the right location
ls -la /usr/local/emhttp/plugins/linux-thermaltake-rgb/dashboard.inc

# Check if the plugin directory exists
ls -la /usr/local/emhttp/plugins/linux-thermaltake-rgb/

# Check config directory
ls -la /boot/config/plugins/linux-thermaltake-rgb/
```

### 2. Check File Permissions
```bash
# Check dashboard.inc permissions
ls -la /usr/local/emhttp/plugins/linux-thermaltake-rgb/dashboard.inc

# Fix permissions if needed
chmod 644 /usr/local/emhttp/plugins/linux-thermaltake-rgb/dashboard.inc
chown root:root /usr/local/emhttp/plugins/linux-thermaltake-rgb/dashboard.inc
```

### 3. Test PHP Syntax
```bash
# Test if the PHP file has syntax errors
php -l /usr/local/emhttp/plugins/linux-thermaltake-rgb/dashboard.inc
```

### 4. Check UNRAID Web Server
```bash
# Restart UNRAID web interface
/etc/rc.d/rc.nginx restart

# Check nginx logs
tail -f /var/log/nginx/error.log

# Check for PHP errors
tail -f /var/log/php_errors.log
```

### 5. Manual Dashboard Test
Create a simple test to verify UNRAID can load PHP files:

```bash
# Create a simple test dashboard
cat > /usr/local/emhttp/plugins/linux-thermaltake-rgb/test-dashboard.inc << 'EOF'
<?php
echo "<div class='panel'>";
echo "<div class='panel-header'>";
echo "<span class='panel-title'>Test Panel</span>";
echo "</div>";
echo "<div class='panel-body'>";
echo "<p>Dashboard integration is working!</p>";
echo "<p>Time: " . date('Y-m-d H:i:s') . "</p>";
echo "</div>";
echo "</div>";
?>
EOF

# Rename to test it
mv /usr/local/emhttp/plugins/linux-thermaltake-rgb/dashboard.inc /usr/local/emhttp/plugins/linux-thermaltake-rgb/dashboard.inc.backup
mv /usr/local/emhttp/plugins/linux-thermaltake-rgb/test-dashboard.inc /usr/local/emhttp/plugins/linux-thermaltake-rgb/dashboard.inc
```

### 6. Check UNRAID Dashboard System
```bash
# Check if UNRAID recognizes the plugin
ls -la /usr/local/emhttp/plugins/

# Check UNRAID plugin registry
cat /usr/local/emhttp/plugins/linux-thermaltake-rgb/linux-thermaltake-rgb.plg 2>/dev/null || echo "Plugin file not found"

# Check if dashboard files are being loaded
grep -r "dashboard.inc" /var/log/nginx/ 2>/dev/null || echo "No dashboard references in logs"
```

### 7. Browser Debugging
1. Open UNRAID web interface
2. Press F12 to open developer tools
3. Go to Console tab - look for PHP/JavaScript errors
4. Go to Network tab - refresh dashboard and look for failed requests
5. Check if any requests are made to dashboard.inc

### 8. UNRAID Specific Issues
```bash
# Check UNRAID version compatibility
unraid-version

# Check if dynamix plugin is installed (required for dashboard)
ls -la /usr/local/emhttp/plugins/dynamix/

# Check UNRAID dashboard configuration
cat /usr/local/emhttp/plugins/dynamix/dynamix.cfg 2>/dev/null || echo "No dynamix config found"
```

## Common Issues and Solutions

### Issue: Dashboard Panel Not Appearing
**Possible Causes:**
- PHP syntax errors in dashboard.inc
- Wrong file permissions
- UNRAID not recognizing the plugin
- Missing dynamix plugin dependency

**Solutions:**
1. Test PHP syntax with `php -l`
2. Fix file permissions (644 for files, 755 for directories)
3. Restart UNRAID web interface
4. Verify dynamix plugin is installed

### Issue: Panel Shows but No Data
**Possible Causes:**
- Daemon not running
- Configuration file missing
- USB devices not detected

**Solutions:**
1. Check if daemon process is running: `ps aux | grep linux-thermaltake-rgb`
2. Verify config exists: `/boot/config/plugins/linux-thermaltake-rgb/config/config.yml`
3. Check USB devices: `lsusb | grep 264d`

### Issue: Start/Stop Buttons Not Working
**Possible Causes:**
- UNRAID exec API not accessible
- Wrong command syntax
- Permission issues

**Solutions:**
1. Test commands manually in terminal
2. Check UNRAID exec API permissions
3. Verify daemon binary exists

## Manual Installation Test

If automatic installation isn't working, try manual steps:

```bash
# Create directories
mkdir -p /usr/local/emhttp/plugins/linux-thermaltake-rgb
mkdir -p /boot/config/plugins/linux-thermaltake-rgb/config

# Copy files manually (adjust paths as needed)
cp /path/to/dashboard.inc /usr/local/emhttp/plugins/linux-thermaltake-rgb/
cp /path/to/config.yml /boot/config/plugins/linux-thermaltake-rgb/config/config.yml.example

# Set permissions
chmod 644 /usr/local/emhttp/plugins/linux-thermaltake-rgb/dashboard.inc
chown root:root /usr/local/emhttp/plugins/linux-thermaltake-rgb/dashboard.inc

# Restart web interface
/etc/rc.d/rc.nginx restart
```

## Getting Help

If none of these steps work:
1. Collect output from all diagnostic commands
2. Check UNRAID forums for similar issues
3. Provide:
   - UNRAID version
   - Plugin version
   - Error messages from logs
   - Browser console errors
