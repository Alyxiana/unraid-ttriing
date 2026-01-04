<?php
/**
 * Linux Thermaltake RGB Dashboard Panel for UNRAID
 * Displays real-time status of Thermaltake RGB controllers, fans, and lighting
 */

// Plugin configuration
$plugin_name = 'linux-thermaltake-rgb';
$config_file = '/boot/config/plugins/' . $plugin_name . '/config/config.yml';
$service_name = 'linux-thermaltake-rgb.service';

// Check if service is running
$service_status = shell_exec('systemctl is-active ' . $service_name . ' 2>/dev/null');
$service_enabled = shell_exec('systemctl is-enabled ' . $service_name . ' 2>/dev/null');

// Get daemon status information
$status_data = [];
$daemon_running = trim($service_status) === 'active';

if ($daemon_running) {
    // Try to get status from the daemon (we'll need to add a status endpoint)
    $status_json = shell_exec('curl -s --max-time 2 http://localhost:5334/status 2>/dev/null');
    if ($status_json) {
        $status_data = json_decode($status_json, true) ?: [];
    }
    
    // Get USB device info
    $usb_devices = shell_exec('lsusb | grep 264d 2>/dev/null');
    
    // Get service logs (last 5 lines)
    $logs = shell_exec('journalctl -u ' . $service_name . ' --no-pager -n 5 --output=cat 2>/dev/null');
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
        <?php if (!$daemon_running): ?>
            <div class="alert alert-warning">
                <i class="fa fa-exclamation-triangle"></i>
                Daemon is not running. Check service status and logs.
            </div>
        <?php endif; ?>
        
        <!-- Service Controls -->
        <div class="service-controls">
            <button class="btn btn-sm <?php echo $daemon_running ? 'btn-danger' : 'btn-success'; ?>"
                    onclick="toggleService('<?php echo $service_name; ?>', '<?php echo $daemon_running ? 'stop' : 'start'; ?>')">
                <i class="fa fa-<?php echo $daemon_running ? 'stop' : 'play'; ?>"></i>
                <?php echo $daemon_running ? 'Stop Service' : 'Start Service'; ?>
            </button>
            
            <button class="btn btn-sm btn-default" onclick="showLogs('<?php echo $service_name; ?>')">
                <i class="fa fa-list-alt"></i> View Logs
            </button>
            
            <button class="btn btn-sm btn-default" onclick="editConfig()">
                <i class="fa fa-cog"></i> Configure
            </button>
        </div>
        
        <!-- Device Status -->
        <?php if ($daemon_running && !empty($usb_devices)): ?>
        <div class="device-status">
            <h4>Connected Devices</h4>
            <pre class="device-list"><?php echo htmlspecialchars(trim($usb_devices)); ?></pre>
        </div>
        <?php endif; ?>
        
        <!-- Controller Information -->
        <?php if (!empty($status_data)): ?>
        <div class="controller-info">
            <h4>Controller Status</h4>
            <div class="row">
                <?php if (isset($status_data['controllers'])): ?>
                    <?php foreach ($status_data['controllers'] as $controller): ?>
                    <div class="col-sm-6">
                        <div class="controller-card">
                            <strong>Controller <?php echo htmlspecialchars($controller['unit']); ?></strong>
                            <br>Type: <?php echo htmlspecialchars($controller['type']); ?>
                            <br>Devices: <?php echo count($controller['devices']); ?>
                        </div>
                    </div>
                    <?php endforeach; ?>
                <?php endif; ?>
            </div>
            
            <?php if (isset($status_data['fans'])): ?>
            <h5>Fan Status</h5>
            <div class="fan-status">
                <?php foreach ($status_data['fans'] as $fan): ?>
                <div class="fan-item">
                    Fan <?php echo htmlspecialchars($fan['id']); ?>: 
                    <span class="fan-speed"><?php echo $fan['speed']; ?>%</span>
                </div>
                <?php endforeach; ?>
            </div>
            <?php endif; ?>
            
            <?php if (isset($status_data['lighting'])): ?>
            <h5>Lighting Status</h5>
            <div class="lighting-status">
                Mode: <?php echo htmlspecialchars($status_data['lighting']['mode']); ?>
                <?php if (isset($status_data['lighting']['color'])): ?>
                <br>Color: RGB(<?php echo $status_data['lighting']['color']['r']; ?>, 
                              <?php echo $status_data['lighting']['color']['g']; ?>, 
                              <?php echo $status_data['lighting']['color']['b']; ?>)
                <?php endif; ?>
            </div>
            <?php endif; ?>
        </div>
        <?php endif; ?>
        
        <!-- Recent Logs -->
        <?php if (!empty($logs)): ?>
        <div class="recent-logs">
            <h5>Recent Logs</h5>
            <pre class="log-viewer"><?php echo htmlspecialchars(trim($logs)); ?></pre>
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

.service-controls {
    margin-bottom: 15px;
}

.service-controls .btn {
    margin-right: 5px;
}

.device-status, .controller-info, .recent-logs {
    margin-top: 15px;
}

.device-list, .log-viewer {
    background: #34495e;
    padding: 10px;
    border-radius: 3px;
    font-size: 12px;
    overflow-x: auto;
}

.controller-card {
    background: #34495e;
    padding: 10px;
    border-radius: 3px;
    margin-bottom: 10px;
}

.fan-status, .lighting-status {
    background: #34495e;
    padding: 10px;
    border-radius: 3px;
    margin-top: 10px;
}

.fan-item {
    margin-bottom: 5px;
}

.fan-speed {
    font-weight: bold;
    color: #3498db;
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

h4, h5 {
    color: #ecf0f1;
    margin-top: 0;
    margin-bottom: 10px;
}
</style>

<script>
function toggleService(service, action) {
    var cmd = action === 'start' ? 'systemctl start ' + service : 'systemctl stop ' + service;
    
    $.post('/plugins/dynamix.docker.manager/scripts/execute', {
        cmd: cmd
    }, function(response) {
        setTimeout(function() {
            location.reload();
        }, 2000);
    });
}

function showLogs(service) {
    var cmd = 'journalctl -u ' + service + ' -f --no-pager';
    
    // Open a modal or redirect to logs page
    window.open('/plugins/dynamix.docker.manager/scripts/terminal?cmd=' + encodeURIComponent(cmd), '_blank');
}

function editConfig() {
    window.open('/Config.php?view=plugins&name=<?php echo $plugin_name; ?>', '_blank');
}
</script>
