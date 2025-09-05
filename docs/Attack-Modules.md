# Attack Modules Documentation

Comprehensive technical documentation for all attack modules in GooseHack.

## Overview

GooseHack implements multiple attack vectors targeting different aspects of wireless security. Each module is designed for specific testing scenarios and vulnerability assessment.

## WiFi Attack Modules

### Deauthentication Attack

#### Technical Implementation
The deauthentication attack uses IEEE 802.11 management frames to forcibly disconnect clients from access points.

**Frame Structure**:
```
Frame Control: 0xC0 (Deauthentication)
Duration: 0x0000
Destination: Target client MAC or broadcast
Source: Target AP BSSID
BSSID: Target AP BSSID
Sequence: Incremental
Reason Code: 0x0007 (Class 3 frame from non-associated STA)
```

**Attack Vectors**:
1. **Broadcast Deauth**: Targets all clients on the network
2. **Unicast Deauth**: Targets specific client devices
3. **Bidirectional**: Sends frames from both AP and client perspectives
4. **Continuous**: Maintains persistent disconnection

**Parameters**:
- Packet interval: 100ms default
- Reason codes: Configurable (default: 0x0007)
- Target filtering: MAC address based
- Channel hopping: Disabled during attack

#### Effectiveness Factors
- **Signal strength**: Stronger signal improves success rate
- **Client behavior**: Some devices ignore deauth frames
- **AP protection**: 802.11w (PMF) provides resistance
- **Timing**: Rapid succession increases effectiveness

### Evil Twin Attack

#### Technical Implementation
Creates a rogue access point with identical SSID to legitimate network, implementing a captive portal for credential harvesting.

**AP Configuration**:
```cpp
SSID: Cloned from target network
Channel: Same as target (or optimal)
Security: Open (to force captive portal)
BSSID: Randomized MAC address
Beacon Interval: 100ms
```

**Captive Portal Features**:
- Automatic redirection of all HTTP requests
- Realistic login interface mimicking common routers
- Credential logging with timestamps
- Device fingerprinting via User-Agent
- Multi-language support

**Data Captured**:
- WiFi passwords
- Device MAC addresses
- Browser information
- Connection timestamps
- Attempted credentials (even incorrect ones)

#### Security Bypass Techniques
- **DNS Hijacking**: Redirects all DNS queries to captive portal
- **HTTP Interception**: Captures all web traffic
- **HTTPS Downgrade**: Forces HTTP for credential capture
- **Certificate Spoofing**: Presents fake certificates for HTTPS sites

### Combined Attack

#### Multi-Core Architecture
Utilizes ESP32's dual-core processor for simultaneous operations:

**Core 0 (Protocol CPU)**:
- Deauthentication task execution
- Raw packet transmission
- Low-level WiFi operations
- Real-time packet processing

**Core 1 (Application CPU)**:
- Web server operations
- Evil twin access point
- User interface handling
- Data management

**Task Management**:
```cpp
xTaskCreatePinnedToCore(
    deauthTask,     // Task function
    "DeauthTask",   // Task name
    8192,           // Stack size
    NULL,           // Parameters
    2,              // Priority
    &taskHandle,    // Task handle
    0               // Core ID (0)
);
```

#### Synchronization Mechanisms
- Shared memory for status communication
- Atomic operations for thread safety
- Task notifications for coordination
- Proper cleanup on termination

### Beacon Spam

#### Implementation Details
Floods the wireless spectrum with fake access point advertisements.

**Target Mode**:
- Creates 10 identical SSIDs on different channels
- Channels used: 1, 6, 11, 3, 8, 13, 2, 7, 12, 4
- Unique MAC addresses for each fake AP
- Identical security settings to target

**Custom Mode**:
- User-defined SSID list
- Sequential MAC address generation
- Configurable broadcast intervals
- Memory-efficient string handling

**Packet Structure**:
```
Frame Type: 0x80 (Beacon)
Flags: 0x00
Duration: 0x0000
Destination: Broadcast (FF:FF:FF:FF:FF:FF)
Source: Generated MAC address
BSSID: Same as source
Timestamp: Current system time
Beacon Interval: 0x0064 (100ms)
Capability: 0x0401 (ESS + Privacy)
```

#### Channel Management
- Rapid channel switching (every 100ms)
- Full 2.4GHz spectrum coverage
- Interference avoidance algorithms
- Optimal channel selection based on environment

### Handshake Capture

#### PMKID Attack Implementation
Modern attack vector targeting WPA2 networks without requiring client deauthentication.

**PMKID Structure**:
```
PMKID = HMAC-SHA1-128(PMK, "PMK Name" + MAC_AP + MAC_STA)
```

**Capture Process**:
1. Send association request to target AP
2. Parse association response for RSN IE
3. Extract PMKID from RSN information element
4. Store for offline cracking

**Advantages**:
- No client disruption required
- Faster than traditional handshake capture
- Works on networks with no connected clients
- Stealthier operation

#### 4-Way Handshake Capture
Traditional WPA/WPA2 handshake capture with enhanced validation.

**Message Validation**:
```cpp
Message 1: ACK=1, MIC=0, Install=0 (ANonce)
Message 2: ACK=0, MIC=1, Install=0 (SNonce + MIC)
Message 3: ACK=1, MIC=1, Install=1 (GTK + MIC)
Message 4: ACK=0, MIC=1, Install=0 (Confirmation)
```

**Enhanced Features**:
- Proper EAPOL frame parsing
- Key information field validation
- Nonce extraction and storage
- MIC verification capabilities
- Replay counter tracking

**Output Formats**:
- Standard PCAP format
- Hashcat-compatible hccapx
- Aircrack-ng compatible cap files
- Raw packet dumps for analysis

## Network Infrastructure Attacks

### Printer Attack Module

#### Discovery Mechanisms
Scans network for printing services using multiple protocols.

**Port Scanning**:
```cpp
Ports Scanned:
- 9100: RAW/JetDirect printing
- 515:  Line Printer Daemon (LPD)
- 631:  Internet Printing Protocol (IPP)
- 80:   Web management interface
- 443:  Secure web management
```

**Protocol Implementation**:

**RAW Printing (Port 9100)**:
```
PJL Header: \x1B%-12345X@PJL
Job Control: @PJL JOB NAME="Security Test"
Language: @PJL ENTER LANGUAGE=PCL
PCL Commands: Font and formatting
Message Content: User-defined text
Termination: \x1BE\x1B%-12345X
```

**IPP Implementation**:
```http
POST /ipp/print HTTP/1.1
Host: [printer_ip]:631
Content-Type: application/ipp
Content-Length: [length]

[IPP Binary Data]
```

#### Security Testing Capabilities
- Default credential testing
- SNMP community string enumeration
- Web interface vulnerability scanning
- Firmware version detection
- Configuration extraction

## Bluetooth Attack Modules

### Sour Apple Attack

#### BLE Advertisement Structure
Exploits Apple's device discovery protocol through malicious advertisements.

**Advertisement Data**:
```cpp
Length: 0x10 (16 bytes)
Type: 0xFF (Manufacturer Specific)
Company ID: 0x004C (Apple Inc.)
Subtype: 0x0F (Proximity Pairing)
Status: 0x05 (Configurable)
Device Type: Randomized from Apple device list
```

**Target Device Types**:
- AirPods (various generations)
- Apple TV
- HomePod
- iPhone/iPad proximity pairing
- Apple Watch
- MacBook Pro/Air

#### Attack Mechanisms
- **Notification Spam**: Triggers repeated iOS popups
- **Battery Drain**: Causes excessive BLE processing
- **UI Disruption**: Interferes with normal device operation
- **Tracking**: Enables device presence detection

### Windows Bluetooth Attack

#### Swift Pair Exploitation
Targets Windows 10/11 Swift Pair functionality.

**Advertisement Structure**:
```cpp
Flags: 0x06 (LE General + BR/EDR Not Supported)
Complete Local Name: "Swift Pair Device"
Manufacturer Data: Microsoft Company ID (0x0006)
Service Data: Swift Pair service UUID
```

**Attack Vectors**:
- Connection flooding
- Pairing request spam
- Service discovery overload
- Stack overflow attempts

## Attack Effectiveness Analysis

### Success Rate Factors

#### Environmental Conditions
- **Signal Strength**: -30dBm to -70dBm optimal range
- **Interference**: 2.4GHz congestion affects performance
- **Physical Obstacles**: Walls and barriers reduce effectiveness
- **Distance**: Optimal range 1-50 meters depending on antenna

#### Target Characteristics
- **Device Age**: Older devices more susceptible
- **Security Implementation**: 802.11w reduces deauth effectiveness
- **Client Behavior**: Some devices ignore management frames
- **Network Configuration**: Enterprise networks have better protection

#### Timing Considerations
- **Peak Usage**: More clients during business hours
- **Reconnection Patterns**: Varies by device and OS
- **Attack Duration**: Longer attacks increase detection risk
- **Coordination**: Simultaneous attacks more effective

### Countermeasures and Detection

#### Network-Level Protections
- **802.11w (PMF)**: Protects management frames
- **Intrusion Detection**: Monitors for attack patterns
- **Rate Limiting**: Prevents excessive deauth frames
- **MAC Filtering**: Restricts authorized devices

#### Client-Side Protections
- **Randomized MAC**: Complicates tracking
- **Fast Transition**: Reduces handshake exposure
- **Certificate Pinning**: Prevents evil twin attacks
- **Network Validation**: Checks for legitimate APs

## Performance Optimization

### Memory Management
- Efficient packet buffer allocation
- Dynamic string handling for SSIDs
- Garbage collection for long-running attacks
- Stack size optimization for tasks

### Processing Efficiency
- Interrupt-driven packet handling
- Optimized channel switching algorithms
- Minimal delay in critical paths
- Parallel processing utilization

### Power Consumption
- Dynamic frequency scaling
- Sleep modes during idle periods
- Optimized transmission power
- Efficient antenna utilization

## Integration with External Tools

### Packet Analysis
- Wireshark compatibility for PCAP files
- Aircrack-ng integration for handshake cracking
- Hashcat support for PMKID attacks
- Custom analysis scripts

### Automation Capabilities
- Serial command interface
- Programmatic control via web API
- Batch operation support
- Integration with penetration testing frameworks

### Data Export Formats
- Standard PCAP for network analysis
- CSV for statistical analysis
- JSON for programmatic processing
- Plain text for manual review
