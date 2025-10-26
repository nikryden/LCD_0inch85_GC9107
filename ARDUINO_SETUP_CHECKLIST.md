# ✅ Arduino Setup Checklist

Quick checklist to get your ESP32 LCD Display System running in Arduino IDE.

## 🔧 Arduino IDE Configuration

### ✅ Step 1: Library Installation
- [ ] Arduino IDE installed (version 2.0+ recommended)
- [ ] Library installed via one of these methods:
  - [ ] **Library Manager**: Search "ESP32 LCD Multi-Display System" and install
  - [ ] **Manual**: Copy project to `Documents/Arduino/libraries/`
  - [ ] **Direct**: Copy `src/` files to your project folder

### ✅ Step 2: Board Manager Setup
- [ ] ESP32 board package installed
  - **URL**: `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`
  - **Package**: "ESP32 by Espressif Systems"

### ✅ Step 3: Board Settings
- [ ] **Board**: ESP32-S3 Dev Module (or ESP32 Dev Module)
- [ ] **Upload Speed**: 921600 (or 115200 if problems)
- [ ] **CPU Frequency**: 240MHz (WiFi/BT)
- [ ] **Flash Size**: 4MB (32Mb) or larger
- [ ] **Partition Scheme**: Default 4MB

### ✅ Step 4: Project Setup
- [ ] Example project opened (File → Examples → ESP32 LCD Multi-Display System)
- [ ] OR: Project folder created with library files
- [ ] SD card formatted as FAT32 (optional, for custom images)
- [ ] Images converted to RAW format if using custom images (use `conversion_tools/`)

## 🔌 Hardware Checklist

### ✅ Power Supply
- [ ] ESP32 board powered (USB or external 3.3V)
- [ ] External 3.3V supply for multiple displays (4+ displays)
- [ ] All grounds connected

### ✅ Display Connections
For single display (default pins):
- [ ] **CS** → GPIO 4
- [ ] **DC** → GPIO 46  
- [ ] **RST** → GPIO 3
- [ ] **BL** → GPIO 2
- [ ] **VCC** → 3.3V
- [ ] **GND** → Ground

### ✅ SD Card Connections
- [ ] **MISO** → GPIO 37
- [ ] **CLK** → GPIO 36
- [ ] **MOSI** → GPIO 35
- [ ] **CS** → GPIO 39
- [ ] **VCC** → 3.3V
- [ ] **GND** → Ground

## 🚀 First Upload Checklist

### ✅ Code Configuration
- [ ] `MY_DISPLAY_COUNT` set to correct number
- [ ] Pin definitions match your wiring
- [ ] Serial Monitor baud rate set to 115200

### ✅ Upload Process
- [ ] Correct COM port selected
- [ ] Upload successful (no error messages)
- [ ] Serial Monitor shows startup messages
- [ ] Display(s) show test colors or content

## 🔍 Troubleshooting Quick Fixes

### Upload Issues
- [ ] Try holding BOOT button during upload
- [ ] Reduce upload speed to 115200
- [ ] Check USB cable (must be data cable)
- [ ] Install USB-to-Serial drivers

### Display Issues
- [ ] Check all wiring connections
- [ ] Verify 3.3V power supply
- [ ] Try single display first
- [ ] Check Serial Monitor for error messages

### SD Card Issues
- [ ] Card formatted as FAT32
- [ ] Card properly inserted
- [ ] Try different SD card
- [ ] Check SD card wiring

## 📖 Need More Help?

- **Complete Guide**: `docs/ARDUINO_CONFIGURATION.md`
- **Examples**: `examples/Ultra_Simple_Display_System.ino`
- **Serial Monitor**: Always check for system messages and guidance
- **Built-in Help**: The system provides extensive troubleshooting wizards

---

*✨ Most issues are solved by double-checking wiring and Arduino board settings!*