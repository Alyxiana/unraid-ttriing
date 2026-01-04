<?php
/**
 * Linux Thermaltake RGB Dashboard Panel for UNRAID
 * Integrates with UNRAID's existing dashboard system
 */

// Plugin configuration
$plugin_name = 'linux-thermaltake-rgb';
$config_file = '/boot/config/plugins/' . $plugin_name . '/config/config.yml';

// Check if configuration exists
$config_exists = file_exists($config_file);

// Check if daemon process is running (using ps instead of systemctl)
$daemon_running = false;
$process_output = [];
exec('ps aux | grep "linux-thermaltake-rgb" | grep -v grep', $process_output);
if (!empty($process_output)) {
    $daemon_running = true;
}

// Get USB device info
$usb_devices = [];
exec('lsusb | grep 264d 2>/dev/null', $usb_devices);
$usb_count = count($usb_devices);

// Try to read configuration
$config_data = [];
if ($config_exists) {
    $config_content = file_get_contents($config_file);
    if ($config_content) {
        // Simple YAML parsing for basic info
        $lines = explode("\n", $config_content);
        foreach ($lines as $line) {
            if (strpos($line, 'controllers:') !== false) {
                $config_data['has_controllers'] = true;
            }
            if (strpos($line, 'fan_manager:') !== false) {
                $config_data['has_fan_manager'] = true;
            }
            if (strpos($line, 'lighting_manager:') !== false) {
                $config_data['has_lighting_manager'] = true;
            }
        }
    }
}

// Get recent logs from UNRAID log system
$logs = [];
$log_file = '/var/log/syslog';
if (file_exists($log_file)) {
    $command = "tail -n 5 " . escapeshellarg($log_file) . " | grep -i thermaltake";
    exec($command, $logs);
}
?>

<div class="panel">
    <div class="panel-header">
        <span class="panel-title">Thermaltake RGB Controller</span>
        <span class="panel-status <?php echo $daemon_running ? 'status-green' : 'status-red'; ?>">
            <?php echo $daemon_running ? 'Running' : 'Stopped'; ?>
        </span>
    </div>
    
    <div class="panel-body">
        <?php if (!$config_exists): ?>
            <div class="alert alert-warning">
                <i class="fa fa-exclamation-triangle"></i>
                Configuration not found. Please configure your devices.
            </div>
        <?php endif; ?>
        
        <!-- Status Overview -->
        <div class="status-overview">
            <div class="status-item">
                <strong>Service:</strong>
                <span class="<?php echo $daemon_running ? 'text-success' : 'text-danger'; ?>">
                    <?php echo $daemon_running ? 'Running' : 'Stopped'; ?>
                </span>
            </div>
            
            <div class="status-item">
                <strong>Devices:</strong>
                <span><?php echo $usb_count; ?> USB controllers detected</span>
            </div>
            
            <div class="status-item">
                <strong>Config:</strong>
                <span class="<?php echo $config_exists ? 'text-success' : 'text-warning'; ?>">
                    <?php echo $config_exists ? 'Found' : 'Missing'; ?>
                </span>
            </div>
        </div>
        
        <!-- Configuration Summary -->
        <?php if (!empty($config_data)): ?>
        <div class="config-summary">
            <h5>Configuration</h5>
            <div class="row">
                <?php if (isset($config_data['has_controllers'])): ?>
                <div class="col-sm-4">
                    <i class="fa fa-microchip text-info"></i> Controllers
                </div>
                <?php endif; ?>
                <?php if (isset($config_data['has_fan_manager'])): ?>
                <div class="col-sm-4">
                    <i class="fa fa-fan text-info"></i> Fan Control
                </div>
                <?php endif; ?>
                <?php if (isset($config_data['has_lighting_manager'])): ?>
                <div class="col-sm-4">
                    <i class="fa fa-lightbulb text-info"></i> Lighting
                </div>
                <?php endif; ?>
            </div>
        </div>
        <?php endif; ?>
        
        <!-- USB Devices -->
        <?php if (!empty($usb_devices)): ?>
        <div class="usb-devices">
            <h5>Connected Devices</h5>
            <div class="device-list">
                <?php foreach ($usb_devices as $device): ?>
                    <div class="device-item">
                        <code><?php echo htmlspecialchars(trim($device)); ?></code>
                    </div>
                <?php endforeach; ?>
            </div>
        </div>
        <?php endif; ?>
        
        <!-- Control Actions -->
        <div class="control-actions">
            <button class="btn btn-sm btn-default" onclick="openConfig()">
                <i class="fa fa-cog"></i> Configure
            </button>
            
            <button class="btn btn-sm btn-default" onclick="openLogs()">
                <i class="fa fa-list-alt"></i> View Logs
            </button>
            
            <?php if ($daemon_running): ?>
            <button class="btn btn-sm btn-warning" onclick="stopDaemon()">
                <i class="fa fa-stop"></i> Stop
            </button>
            <?php else: ?>
            <button class="btn btn-sm btn-success" onclick="startDaemon()">
                <i class="fa fa-play"></i> Start
            </button>
            <?php endif; ?>
        </div>
        
        <!-- Recent Logs -->
        <?php if (!empty($logs)): ?>
        <div class="recent-logs">
            <h5>Recent Logs</h5>
            <div class="log-viewer">
                <?php foreach ($logs as $log_line): ?>
                    <div class="log-line">
                        <?php echo htmlspecialchars(substr($log_line, 0, 100)); ?>
                    </div>
                <?php endforeach; ?>
            </div>
        </div>
        <?php endif; ?>
    </div>
</div>

<style>
.panel {
    background: #2c3e50;
    border: 1px solid #34495e;
    border-radius: 4px;
    margin-bottom: 20px;
}

.panel-header {
    padding: 10px 15px;
    border-bottom: 1px solid #34495e;
    display: flex;
    justify-content: space-between;
    align-items: center;
}

.panel-title {
    color: #ecf0f1;
    font-weight: bold;
}

.status-green {
    color: #27ae60;
}

.status-red {
    color: #e74c3c;
}

.panel-body {
    padding: 15px;
    color: #ecf0f1;
}

.alert {
    padding: 10px;
    border-radius: 3px;
    margin-bottom: 15px;
}

.alert-warning {
    background: #f39c12;
    color: #fff;
}

.status-overview {
    margin-bottom: 15px;
}

.status-item {
    margin-bottom: 5px;
}

.text-success {
    color: #27ae60;
}

.text-danger {
    color: #e74c3c;
}

.text-warning {
    color: #f39c12;
}

.text-info {
    color: #3498db;
}

.config-summary, .usb-devices, .recent-logs {
    margin-top: 15px;
}

.device-list, .log-viewer {
    background: #34495e;
    padding: 10px;
    border-radius: 3px;
    font-size: 12px;
    max-height: 150px;
    overflow-y: auto;
}

.device-item, .log-line {
    margin-bottom: 3px;
    word-break: break-all;
}

.control-actions {
    margin-top: 15px;
}

.control-actions .btn {
    margin-right: 5px;
}

h5 {
    color: #ecf0f1;
    margin-top: 0;
    margin-bottom: 10px;
    font-size: 14px;
}
</style>

<script>
function openConfig() {
    window.open('/Config.php?view=plugins&name=<?php echo $plugin_name; ?>', '_blank');
}

function openLogs() {
    window.open('/var/log/syslog', '_blank');
}

function startDaemon() {
    // Use UNRAID's exec API to start the daemon
    fetch('/plugins/dynamix.docker.manager/scripts/execute', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/x-www-form-urlencoded',
        },
        body: 'cmd=/usr/local/emhttp/plugins/dynamix.docker.manager/scripts/python /usr/local/bin/linux-thermaltake-rgb'
    })
    .then(response => response.text())
    .then(data => {
        setTimeout(() => location.reload(), 2000);
    })
    .catch(error => {
        console.error('Error starting daemon:', error);
    });
}

function stopDaemon() {
    // Use UNRAID's exec API to stop the daemon
    fetch('/plugins/dynamix.docker.manager/scripts/execute', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/x-www-form-urlencoded',
        },
        body: 'cmd=killall linux-thermaltake-rgb'
    })
    .then(response => response.text())
    .then(data => {
        setTimeout(() => location.reload(), 2000);
    })
    .catch(error => {
        console.error('Error stopping daemon:', error);
    });
}
</script>
