# 🦆 Goose Hack - Quick Start Guide

## 🚀 Fast Compilation & Upload

### Option 1: Using Build Script (Recommended)
```bash
cd Fixed/
./build.sh
```

### Option 2: Arduino IDE
1. Install ESP32 board package (v2.0.5)
2. Install NimBLE-Arduino library
3. Open `captive.ino`
4. Select "ESP32 Dev Module" board
5. Click "Upload" or "Export Compiled Binary"

### Option 3: Arduino CLI Manual
```bash
# Install dependencies
arduino-cli core install esp32:esp32
arduino-cli lib install "NimBLE-Arduino"

# Compile
arduino-cli compile --fqbn esp32:esp32:esp32 captive.ino --output-dir build/

# Upload
esptool.py --chip esp32 --port /dev/ttyUSB0 write_flash 0x1000 build/captive.ino.bin
```

## 🎯 Usage After Upload

1. **Power on ESP32**
2. **Connect to WiFi**: "Durgesh ko bhai Hagess" (password: "Bruh1234@#")
3. **Open browser**: Go to `192.168.4.1`
4. **Start hacking**: Select target and choose attack mode!

## 🔥 Attack Modes Available

- **Deauth**: Kick devices off networks
- **Evil Twin**: Capture passwords with fake AP
- **Combined**: Deauth + Evil Twin simultaneously  
- **Beacon Spam**: Flood area with fake networks
- **Sour Apple**: Attack iOS devices
- **Windows BT**: Attack Windows Bluetooth
- **Handshake Capture**: Capture WPA handshakes for offline cracking

## 📱 Professional Web Interface

The "Goose Hack" interface provides:
- Real-time status monitoring
- Easy target selection
- One-click attack launching
- PCAP download for hashcat
- Mobile-responsive design

## ⚠️ Legal Notice

This tool is for **educational and authorized security testing only**. 
Users are responsible for compliance with local laws and regulations.

---
**Happy Hacking! 🦆💻**
