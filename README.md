# WiFi Penetration Testing Tool

A modular ESP32-based WiFi security testing tool with web interface.

## Features

- **Deauth Attack**: Disconnect devices from target networks
- **Evil Twin**: Create fake access points to capture credentials
- **Combined Attack**: Run deauth + evil twin simultaneously
- **Beacon Spam**: Flood area with fake WiFi networks
- **Sour Apple**: BLE attack targeting Apple devices
- **Web Interface**: Control all functions via browser at 192.168.4.1

## Hardware Requirements

- ESP32 development board
- No external display required (web-based interface)

## Installation

1. Install Arduino IDE with ESP32 board support
2. Install required libraries:
   - ESP32 WiFi library (built-in)
   - NimBLE-Arduino library
3. Upload the code to your ESP32

## Usage

1. Power on the ESP32
2. Connect to the WiFi network "Durgesh ko bhai Hagess" (password: "Bruh1234@#")
3. Open browser and go to 192.168.4.1
4. Select target network and choose attack type

## File Structure

- `captive.ino` - Main Arduino sketch
- `config.h` - Configuration and global variables
- `web_interface.h/.cpp` - Web server and HTML generation
- `attacks.h/.cpp` - Attack implementations

## Legal Notice

This tool is for educational and authorized security testing only. Users are responsible for compliance with local laws and regulations.
