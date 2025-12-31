#!/bin/bash

# Goose Hack Build Script
echo "🦆 Goose Hack Compilation Script"
echo "================================="

# Check if Arduino CLI is installed
if ! command -v arduino-cli &> /dev/null; then
    echo "❌ Arduino CLI not found. Please install it first."
    echo "   curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh"
    exit 1
fi

# Update core index
echo "📦 Updating Arduino core index..."
arduino-cli core update-index

# Install ESP32 core if not present
echo "🔧 Installing ESP32 core..."
arduino-cli core install esp32:esp32@2.0.5

# Install required libraries
echo "📚 Installing required libraries..."
arduino-cli lib install "NimBLE-Arduino"

# Compile the project
echo "🔨 Compiling Goose Hack..."
arduino-cli compile --fqbn esp32:esp32:esp32 \
    --build-property "build.partitions=default" \
    --build-property "upload.maximum_size=1310720" \
    --build-property "compiler.c.elf.extra_flags=-Wl,-zmuldefs" \
    --output-dir build/ \
    captive

if [ $? -eq 0 ]; then
    echo "✅ Compilation successful!"
    echo "📁 Binary files created in build/ directory"
    echo "🎯 Main binary: build/captive.ino.bin"
    echo ""
    echo "📤 To upload to ESP32:"
    echo "   esptool.py --chip esp32 --port /dev/ttyUSB0 --baud 921600 write_flash -z 0x1000 build/captive.ino.bin"
else
    echo "❌ Compilation failed!"
    exit 1
fi
