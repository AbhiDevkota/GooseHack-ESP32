# GooseHack - ESP32 WiFi Security Testing Framework

A comprehensive ESP32-based wireless security testing platform designed for authorized penetration testing and security research. This tool provides multiple attack vectors and monitoring capabilities through a web-based interface.

## Table of Contents

- [Features](#features)
- [Hardware Requirements](#hardware-requirements)
- [Installation](#installation)
- [Usage](#usage)
- [Attack Modules](#attack-modules)
- [Web Interface](#web-interface)
- [Configuration](#configuration)
- [Legal Notice](#legal-notice)
- [Contributing](#contributing)
- [License](#license)

## Features

### Core Attack Capabilities
- **Deauthentication Attacks**: Targeted client disconnection with MAC filtering
- **Evil Twin Access Points**: Credential harvesting through captive portals
- **Combined Attacks**: Simultaneous deauth and evil twin operations
- **Beacon Spam**: Network flooding with multiple fake access points
- **Handshake Capture**: WPA/WPA2 handshake and PMKID collection
- **Network Printer Exploitation**: Unauthorized printing on network devices
- **Bluetooth Low Energy Attacks**: Sour Apple and Windows device targeting

### Advanced Features
- **Standards-Compliant Packet Capture**: Compatible with aircrack-ng and hashcat
- **Multi-Protocol Printer Support**: RAW, LPD, and IPP printing protocols
- **Persistent Configuration**: EEPROM-based settings storage
- **Real-Time Monitoring**: Live attack statistics and device tracking
- **Channel Hopping**: Automated frequency scanning and targeting

## Hardware Requirements

### Minimum Requirements
- ESP32 development board (WROOM-32 or similar)
- Micro USB cable for programming and power
- Computer with Arduino IDE or compatible development environment

### Recommended Hardware
- ESP32 with external antenna connector
- High-gain directional antenna (2.4GHz)
- External power supply for extended operations
- SD card module for packet storage (optional)

### Supported Boards
- ESP32 WROOM-32
- ESP32 WROVER
- ESP32-S2
- ESP32-C3
- Other ESP32 variants with WiFi capability

## Installation

### Prerequisites
1. Arduino IDE 1.8.19 or later
2. ESP32 board package installed
3. Required libraries (see below)

### Required Libraries
Install the following libraries through Arduino IDE Library Manager:
- ESP32 WiFi (built-in)
- NimBLE-Arduino
- DNSServer (built-in)
- WebServer (built-in)
- Preferences (built-in)

### Installation Steps

1. **Clone the repository**
   ```bash
   git clone https://github.com/AbhiDevkota/GooseHack.git
   cd GooseHack
   ```

2. **Open in Arduino IDE**
   - Open `captive.ino` in Arduino IDE
   - Select your ESP32 board from Tools > Board
   - Select the correct COM port

3. **Configure settings** (optional)
   - Modify `config.h` for custom default settings
   - Adjust TX power and channel preferences

4. **Upload to ESP32**
   - Click Upload button or use Ctrl+U
   - Monitor serial output for successful initialization

### Alternative Build Method
Use the included build script for command-line compilation:
```bash
chmod +x build.sh
./build.sh
```

## Usage

### Initial Setup
1. Power on the ESP32 device
2. Connect to the WiFi network "Durgesh ko bhai Hagess" (default)
3. Default password: "Bruh1234@#"
4. Navigate to `192.168.4.1` in your web browser

### Basic Operation Flow
1. **Network Discovery**: Use the rescan function to identify target networks
2. **Target Selection**: Choose specific SSID and channel for attacks
3. **Attack Execution**: Select appropriate attack vector from web interface
4. **Monitoring**: Observe real-time statistics and captured data
5. **Data Extraction**: Download captured handshakes or view credentials

## Attack Modules

### Deauthentication Attack
Disconnects clients from target access points using IEEE 802.11 deauthentication frames.

**Features:**
- Target-specific MAC filtering
- Broadcast and unicast deauth frames
- Configurable packet intervals
- Station discovery and tracking

**Use Cases:**
- Testing client reconnection behavior
- Forcing handshake captures
- Network availability testing

### Evil Twin Attack
Creates a fake access point mimicking legitimate networks to capture credentials.

**Features:**
- Identical SSID replication
- Captive portal implementation
- Credential logging and storage
- Multi-language portal support

**Captured Data:**
- WiFi passwords
- Device information
- Connection timestamps
- User agent strings

### Combined Attack
Executes deauthentication and evil twin attacks simultaneously for maximum effectiveness.

**Technical Implementation:**
- Multi-core task management
- Separate deauth task on Core 0
- Web server and AP on Core 1
- Synchronized attack coordination

### Beacon Spam
Floods the area with fake WiFi networks to create confusion and hide legitimate networks.

**Modes:**
- **Target Mode**: Creates multiple copies of selected network on different channels
- **Custom Mode**: Broadcasts user-defined SSID lists
- **Random Mode**: Generates random network names

### Handshake Capture
Captures WPA/WPA2 handshakes and PMKID for offline password cracking.

**Supported Methods:**
- **Passive Capture**: Monitors natural handshakes and PMKID
- **Aggressive Capture**: Forces handshakes through deauthentication
- **PMKID Collection**: Modern attack vector for WPA2 networks

**Output Formats:**
- Standard PCAP files
- Hashcat-compatible formats
- Real-time packet analysis

### Network Printer Attack
Discovers and exploits network printers for unauthorized document printing.

**Supported Protocols:**
- RAW printing (Port 9100)
- Line Printer Daemon (Port 515)
- Internet Printing Protocol (Port 631)

**Capabilities:**
- Network printer discovery
- Multi-protocol communication
- Custom message printing
- Security awareness demonstrations

### Bluetooth Attacks

#### Sour Apple Attack
Targets Apple devices using BLE advertisement manipulation.

**Mechanism:**
- Mimics Apple device advertisements
- Triggers iOS popup notifications
- Causes device performance degradation
- Demonstrates BLE security weaknesses

#### Windows Bluetooth Attack
Exploits Windows Bluetooth stack vulnerabilities.

**Features:**
- Swift Pair exploitation
- Device enumeration
- Connection flooding
- Bluetooth stack stress testing

## Web Interface

### Main Dashboard
- Real-time attack statistics
- Network discovery results
- Device connection status
- System configuration options

### Attack Controls
- Individual attack module toggles
- Parameter configuration
- Real-time monitoring
- Emergency stop functionality

### Data Management
- Captured credential viewing
- Handshake download options
- Log file management
- Configuration backup/restore

### Network Configuration
- Access point settings
- Channel selection
- Power level adjustment
- Persistent configuration storage

## Configuration

### Default Settings
```cpp
String MAIN_SSID = "Durgesh ko bhai Hagess";
String MAIN_PASS = "Bruh1234@#";
IPAddress APIP(192, 168, 4, 1);
const byte DNS_PORT = 53;
```

### Customization Options
- Modify `config.h` for default values
- Web interface for runtime changes
- EEPROM storage for persistence
- Factory reset capabilities

### Performance Tuning
- TX power adjustment (0-78)
- Channel hopping intervals
- Task priority settings
- Memory allocation optimization

## Legal Notice

**IMPORTANT: This tool is designed exclusively for authorized security testing and educational purposes.**

### Authorized Use Only
- Obtain explicit written permission before testing any network
- Use only on networks you own or have authorization to test
- Comply with all applicable local, state, and federal laws
- Respect privacy and data protection regulations

### Prohibited Activities
- Unauthorized network access or testing
- Malicious use against third-party networks
- Commercial exploitation without proper licensing
- Any activity violating applicable laws or regulations

### User Responsibility
Users are solely responsible for ensuring their use of this tool complies with all applicable laws and regulations. The developers assume no liability for misuse or illegal activities.

## Contributing

### Development Guidelines
1. Fork the repository
2. Create a feature branch
3. Implement changes with proper documentation
4. Test thoroughly on multiple ESP32 variants
5. Submit pull request with detailed description

### Code Standards
- Follow existing code style and structure
- Include comprehensive comments
- Implement proper error handling
- Maintain backward compatibility where possible

### Bug Reports
- Use GitHub Issues for bug reports
- Include detailed reproduction steps
- Provide hardware and software version information
- Attach relevant log files or screenshots

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

### Third-Party Libraries
- ESP32 Arduino Core: Apache License 2.0
- NimBLE-Arduino: Apache License 2.0
- Additional dependencies listed in respective source files

## Disclaimer

This software is provided "as is" without warranty of any kind. The developers are not responsible for any damage or legal consequences resulting from the use of this tool. Users must ensure compliance with all applicable laws and regulations in their jurisdiction.

---

**Version**: 1.0.0  
**Last Updated**: January 2025  
**Maintainer**: AbhiDevkota
