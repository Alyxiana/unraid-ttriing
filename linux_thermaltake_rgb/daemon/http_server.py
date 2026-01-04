"""
HTTP Server for Thermaltake RGB Daemon Status
Provides REST API endpoints for monitoring and status
"""

import json
import threading
import time
from http.server import HTTPServer, BaseHTTPRequestHandler
from urllib.parse import urlparse, parse_qs
from linux_thermaltake_rgb import LOGGER


class StatusHandler(BaseHTTPRequestHandler):
    """HTTP request handler for status endpoints"""
    
    def __init__(self, daemon_instance, *args, **kwargs):
        self.daemon = daemon_instance
        super().__init__(*args, **kwargs)
    
    def do_GET(self):
        """Handle GET requests"""
        parsed_path = urlparse(self.path)
        
        if parsed_path.path == '/status':
            self.handle_status()
        elif parsed_path.path == '/health':
            self.handle_health()
        else:
            self.send_response(404)
            self.end_headers()
    
    def handle_status(self):
        """Return daemon status information"""
        try:
            status_data = {
                'timestamp': int(time.time()),
                'running': self.daemon._continue,
                'controllers': [],
                'fans': [],
                'lighting': None
            }
            
            # Get controller information
            for unit, controller in self.daemon.controllers.items():
                controller_info = {
                    'unit': unit,
                    'type': controller.__class__.__name__.replace('Controller', '').lower(),
                    'devices': []
                }
                
                # Get device information
                for device_id, device in controller.attached_devices.items():
                    device_info = {
                        'id': device_id,
                        'model': device.model,
                        'type': 'fan' if hasattr(device, 'get_speed') else 'light'
                    }
                    
                    # Add fan-specific information
                    if hasattr(device, 'get_speed'):
                        try:
                            device_info['speed'] = device.get_speed()
                        except:
                            device_info['speed'] = 'N/A'
                    
                    controller_info['devices'].append(device_info)
                
                status_data['controllers'].append(controller_info)
            
            # Get fan manager status
            if self.daemon.fan_manager:
                for device in self.daemon.fan_manager.devices:
                    try:
                        speed = device.get_speed() if hasattr(device, 'get_speed') else 'N/A'
                        status_data['fans'].append({
                            'id': f"{device.controller.unit}:{device.port}",
                            'model': device.model,
                            'speed': speed
                        })
                    except Exception as e:
                        LOGGER.warning(f"Error getting fan status: {e}")
            
            # Get lighting manager status
            if self.daemon.lighting_manager:
                lighting_info = {
                    'mode': self.daemon.lighting_manager.__class__.__name__.replace('Lighting', '').lower()
                }
                
                # Try to get current color if available
                if hasattr(self.daemon.lighting_manager, 'rgb'):
                    lighting_info['color'] = {
                        'r': self.daemon.lighting_manager.rgb[0],
                        'g': self.daemon.lighting_manager.rgb[1],
                        'b': self.daemon.lighting_manager.rgb[2]
                    }
                
                status_data['lighting'] = lighting_info
            
            # Send response
            self.send_response(200)
            self.send_header('Content-type', 'application/json')
            self.send_header('Access-Control-Allow-Origin', '*')
            self.end_headers()
            self.wfile.write(json.dumps(status_data, indent=2).encode())
            
        except Exception as e:
            LOGGER.error(f"Error generating status: {e}")
            self.send_response(500)
            self.end_headers()
    
    def handle_health(self):
        """Simple health check endpoint"""
        self.send_response(200 if self.daemon._continue else 503)
        self.send_header('Content-type', 'application/json')
        self.end_headers()
        self.wfile.write(json.dumps({'status': 'ok' if self.daemon._continue else 'stopped'}).encode())
    
    def log_message(self, format, *args):
        """Suppress default HTTP logging"""
        pass


class StatusHTTPServer:
    """HTTP server for daemon status"""
    
    def __init__(self, daemon_instance, host='localhost', port=5334):
        self.daemon = daemon_instance
        self.host = host
        self.port = port
        self.server = None
        self.server_thread = None
    
    def start(self):
        """Start the HTTP server in a separate thread"""
        try:
            # Create handler with daemon instance
            handler = lambda *args, **kwargs: StatusHandler(self.daemon, *args, **kwargs)
            
            self.server = HTTPServer((self.host, self.port), handler)
            self.server_thread = threading.Thread(target=self.server.serve_forever, daemon=True)
            self.server_thread.start()
            
            LOGGER.info(f"Status HTTP server started on http://{self.host}:{self.port}")
            
        except Exception as e:
            LOGGER.error(f"Failed to start HTTP server: {e}")
    
    def stop(self):
        """Stop the HTTP server"""
        if self.server:
            self.server.shutdown()
            self.server.server_close()
            if self.server_thread:
                self.server_thread.join(timeout=5)
            LOGGER.info("Status HTTP server stopped")
