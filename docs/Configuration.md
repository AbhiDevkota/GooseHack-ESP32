# Configuration Guide

Comprehensive configuration options for GooseHack ESP32 WiFi Security Testing Framework.

## Configuration Files

### config.h - Main Configuration
Primary configuration file containing system-wide settings and default values.

#### Network Settings
```cpp
// Default Access Point Configuration
String MAIN_SSID = "Durgesh ko bhai Hagess";
String MAIN_PASS = "Bruh1234@#";
IPAddress APIP(192, 168, 4, 1);
const byte DNS_PORT = 53;
const byte HTTP_CODE = 200;

// Network Interface Settings
#define WIFI_CHANNEL_DEFAULT 1
#define MAX_CLIENTS 4
#define BEACON_INTERVAL 100
```

#### Attack Parameters
```cpp
// Deauthentication Settings
#define ATTACK_INTERVAL 100        // Milliseconds between packets
#define DEAUTH_REASON_CODE 0x0007  // Reason code for deauth frames
#define MAX_DEAUTH_PACKETS 1000    // Maximum packets per session

// Handshake Capture Settings
#define HANDSHAKE_TIMEOUT 80000    // Timeout in milliseconds
#define MAX_HANDSHAKE_SIZE 50000   // Maximum capture buffer size
#define PMKID_TIMEOUT 30000        // PMKID capture timeout

// Beacon Spam Settings
#define BEACON_SPAM_INTERVAL 100   // Milliseconds between beacons
#define MAX_FAKE_APS 20           // Maximum simultaneous fake APs
#define CHANNEL_HOP_INTERVAL 500   // Channel switching interval
```

#### Hardware Configuration
```cpp
// Power Management
#define TX_POWER_MAX 78           // Maximum transmission power (0-78)
#define CPU_FREQUENCY 240         // CPU frequency in MHz
#define WIFI_POWER_SAVE false     // WiFi power saving mode

// Memory Allocation
#define TASK_STACK_SIZE 8192      // Stack size for attack tasks
#define WEB_SERVER_STACK 4096     // Web server task stack
#define CAPTURE_BUFFER_SIZE 2048  // Packet capture buffer
```

### Persistent Settings (EEPROM)
Settings stored in ESP32's non-volatile memory that persist across reboots.

#### Stored Parameters
- Access Point SSID and password
- Default channel preferences
- TX power settings
- Attack timing parameters
- User preferences

#### EEPROM Functions
```cpp
void loadAPSettings();    // Load saved AP configuration
void saveAPSettings();    // Save current AP configuration
void factoryReset();      // Clear all saved settings
```

## Web Interface Configuration

### Runtime Settings
Settings that can be modified through the web interface at 192.168.4.1.

#### Access Point Settings
- **SSID**: Network name for the ESP32 access point
- **Password**: WPA2 password (minimum 8 characters)
- **Channel**: Operating channel (1-13)
- **Hidden**: Broadcast SSID visibility

#### Attack Configuration
- **Target Selection**: Choose specific network for attacks
- **Attack Intensity**: Packet transmission rate
- **Timeout Values**: Maximum duration for each attack type
- **Auto-Stop**: Automatic termination conditions

#### Capture Settings
- **File Format**: PCAP, CAP, or custom formats
- **Compression**: Enable/disable packet compression
- **Filtering**: Capture specific packet types only
- **Storage**: Local buffer or external storage options

## Advanced Configuration

### Custom Attack Parameters

#### Deauthentication Customization
```cpp
// Custom reason codes for different scenarios
#define DEAUTH_UNSPECIFIED 0x0001
#define DEAUTH_PREV_AUTH_INVALID 0x0002
#define DEAUTH_LEAVING_NETWORK 0x0003
#define DEAUTH_INACTIVITY 0x0004
#define DEAUTH_AP_OVERLOAD 0x0005
#define DEAUTH_CLASS2_FRAME 0x0006
#define DEAUTH_CLASS3_FRAME 0x0007

// Timing configurations
struct DeauthConfig {
    uint16_t interval_ms;      // Time between packets
    uint16_t burst_count;      // Packets per burst
    uint16_t burst_delay;      // Delay between bursts
    bool bidirectional;        // Send from both AP and client
    bool randomize_timing;     // Add random jitter
};
```

#### Evil Twin Customization
```cpp
// Captive portal configuration
struct EvilTwinConfig {
    String portal_title;       // Page title
    String portal_message;     // Login prompt message
    String redirect_url;       // Post-login redirect
    bool log_attempts;         // Log failed attempts
    bool mimic_target;         // Copy target AP appearance
    uint16_t session_timeout;  // Session timeout in minutes
};
```

### Channel Management

#### Channel Lists
```cpp
// 2.4GHz channels (region-dependent)
uint8_t channels_us[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
uint8_t channels_eu[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
uint8_t channels_jp[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};

// Optimal channels for beacon spam
uint8_t optimal_channels[] = {1, 6, 11};  // Non-overlapping channels
```

#### Channel Hopping Configuration
```cpp
struct ChannelConfig {
    uint8_t* channel_list;     // Array of channels to use
    uint8_t channel_count;     // Number of channels in list
    uint16_t hop_interval;     // Time on each channel (ms)
    bool random_order;         // Randomize channel sequence
    bool avoid_interference;   // Skip congested channels
};
```

### Security Configuration

#### MAC Address Management
```cpp
// MAC address randomization
struct MACConfig {
    bool randomize_on_boot;    // New MAC on startup
    bool randomize_per_attack; // New MAC per attack
    uint8_t oui_prefix[3];     // Organizationally Unique Identifier
    bool use_real_oui;         // Use legitimate vendor OUIs
};
```

#### Encryption Settings
```cpp
// WPA/WPA2 configuration for evil twin
struct SecurityConfig {
    bool use_wpa2;             // Enable WPA2 encryption
    String psk;                // Pre-shared key
    bool use_enterprise;       // WPA2-Enterprise mode
    String radius_server;      // RADIUS server IP
    uint16_t radius_port;      // RADIUS server port
};
```

## Performance Tuning

### Memory Optimization

#### Buffer Sizes
```cpp
// Adjust based on available RAM
#define RX_BUFFER_SIZE 1024       // Receive buffer
#define TX_BUFFER_SIZE 1024       // Transmit buffer
#define PACKET_QUEUE_SIZE 50      // Packet queue depth
#define STRING_BUFFER_SIZE 256    // General string operations
```

#### Task Priorities
```cpp
// FreeRTOS task priorities (0-25, higher = more priority)
#define PRIORITY_DEAUTH_TASK 5    // Deauth attack task
#define PRIORITY_WEB_SERVER 3     // Web interface
#define PRIORITY_PACKET_CAPTURE 4 // Packet capture
#define PRIORITY_BEACON_SPAM 2    // Beacon spam
```

### CPU Optimization

#### Core Assignment
```cpp
// Assign tasks to specific CPU cores
#define CORE_PROTOCOL 0           // Core 0 for WiFi operations
#define CORE_APPLICATION 1        // Core 1 for user interface
```

#### Frequency Scaling
```cpp
// CPU frequency options (MHz)
#define CPU_FREQ_80  80
#define CPU_FREQ_160 160
#define CPU_FREQ_240 240

// Power vs Performance trade-offs
setCpuFrequencyMhz(CPU_FREQ_240);  // Maximum performance
setCpuFrequencyMhz(CPU_FREQ_160);  // Balanced
setCpuFrequencyMhz(CPU_FREQ_80);   // Power saving
```

## Regional Compliance

### Regulatory Settings

#### Transmission Power Limits
```cpp
// Regional power limits (dBm)
#define POWER_LIMIT_FCC 20        // United States (FCC)
#define POWER_LIMIT_CE 20         // Europe (CE)
#define POWER_LIMIT_IC 20         // Canada (IC)
#define POWER_LIMIT_MIC 20        // Japan (MIC)

// Set appropriate limit for your region
#define TX_POWER_LIMIT POWER_LIMIT_FCC
```

#### Channel Restrictions
```cpp
// Region-specific channel availability
#ifdef REGION_US
    #define AVAILABLE_CHANNELS {1,2,3,4,5,6,7,8,9,10,11}
#elif REGION_EU
    #define AVAILABLE_CHANNELS {1,2,3,4,5,6,7,8,9,10,11,12,13}
#elif REGION_JP
    #define AVAILABLE_CHANNELS {1,2,3,4,5,6,7,8,9,10,11,12,13,14}
#endif
```

## Debugging Configuration

### Debug Levels
```cpp
// Debug output levels
#define DEBUG_NONE 0
#define DEBUG_ERROR 1
#define DEBUG_WARNING 2
#define DEBUG_INFO 3
#define DEBUG_VERBOSE 4

// Set current debug level
#define DEBUG_LEVEL DEBUG_INFO
```

### Serial Output Configuration
```cpp
// Serial communication settings
#define SERIAL_BAUD_RATE 115200
#define SERIAL_BUFFER_SIZE 256
#define ENABLE_SERIAL_DEBUG true
#define TIMESTAMP_DEBUG_OUTPUT true
```

### Logging Configuration
```cpp
// Log file settings
struct LogConfig {
    bool enable_logging;       // Enable file logging
    String log_filename;       // Log file name
    uint32_t max_log_size;     // Maximum log file size
    bool rotate_logs;          // Enable log rotation
    uint8_t max_log_files;     // Maximum number of log files
};
```

## Factory Reset and Recovery

### Reset Procedures

#### Software Reset
```cpp
void factoryReset() {
    // Clear all EEPROM settings
    preferences.begin("wifi-tool", false);
    preferences.clear();
    preferences.end();
    
    // Reset to default configuration
    loadDefaultConfig();
    
    // Restart device
    ESP.restart();
}
```

#### Hardware Reset
- Hold BOOT button while powering on
- Connect GPIO0 to GND during startup
- Use recovery firmware for complete reset

### Recovery Mode
```cpp
// Enable recovery mode on startup
bool checkRecoveryMode() {
    if (digitalRead(BOOT_BUTTON) == LOW) {
        Serial.println("Recovery mode activated");
        factoryReset();
        return true;
    }
    return false;
}
```

## Configuration Validation

### Parameter Validation
```cpp
// Validate configuration parameters
bool validateConfig() {
    // Check SSID length
    if (MAIN_SSID.length() > 32 || MAIN_SSID.length() < 1) {
        return false;
    }
    
    // Check password strength
    if (MAIN_PASS.length() < 8) {
        return false;
    }
    
    // Validate IP address
    if (!APIP.isValid()) {
        return false;
    }
    
    return true;
}
```

### Error Handling
```cpp
// Configuration error codes
#define CONFIG_OK 0
#define CONFIG_INVALID_SSID 1
#define CONFIG_WEAK_PASSWORD 2
#define CONFIG_INVALID_IP 3
#define CONFIG_INVALID_CHANNEL 4

// Handle configuration errors
void handleConfigError(int error_code) {
    switch (error_code) {
        case CONFIG_INVALID_SSID:
            Serial.println("Error: Invalid SSID");
            break;
        case CONFIG_WEAK_PASSWORD:
            Serial.println("Error: Password too weak");
            break;
        // ... handle other errors
    }
}
```

## Best Practices

### Security Considerations
- Change default credentials immediately
- Use strong passwords (minimum 12 characters)
- Regularly update firmware
- Monitor for unauthorized access
- Implement access logging

### Performance Guidelines
- Adjust buffer sizes based on available memory
- Use appropriate task priorities
- Monitor CPU and memory usage
- Optimize for specific use cases
- Regular performance testing

### Maintenance Procedures
- Regular configuration backups
- Monitor system logs
- Update configurations as needed
- Test configuration changes
- Document all modifications
