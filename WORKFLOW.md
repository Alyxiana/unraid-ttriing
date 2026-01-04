# UNRAID Plugin Development Workflow

## Plugin File Creation Process

### 1. Create Plugin Structure
```xml
<!DOCTYPE PLUGIN [
<!ENTITY name "plugin-name">
<!ENTITY version "1.0.0">
<!ENTITY launch "Config.php?view=plugins&amp;name=plugin-name">
]>

<PLUGIN name="&name;" author="Author" version="&version;" launch="&launch;" min="6.9.0">
```

### 2. Add File Sections
For each file to be included:
```xml
<FILE Name="filename">
<URL>https://raw.githubusercontent.com/user/repo/branch/path/to/file</URL>
<MD5>lowercase_md5_hash</MD5>
<INSTALL>installation_commands</INSTALL>
</FILE>
```

### 3. Calculate Correct MD5 Hashes
**Critical: UNRAID expects lowercase MD5 hashes**

```bash
# Download the remote file
Invoke-WebRequest -Uri "https://raw.githubusercontent.com/user/repo/branch/path/to/file" -OutFile "temp_file.txt"

# Calculate lowercase MD5
(Get-FileHash -Path "temp_file.txt" -Algorithm MD5).Hash.ToLower()

# Clean up
Remove-Item temp_file.txt -Force
```

### 4. Installation Script
```xml
<INSTALL>
#!/bin/bash
# Installation commands here
</INSTALL>
```

### 5. Removal Script
```xml
<REMOVE>
#!/bin/bash
# Cleanup commands here
</REMOVE>
```

## Common Issues and Solutions

### MD5 Hash Mismatch
- **Problem**: "bad file MD5" error
- **Cause**: Using uppercase MD5 or local file hash instead of remote file hash
- **Solution**: Always calculate MD5 from the actual remote file and use lowercase

### XML Parsing Errors
- **Problem**: "XML file doesn't exist or xml parse error"
- **Cause**: Malformed XML, unclosed quotes, or self-referencing URLs
- **Solution**: Validate XML structure and avoid pluginURL self-references

### Missing Package Files
- **Problem**: "Invalid URL / Server error response" for txz files
- **Cause**: Referencing non-existent package files
- **Solution**: Install directly from source code or create proper packages

## Dashboard Panel Integration

### Dashboard File Location
- Path: `/usr/local/emhttp/plugins/plugin-name/dashboard.inc`
- Must be installed by the plugin
- PHP file with HTML, CSS, and JavaScript

### Dashboard Panel Requirements
1. **PHP Structure**: Use proper PHP tags and UNRAID styling
2. **API Integration**: Daemon must provide HTTP endpoints
3. **Permissions**: Ensure web server can access the panel
4. **Service Dependencies**: Panel should check if service is running

### Troubleshooting Dashboard Issues
1. **Check file installation**: Verify dashboard.inc exists in plugin directory
2. **Check service status**: Ensure daemon is running
3. **Check API endpoints**: Test HTTP server responses
4. **Check browser console**: Look for JavaScript errors
5. **Check UNRAID logs**: Review system and plugin logs

## Testing Process

### 1. Local Testing
- Validate XML syntax
- Check MD5 hashes
- Test installation script manually

### 2. UNRAID Testing
- Install plugin via web UI
- Check dashboard for panel appearance
- Verify service functionality
- Test removal process

### 3. Debugging Commands
```bash
# Check plugin files
ls -la /usr/local/emhttp/plugins/plugin-name/

# Check service status
systemctl status service-name

# Check logs
journalctl -u service-name -f

# Test API endpoints
curl http://localhost:port/status
```

## Best Practices

1. **Always use lowercase MD5 hashes**
2. **Test remote file URLs before adding to plugin**
3. **Include proper error handling in installation scripts**
4. **Use semantic versioning**
5. **Document all dashboard features**
6. **Test both installation and removal**
7. **Include comprehensive logging**
