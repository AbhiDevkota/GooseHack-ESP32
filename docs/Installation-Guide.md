# Installation Guide

This guide provides detailed instructions for setting up GooseHack on your ESP32 device.

## Prerequisites

### Hardware Requirements
- ESP32 development board (WROOM-32, WROVER, or compatible)
- USB cable (typically USB-A to Micro-USB)
- Computer running Windows, macOS, or Linux

### Software Requirements
- Arduino IDE 1.8.19 or later
- ESP32 Board Package
- Required Arduino libraries

## Step-by-Step Installation

### 1. Install Arduino IDE

Download and install Arduino IDE from the official website:
- Visit: https://www.arduino.cc/en/software
- Download the appropriate version for your operating system
- Follow the installation wizard

### 2. Install ESP32 Board Package

1. Open Arduino IDE
2. Go to **File > Preferences**
3. In "Additional Board Manager URLs", add:
   ```
   https://dl.espressif.com/dl/package_esp32_index.json
   ```
4. Go to **Tools > Board > Boards Manager**
5. Search for "ESP32"
6. Install "ESP32 by Espressif Systems"

### 3. Install Required Libraries

Open Arduino IDE and install these libraries via **Tools > Manage Libraries**:

#### Core Libraries (Built-in)
- WiFi
- WebServer
- DNSServer
- Preferences
- esp_wifi

#### External Libraries
- **NimBLE-Arduino**: Search and install "NimBLE-Arduino by h2zero"

### 4. Download GooseHack

#### Option A: Git Clone
```bash
git clone https://github.com/AbhiDevkota/GooseHack.git
cd GooseHack
```

#### Option B: Download ZIP
1. Visit: https://github.com/AbhiDevkota/GooseHack
2. Click "Code" > "Download ZIP"
3. Extract to desired location

### 5. Configure Arduino IDE

1. Connect your ESP32 to computer via USB
2. Open `captive.ino` in Arduino IDE
3. Configure board settings:
   - **Board**: "ESP32 Dev Module" (or your specific board)
   - **Upload Speed**: 921600
   - **CPU Frequency**: 240MHz (WiFi/BT)
   - **Flash Frequency**: 80MHz
   - **Flash Mode**: QIO
   - **Flash Size**: 4MB (32Mb)
   - **Partition Scheme**: Default 4MB with spiffs
   - **Core Debug Level**: None
   - **PSRAM**: Disabled (unless your board has PSRAM)

### 6. Select COM Port

1. Go to **Tools > Port**
2. Select the port showing your ESP32 device
   - Windows: Usually COM3, COM4, etc.
   - macOS: Usually /dev/cu.usbserial-*
   - Linux: Usually /dev/ttyUSB0 or /dev/ttyACM0

### 7. Upload Firmware

1. Click the **Upload** button (arrow icon)
2. Wait for compilation and upload to complete
3. Monitor the serial output for successful initialization

## Verification

### Serial Monitor Check
1. Open **Tools > Serial Monitor**
2. Set baud rate to **115200**
3. You should see output similar to:
   ```
   Loaded AP: [Your AP Name]
   Main AP started: [Your AP Name]
   TX power set to max
   Tool started at 192.168.4.1
   ```

### WiFi Connection Test
1. Look for WiFi network "Durgesh ko bhai Hagess" (default)
2. Connect using password "Bruh1234@#"
3. Navigate to http://192.168.4.1
4. You should see the GooseHack web interface

## Troubleshooting

### Common Issues

#### Upload Failed
- **Check USB cable**: Ensure data transfer capability
- **Driver issues**: Install CP210x or CH340 drivers if needed
- **Port selection**: Verify correct COM port is selected
- **Board selection**: Ensure correct ESP32 board type

#### Compilation Errors
- **Library missing**: Install all required libraries
- **Board package**: Update ESP32 board package to latest version
- **Arduino IDE**: Update to latest version

#### WiFi Issues
- **AP not visible**: Check serial monitor for errors
- **Cannot connect**: Verify default credentials
- **Web interface not loading**: Check IP address (192.168.4.1)

#### Memory Issues
- **Sketch too big**: Use "Minimal SPIFFS" partition scheme
- **Runtime crashes**: Check serial monitor for stack traces

### Advanced Troubleshooting

#### Enable Debug Output
Modify `config.h` to enable debugging:
```cpp
#define DEBUG_ENABLED true
```

#### Factory Reset
To reset all settings:
1. Hold BOOT button while powering on
2. Or modify code to clear preferences on startup

#### Custom Configuration
Edit `config.h` before uploading to customize:
- Default AP name and password
- IP address ranges
- TX power levels
- Channel preferences

## Performance Optimization

### Hardware Optimizations
- Use ESP32 with external antenna connector for better range
- Add heat sink for extended operation
- Use quality USB cable for stable power

### Software Optimizations
- Adjust TX power in `config.h`
- Modify task priorities for specific use cases
- Optimize memory allocation for large operations

## Next Steps

After successful installation:
1. Read the [User Guide](User-Guide.md) for operation instructions
2. Review [Attack Modules](Attack-Modules.md) for detailed attack information
3. Check [Configuration](Configuration.md) for customization options
4. See [Troubleshooting](Troubleshooting.md) for common issues

## Support

For installation issues:
1. Check existing GitHub Issues
2. Create new issue with detailed error information
3. Include hardware specifications and error logs
4. Follow issue template for faster resolution
