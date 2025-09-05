# User Guide

Complete guide for operating GooseHack ESP32 WiFi Security Testing Framework.

## Getting Started

### Initial Connection
1. Power on your ESP32 device
2. Connect to WiFi network "Durgesh ko bhai Hagess" (or your custom AP name)
3. Enter password "Bruh1234@#" (or your custom password)
4. Open web browser and navigate to `192.168.4.1`

### Web Interface Overview
The main interface consists of several sections:
- **Status Panel**: Real-time statistics and system information
- **Network Scanner**: Discover and select target networks
- **Attack Controls**: Launch and manage security tests
- **Management Tools**: Configuration and data handling

## Network Discovery

### Scanning for Networks
1. Click **"Rescan Networks"** button
2. Wait for scan completion (5-10 seconds)
3. Available networks will appear in the list
4. Each entry shows: SSID, Channel, Signal Strength, Security Type

### Target Selection
1. Click on desired network from the scan results
2. Target information will be highlighted
3. Selected network becomes the focus for attacks
4. Channel and BSSID are automatically configured

## Attack Operations

### Deauthentication Attack
**Purpose**: Disconnect clients from target access point

**Operation**:
1. Select target network from scan results
2. Click **"Start Deauth"** button
3. Monitor deauth packet count in status panel
4. Click **"Stop Deauth"** to terminate attack

**Indicators**:
- Button changes to red "Stop Deauth" when active
- Deauth packet counter increases
- Station count shows discovered clients

### Evil Twin Attack
**Purpose**: Create fake access point to capture credentials

**Operation**:
1. Select target network (SSID will be cloned)
2. Click **"Start Evil Twin"** button
3. Fake AP broadcasts with identical SSID
4. Victims connecting will see captive portal
5. Monitor captured credentials in management section

**Indicators**:
- Button changes to red "Stop Evil Twin" when active
- Captive portal becomes active
- Credentials appear in "View Credentials" section

### Combined Attack
**Purpose**: Simultaneous deauth and evil twin for maximum effectiveness

**Operation**:
1. Select target network
2. Click **"Start Combined"** button
3. Both deauth and evil twin operate simultaneously
4. Deauth forces disconnections while evil twin captures reconnections

**Technical Details**:
- Uses multi-core processing for parallel operations
- Deauth task runs on Core 0
- Web server and AP run on Core 1
- Automatic task management and cleanup

### Beacon Spam
**Purpose**: Flood area with fake networks to create confusion

**Target Mode**:
1. Select target network
2. Click **"Target Spam"** button
3. Creates 10 fake APs with same SSID on different channels
4. Confuses victims about which network is legitimate

**Custom Mode**:
1. Enter custom SSID list (one per line) in text area
2. Click **"Start Custom Spam"** button
3. Broadcasts all listed SSIDs simultaneously

### Handshake Capture

#### Passive Capture
**Purpose**: Monitor for natural WPA handshakes and PMKID

**Operation**:
1. Select target network
2. Click **"Passive Handshake"** button
3. Device monitors for 80 seconds
4. Captures handshakes from natural client connections
5. Auto-stops when complete handshake or PMKID found

#### Aggressive Capture
**Purpose**: Force handshakes through deauthentication

**Operation**:
1. Select target network
2. Click **"Aggressive Handshake (De)"** button
3. Simultaneously captures packets and sends deauth frames
4. Forces clients to reconnect and generate handshakes
5. Auto-stops when complete 4-way handshake captured

**Data Management**:
- Click **"Download PCAP"** to get captured data
- Use **"Clear Handshakes"** to reset capture buffer
- Compatible with aircrack-ng and hashcat for offline cracking

### Network Printer Attack
**Purpose**: Discover and test network printer security

**Operation**:
1. Ensure ESP32 is connected to target network
2. Click **"Scan Network"** in printer section
3. Discovered printers appear in list
4. Enter target IP and custom message
5. Click **"Send Print Job"** to test printing

**Supported Protocols**:
- RAW printing (Port 9100)
- Line Printer Daemon (Port 515)
- Internet Printing Protocol (Port 631)

### Bluetooth Attacks

#### Sour Apple Attack
**Purpose**: Target Apple devices with BLE advertisement spam

**Operation**:
1. Click **"Start Sour Apple"** button
2. Broadcasts malicious BLE advertisements
3. Triggers popup notifications on nearby iOS devices
4. Can cause performance degradation

#### Windows Bluetooth Attack
**Purpose**: Exploit Windows Bluetooth stack vulnerabilities

**Operation**:
1. Click **"Start Windows BT"** button
2. Sends Swift Pair exploitation packets
3. Targets Windows 10/11 devices
4. Can cause Bluetooth stack instability

## Data Management

### Viewing Captured Credentials
1. Click **"View Credentials"** button
2. Displays all captured WiFi passwords
3. Shows timestamp and device information
4. Click **"Clear Credentials"** to reset

### Handshake Data
1. **Download PCAP**: Exports captured handshakes in standard format
2. **Clear Handshakes**: Removes all captured handshake data
3. Files compatible with standard cracking tools

### Configuration Management
1. **Change AP Settings**: Modify device SSID and password permanently
2. **Update Configuration**: Changes are saved to EEPROM
3. **Factory Reset**: Clear all saved settings (requires code modification)

## Status Monitoring

### Real-Time Statistics
- **Packets**: Total packets transmitted
- **Deauths**: Deauthentication frames sent
- **Stations**: Discovered client devices
- **Handshakes**: Captured handshake packets
- **Captured**: Credential capture status

### System Information
- **Target**: Currently selected network
- **Channel**: Operating frequency
- **Mode**: Current attack mode
- **Uptime**: Device operation time

## Best Practices

### Target Selection
- Choose networks with strong signal strength
- Verify target network has connected clients
- Consider legal and ethical implications
- Obtain proper authorization before testing

### Attack Timing
- Monitor for client activity before starting attacks
- Use appropriate attack duration (avoid excessive disruption)
- Allow time for natural reconnections
- Stop attacks when objectives are met

### Data Handling
- Download captured data promptly
- Clear sensitive information after testing
- Secure storage of captured handshakes
- Proper disposal of credential data

### Operational Security
- Use in controlled environments
- Monitor for detection and countermeasures
- Maintain low profile during testing
- Document all testing activities

## Troubleshooting

### Common Issues

#### No Networks Found
- Check antenna connection
- Verify ESP32 is powered properly
- Try different physical location
- Restart device and rescan

#### Attacks Not Working
- Ensure target network is selected
- Check signal strength (closer proximity may help)
- Verify target has connected clients
- Monitor serial output for error messages

#### Web Interface Issues
- Verify correct IP address (192.168.4.1)
- Check WiFi connection to ESP32 AP
- Try different web browser
- Clear browser cache and cookies

#### Capture Problems
- Ensure target network uses WPA/WPA2
- Check for client activity on target network
- Try different capture methods (passive vs aggressive)
- Verify packet capture is enabled

### Performance Optimization
- Position ESP32 closer to target
- Use external antenna for better range
- Avoid interference from other devices
- Monitor device temperature during extended use

## Advanced Usage

### Custom Configuration
- Modify `config.h` for advanced settings
- Adjust TX power levels
- Configure custom channel lists
- Set attack timing parameters

### Integration with Other Tools
- Export PCAP files for analysis
- Use captured handshakes with hashcat
- Import wordlists for credential testing
- Combine with external monitoring tools

### Scripted Operations
- Use serial commands for automation
- Implement custom attack sequences
- Monitor via serial interface
- Log operations for analysis

## Safety and Legal Considerations

### Authorized Testing Only
- Obtain written permission before testing
- Use only on networks you own or are authorized to test
- Comply with all applicable laws and regulations
- Respect privacy and data protection requirements

### Responsible Disclosure
- Report vulnerabilities to network owners
- Provide remediation recommendations
- Follow coordinated disclosure practices
- Document findings professionally

### Risk Mitigation
- Minimize disruption to legitimate users
- Avoid accessing sensitive data
- Use minimal necessary force for testing
- Maintain professional conduct throughout testing
