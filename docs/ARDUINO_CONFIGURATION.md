# 🔧 Arduino Configuration Guide

## 📋 Table of Contents
- [Library Installation](#library-installation)
- [Arduino IDE Setup](#arduino-ide-setup)
- [ESP32 Board Configuration](#esp32-board-configuration)
- [Pin Configuration](#pin-configuration)
- [Library Dependencies](#library-dependencies)
- [Board Settings](#board-settings)
- [Hardware Requirements](#hardware-requirements)
- [Troubleshooting](#troubleshooting)

---

## � Library Installation

### Method 1: Arduino Library Manager (Recommended)
1. Open Arduino IDE
2. Go to **Tools** → **Manage Libraries...**
3. Search for **"ESP32 LCD Multi-Display System"**
4. Click **Install** on the latest version
5. All dependencies will be automatically installed

### Method 2: Manual Installation
1. Download this repository as ZIP or clone it
2. Copy the entire project folder to your Arduino libraries directory:
   - **Windows**: `Documents\Arduino\libraries\`
   - **macOS**: `~/Documents/Arduino/libraries/`
   - **Linux**: `~/Arduino/libraries/`
3. Restart Arduino IDE
4. The library will appear in **File** → **Examples** → **ESP32 LCD Multi-Display System**

### Method 3: Direct Project Setup
1. Create a new folder for your project
2. Copy all files from the `src/` folder to your project folder
3. Copy one of the example `.ino` files to your project folder
4. Open the `.ino` file in Arduino IDE

---

## �🚀 Arduino IDE Setup

### Step 1: Install Arduino IDE
- Download latest Arduino IDE from [arduino.cc](https://www.arduino.cc/en/software)
- Version 2.0+ recommended for best ESP32 support

### Step 2: Add ESP32 Board Support
1. Open Arduino IDE
2. Go to **File** → **Preferences**
3. Add this URL to "Additional Board Manager URLs":
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
4. Go to **Tools** → **Board** → **Boards Manager**
5. Search for "ESP32" and install **"ESP32 by Espressif Systems"**

---

## 🎯 ESP32 Board Configuration

### Recommended ESP32 Board Settings

| Setting | Value | Description |
|---------|-------|-------------|
| **Board** | ESP32-S3 Dev Module | Primary recommendation |
| **Upload Speed** | 921600 | Fastest upload speed |
| **CPU Frequency** | 240MHz (WiFi/BT) | Maximum performance |
| **Flash Mode** | QIO | Fastest flash access |
| **Flash Frequency** | 80MHz | Optimal for SPI displays |
| **Flash Size** | 4MB (32Mb) or larger | Minimum for this project |
| **Partition Scheme** | Default 4MB | Standard partitioning |
| **Core Debug Level** | None | For performance |
| **PSRAM** | Enabled (if available) | Improves performance |

### Alternative ESP32 Boards
| Board | Compatibility | Notes |
|-------|---------------|-------|
| ESP32 Dev Module | ✅ Full | Original ESP32 |
| ESP32-S2 | ✅ Full | Good alternative |
| ESP32-C3 | ⚠️ Limited | Fewer GPIO pins |
| ESP32-S3 | ✅ **Recommended** | Best performance |

---

## 📌 Pin Configuration

### Default Pin Assignments

#### SPI Configuration (Shared by all displays)
```cpp
// Standard SPI pins for ESP32-S3
#define SPI_MOSI_PIN    11    // Master Out Slave In
#define SPI_MISO_PIN    13    // Master In Slave Out (not used)
#define SPI_CLK_PIN     12    // SPI Clock
#define SPI_FREQUENCY   40000000  // 40MHz for optimal performance
```

#### SD Card Configuration
```cpp
// Dedicated SPI bus for SD card
#define SD_MISO_PIN     37    // SD Card MISO
#define SD_CLK_PIN      36    // SD Card Clock  
#define SD_MOSI_PIN     35    // SD Card MOSI
#define SD_CS_PIN       39    // SD Card Chip Select
```

#### Display Pin Configurations

##### Single Display Setup
```cpp
const DisplayPins SINGLE_CONFIG[] = {
    {cs: 4, dc: 46, rst: 3, bl: 2}
};
```

##### Dual Display Setup
```cpp
const DisplayPins DUAL_CONFIG[] = {
    {cs: 4, dc: 46, rst: 3, bl: 2},   // Display 1
    {cs: 5, dc: 46, rst: 6, bl: 15}  // Display 2
};
```

##### Quad Display Setup (2x2 Grid)
```cpp
const DisplayPins QUAD_CONFIG[] = {
    {cs: 4, dc: 46, rst: 3, bl: 2},   // Display 1 (Top-Left)
    {cs: 5, dc: 46, rst: 6, bl: 15},  // Display 2 (Top-Right)
    {cs: 7, dc: 46, rst: 8, bl: 16},  // Display 3 (Bottom-Left)
    {cs: 9, dc: 46, rst: 10, bl: 17} // Display 4 (Bottom-Right)
};
```

##### Full 8-Display Configuration
```cpp
const DisplayPins OCTA_CONFIG[] = {
    {cs: 4, dc: 46, rst: 3, bl: 2},    // Display 1
    {cs: 5, dc: 46, rst: 6, bl: 15},   // Display 2
    {cs: 7, dc: 46, rst: 8, bl: 16},   // Display 3
    {cs: 9, dc: 46, rst: 10, bl: 17},  // Display 4
    {cs: 11, dc: 46, rst: 12, bl: 42}, // Display 5
    {cs: 13, dc: 46, rst: 14, bl: 45}, // Display 6
    {cs: 21, dc: 46, rst: 18, bl: 1},  // Display 7
    {cs: 38, dc: 46, rst: 19, bl: 47}  // Display 8
};
```

### Pin Function Explanation

| Pin Type | Purpose | Notes |
|----------|---------|-------|
| **CS** | Chip Select | Unique per display, selects active display |
| **DC** | Data/Command | Shared (pin 46), tells display if data is command or pixel data |
| **RST** | Reset | Unique per display, hardware reset control |
| **BL** | Backlight | Unique per display, brightness control (PWM capable) |

---

## 📚 Library Dependencies

### Required Libraries (Auto-included)
These libraries are included with the ESP32 Arduino package:

```cpp
#include <Arduino.h>     // Core Arduino functions
#include <SPI.h>         // SPI communication
#include <SD.h>          // SD card support
```

### Project-Specific Libraries
These are included in the `src/` folder:

```cpp
#include "EasyDisplayManager.h"  // Ultra user-friendly interface
#include "DisplayManager.h"      // Advanced multi-display management
#include "ImageDisplay.h"        // High-performance image loading
#include "LCD_Driver.h"          // Low-level GC9107 communication
#include "GUI_Paint.h"           // Graphics and drawing functions
```

### No External Dependencies Required!
✅ This project uses only standard Arduino libraries - no additional installations needed.

---

## ⚙️ Board Settings by Project Type

### For Beginners (`Ultra_Simple_Display_System.ino`)
```
Board: ESP32-S3 Dev Module
Upload Speed: 115200 (more reliable)
CPU Frequency: 240MHz (WiFi/BT)
Flash Mode: QIO
Flash Size: 4MB (32Mb)
Partition Scheme: Default 4MB
Core Debug Level: Info (helpful for troubleshooting)
```

### For Advanced Users (`Advanced_Display_Example.ino`)
```
Board: ESP32-S3 Dev Module
Upload Speed: 921600 (fastest)
CPU Frequency: 240MHz (WiFi/BT)
Flash Mode: QIO
Flash Frequency: 80MHz
Flash Size: 8MB (64Mb) or 16MB (128Mb)
Partition Scheme: 8M Flash (3MB APP/1.5MB SPIFFS)
Core Debug Level: None (maximum performance)
PSRAM: Enabled
```

### For Development/Debugging
```
Board: ESP32-S3 Dev Module
Upload Speed: 115200
CPU Frequency: 240MHz (WiFi/BT)
Flash Mode: QIO
Flash Size: 4MB (32Mb)
Core Debug Level: Debug (maximum information)
Erase All Flash Before Sketch Upload: Enabled
```

---

## 🔌 Hardware Requirements

### Minimum Requirements
- **ESP32** (any variant)
- **1-8 GC9107 LCD displays** (128x128 pixels)
- **MicroSD card** (for image storage)
- **Breadboard and jumper wires**
- **3.3V power supply** (ESP32 can provide this)

### Recommended Hardware
- **ESP32-S3** development board
- **Multiple GC9107 displays** with SPI interface
- **High-speed microSD card** (Class 10 or better)
- **Dedicated 3.3V power supply** for multiple displays
- **Logic level converter** (if using 5V Arduino)

### Power Considerations
| Display Count | Current Draw | Recommendation |
|---------------|--------------|----------------|
| 1 display | ~50-100mA | ESP32 USB power OK |
| 2-3 displays | ~150-300mA | External 3.3V recommended |
| 4+ displays | ~400mA+ | **External power required** |

---

## 🔧 Compilation Settings

### Optimize for Performance
```cpp
// In your sketch, add these compiler optimizations
#pragma GCC optimize("O3")          // Maximum optimization
#define CORE_DEBUG_LEVEL 0          // Disable debug output
```

### Memory Optimization
```cpp
// For large image projects
#define CONFIG_SPIRAM_CACHE_WORKAROUND 1
#define CONFIG_SPIRAM_USE_MALLOC 1
```

---

## 🚨 Troubleshooting

### Common Upload Issues

#### "Failed to connect to ESP32"
**Solutions:**
1. Hold **BOOT** button while clicking upload
2. Try lower upload speed (115200)
3. Check USB cable (data cable, not charging-only)
4. Install USB-to-Serial drivers for your board

#### "Sketch too big"
**Solutions:**
1. Select larger flash size (8MB or 16MB)
2. Change partition scheme to give more app space
3. Enable compiler optimizations

### Common Runtime Issues

#### "No displays detected"
**Check:**
1. Wiring connections (especially power and ground)
2. Pin definitions match your hardware
3. SPI connections are correct
4. Displays are getting 3.3V power

#### "SD card not found"
**Check:**
1. SD card is formatted as FAT32
2. SD card pin connections
3. SD card is inserted properly
4. Try different SD card

#### "Images not loading"
**Check:**
1. Images are in RAW format (use provided converters)
2. Images are 128x128 pixels
3. Images are in root folder of SD card
4. Filenames match code expectations

### Performance Issues

#### "Slow image loading"
**Solutions:**
1. Use high-speed SD card (Class 10+)
2. Enable PSRAM if available
3. Increase SPI frequency
4. Use memory optimization features

#### "Display flickering"
**Solutions:**
1. Check power supply stability
2. Reduce SPI frequency if too high
3. Add decoupling capacitors
4. Check wiring for interference

---

## 📖 Example Configurations

### Quick Test Configuration
```cpp
// Minimal setup for testing
#define MY_DISPLAY_COUNT 1
EasyDisplayManager* system = new EasyDisplayManager(SINGLE_DISPLAY);
system->quickStart();
```

### Production Configuration  
```cpp
// Advanced setup with error handling
#define MY_DISPLAY_COUNT 4
EasyDisplayManager* system = new EasyDisplayManager(QUAD_DISPLAY);
if (!system->smartSetup()) {
    system->troubleshootingWizard();
}
```

### Custom Pin Configuration
```cpp
// Define your own pin layout
const DisplayPins CUSTOM_CONFIG[] = {
    {cs: 10, dc: 9, rst: 8, bl: 7},   // Your custom pins
    {cs: 6,  dc: 9, rst: 5, bl: 4}   // DC can be shared
};
DisplayManager* manager = new DisplayManager(CUSTOM_CONFIG, 2);
```

---

## 🎯 Next Steps

1. **Choose your board settings** based on your experience level
2. **Wire your displays** according to the pin configurations
3. **Upload one of the example sketches**
4. **Check Serial Monitor** for setup guidance and troubleshooting
5. **Add your own images** using the conversion tools in `examples/conversion_tools/`

---

## 💡 Pro Tips

- **Start small**: Begin with 1 display, then add more
- **Use Serial Monitor**: The system provides excellent debugging information
- **Check power**: Multiple displays need adequate power supply
- **Test connections**: Use the built-in hardware test functions
- **Read the examples**: Each example includes detailed comments and guidance

---

*For more detailed information, see the other documentation files in the `docs/` folder and examples in the `examples/` folder.*