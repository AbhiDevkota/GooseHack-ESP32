# Compilation Instructions for Goose Hack

## Required Libraries
Install these libraries in Arduino IDE:
- ESP32 Board Package (v2.0.5 by Espressif Systems)
- NimBLE-Arduino (latest version)

## Board Settings
- Board: ESP32 Dev Module
- Upload Speed: 921600
- CPU Frequency: 240MHz (WiFi/BT)
- Flash Frequency: 80MHz
- Flash Mode: QIO
- Flash Size: 4MB (32Mb)
- Partition Scheme: Default 4MB with spiffs
- Core Debug Level: None
- PSRAM: Disabled

## Arduino CLI Compilation Command
```bash
arduino-cli compile --fqbn esp32:esp32:esp32 Fixed/captive.ino --output-dir build/
```

## Manual Compilation Steps
1. Open Arduino IDE
2. Install ESP32 board package and NimBLE library
3. Open Fixed/captive.ino
4. Select ESP32 Dev Module board
5. Set the board settings above
6. Click Sketch -> Export Compiled Binary
7. The .bin file will be in Fixed/ folder

## Upload to ESP32
```bash
esptool.py --chip esp32 --port /dev/ttyUSB0 --baud 921600 write_flash -z 0x1000 captive.ino.esp32.esp32.esp32.bin
```

## File Structure
- captive.ino - Main file
- config.h - Configuration
- web_interface.h/.cpp - Web interface
- attacks.h/.cpp - Attack functions
- web_styles.h - CSS styles

## Features Included
✅ Deauth Attack
✅ Evil Twin
✅ Combined Attack (Deauth + Evil Twin)
✅ Beacon Spam (Target & Custom modes)
✅ Sour Apple (iOS attack)
✅ Windows Bluetooth attack
✅ Handshake Capture (Passive 80s timeout & Aggressive auto-stop)
✅ PCAP Download for hashcat cracking
✅ Professional web interface "Goose Hack"

## Memory Usage
Optimized for ESP32 with minimal RAM usage and PROGMEM storage for CSS.
