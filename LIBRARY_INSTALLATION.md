# 📦 Library Installation Guide

## 🚀 Quick Installation

### Option 1: Arduino Library Manager (Easiest)
1. Open Arduino IDE
2. **Tools** → **Manage Libraries...**
3. Search: **"ESP32 LCD Multi-Display System"**
4. Click **Install**
5. Done! Examples available in **File** → **Examples**

### Option 2: Manual Installation
1. Download ZIP from GitHub
2. **Sketch** → **Include Library** → **Add .ZIP Library...**
3. Select the downloaded ZIP file
4. Restart Arduino IDE

### Option 3: Git Clone
```bash
cd ~/Documents/Arduino/libraries/
git clone https://github.com/nikryden/LCD_0inch85_GC9107.git
```

## 📁 Library Structure

After installation, you'll have:
```
Arduino/libraries/LCD_0inch85_GC9107/
├── library.properties          # Library metadata
├── keywords.txt                # Arduino IDE syntax highlighting
├── src/                        # Library source code
│   ├── EasyDisplayManager.h    # Main user-friendly interface
│   ├── DisplayManager.h        # Advanced multi-display control
│   ├── ImageDisplay.h          # Image loading and transparency
│   └── ...                     # Supporting files
├── examples/                   # Example sketches
│   ├── Ultra_Simple_Display_System/
│   ├── Advanced_Display_Example/
│   └── ...
└── docs/                       # Complete documentation
```

## 🎯 Using the Library

### In Your Sketch
```cpp
#include <EasyDisplayManager.h>

// For beginners - ultra simple
EasyDisplayManager* display = new EasyDisplayManager(DUAL_DISPLAY);

void setup() {
    Serial.begin(115200);
    display->quickStart();  // Does everything automatically!
}

void loop() {
    // Your code here
}
```

### Include Statements
Choose what you need:
```cpp
#include <EasyDisplayManager.h>     // User-friendly interface
#include <DisplayManager.h>         // Advanced control
#include <ImageDisplay.h>           // Image loading
#include <LCD_Display.h>            // Individual display control
```

## 🔍 Verify Installation

1. **Check Examples**: **File** → **Examples** → **ESP32 LCD Multi-Display System**
2. **Test Compilation**: Open and verify any example compiles without errors
3. **Serial Monitor**: Upload example and check Serial Monitor for system messages

## 🚨 Troubleshooting

### Library Not Found
- Restart Arduino IDE after installation
- Check libraries folder: `Documents/Arduino/libraries/`
- Verify ESP32 board package is installed

### Compilation Errors
- Select correct ESP32 board (ESP32-S3 Dev Module recommended)
- Check all dependencies are installed (SPI, SD libraries)
- See `docs/ARDUINO_CONFIGURATION.md` for complete setup

### Examples Not Showing
- Library must be in correct Arduino libraries directory
- Folder name should match library name
- Restart Arduino IDE completely

## 📖 Next Steps

1. **Quick Start**: Open `Ultra_Simple_Display_System` example
2. **Configuration**: Read `docs/ARDUINO_CONFIGURATION.md`
3. **Hardware Setup**: Follow `ARDUINO_SETUP_CHECKLIST.md`
4. **Documentation**: Explore `docs/` folder for complete guides

---

**✨ Ready to create amazing multi-display projects with ESP32!**