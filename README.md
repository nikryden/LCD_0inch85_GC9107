# GC9107 LCD Display Library

This is a library for using [LCD_0inch85_GC9107](https://www.aliexpress.com/item/1005007443164311.html) or other displays using the GC9107 driver chip.

## Features

- Support for GC9107 driver-based LCD displays (128x128 pixels typical)
- Hardware SPI interface for fast communication
- Basic graphics primitives (pixels, lines, rectangles)
- RGB565 color support
- Display rotation (0°, 90°, 180°, 270°)
- Display inversion
- Sleep mode support
- Compatible with Arduino, ESP32, and other platforms

## Hardware Requirements

- Arduino board (Uno, Mega, Nano, etc.) or ESP32
- GC9107-based LCD display (e.g., 0.85" LCD)
- Connections via SPI interface

## Wiring

Connect your GC9107 display to your microcontroller using SPI:

### Arduino Uno/Nano
| Display Pin | Arduino Pin |
|-------------|-------------|
| VCC         | 5V or 3.3V  |
| GND         | GND         |
| CS          | 10          |
| DC          | 9           |
| RST         | 8           |
| SDA (MOSI)  | 11          |
| SCL (SCK)   | 13          |

### ESP32
| Display Pin | ESP32 Pin   |
|-------------|-------------|
| VCC         | 3.3V        |
| GND         | GND         |
| CS          | GPIO 5      |
| DC          | GPIO 4      |
| RST         | GPIO 2      |
| SDA (MOSI)  | GPIO 23     |
| SCL (SCK)   | GPIO 18     |

*Note: Pin assignments can be customized in your code.*

## Installation

### Arduino IDE
1. Download this repository as a ZIP file
2. In Arduino IDE, go to **Sketch** → **Include Library** → **Add .ZIP Library**
3. Select the downloaded ZIP file
4. Restart Arduino IDE

### PlatformIO
Add to your `platformio.ini`:
```ini
lib_deps = 
    https://github.com/nikryden/LCD_0inch85_GC9107
```

## Usage

### Basic Example

```cpp
#include <GC9107.h>
#include <SPI.h>

// Define pins
#define TFT_CS   10
#define TFT_DC   9
#define TFT_RST  8

// Create display object
GC9107 display(TFT_CS, TFT_DC, TFT_RST);

void setup() {
  // Initialize display
  display.begin(128, 128);
  
  // Fill screen with blue
  display.fillScreen(GC9107_BLUE);
  
  // Draw a red rectangle
  display.fillRect(20, 20, 50, 50, GC9107_RED);
}

void loop() {
  // Your code here
}
```

## API Reference

### Initialization

#### `GC9107(int8_t cs, int8_t dc, int8_t rst = -1)`
Constructor to create a display object.
- `cs`: Chip Select pin
- `dc`: Data/Command pin
- `rst`: Reset pin (optional, use -1 if not connected)

#### `void begin(uint16_t width = 128, uint16_t height = 128)`
Initialize the display with specified dimensions.

### Display Control

#### `void setRotation(uint8_t rotation)`
Set display rotation (0-3 for 0°, 90°, 180°, 270°).

#### `void invertDisplay(bool invert)`
Invert display colors.

#### `void displayOn()` / `void displayOff()`
Turn display on or off.

#### `void sleepMode(bool enable)`
Enable or disable sleep mode (low power).

### Drawing Functions

#### `void fillScreen(uint16_t color)`
Fill entire screen with a color.

#### `void drawPixel(int16_t x, int16_t y, uint16_t color)`
Draw a single pixel.

#### `void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)`
Draw a filled rectangle.

#### `void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)`
Draw a horizontal line.

#### `void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)`
Draw a vertical line.

#### `void drawRGBBitmap(int16_t x, int16_t y, const uint16_t *bitmap, int16_t w, int16_t h)`
Draw an RGB565 bitmap image.

### Utility Functions

#### `uint16_t width()` / `uint16_t height()`
Get current display width/height.

### Predefined Colors (RGB565)

- `GC9107_BLACK`
- `GC9107_WHITE`
- `GC9107_RED`
- `GC9107_GREEN`
- `GC9107_BLUE`
- `GC9107_CYAN`
- `GC9107_MAGENTA`
- `GC9107_YELLOW`
- `GC9107_ORANGE`

## Examples

The library includes several examples:

1. **BasicTest** - Demonstrates basic drawing functions and color fills
2. **RotationTest** - Shows display rotation and inversion features

You can find these examples in the `examples` folder or through the Arduino IDE menu: **File** → **Examples** → **GC9107**.

## Display Specifications

- Driver: GC9107
- Resolution: 128x128 pixels (typical for 0.85" displays)
- Color depth: 16-bit RGB565 (65,536 colors)
- Interface: SPI
- Voltage: 3.3V (most common) or 5V tolerant

## Troubleshooting

### Display is blank
- Check wiring connections
- Verify power supply voltage (3.3V or 5V)
- Ensure RST pin is properly connected or set to -1 in constructor

### Wrong colors or artifacts
- Check SPI connections (MOSI, SCK)
- Verify CS and DC pins are correctly defined
- Try reducing SPI speed if you have long wires

### Display is dim
- Check backlight connection (BL pin, usually to 3.3V or 5V)
- Some displays have separate backlight control

## License

This library is released under the GNU General Public License v3.0. See [LICENSE](LICENSE) for details.

## Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues on GitHub.

## Credits

Developed by nikryden

## References

- [GC9107 Datasheet](https://www.buydisplay.com/)
- [AliExpress Product Link](https://www.aliexpress.com/item/1005007443164311.html)
