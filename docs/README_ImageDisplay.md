# ImageDisplay Library - Reusable LCD Image Loading

## 📚 Overview

The **ImageDisplay** library is a modular, reusable solution for loading images from SD cards to LCD displays, specifically designed for GC9107 displays with bit inversion behavior. This library was extracted from the main Arduino project to enable reuse across multiple projects.

## 🏗️ Architecture

```
LCD_0inch85.ino          # Main application (now streamlined)
├── ImageDisplay.h       # Library header with public interface
├── ImageDisplay.cpp     # Library implementation
├── LCD_Display.h        # LCD driver (dependency)
└── fonts.h             # Font definitions (dependency)
```

## 🎯 Key Features

- ✅ **Modular Design** - Completely self-contained library
- ✅ **Multiple Formats** - Raw RGB565 binary and C++ header files
- ✅ **Bit Inversion Fix** - Automatic color correction for GC9107 LCDs
- ✅ **Memory Optimized** - Smart heap management and progress tracking
- ✅ **Error Handling** - Comprehensive validation and user feedback
- ✅ **Color Definitions** - Includes standard RGB565 color constants
- ✅ **Reusable** - Works with any LCD_Display compatible project

## 📋 Library API Reference

### Core Functions

#### `bool displayImageOnDisplay(const char* filename, LCD_Display& display)`
**Purpose:** Load and display image file on LCD display  
**Parameters:**
- `filename` - Full path to image file on SD card (e.g., "/image.raw")
- `display` - Reference to initialized LCD_Display object  
**Returns:** `true` if successful, `false` on error  
**Formats:** Supports .raw (RGB565 binary) and .h (C++ header) files

#### `int findImageFiles(String* imageFiles, int maxFiles)`
**Purpose:** Search SD card for compatible image files  
**Parameters:**
- `imageFiles` - Array to store found filenames
- `maxFiles` - Maximum number of files to find  
**Returns:** Number of image files found  
**Searches:** .raw, .h, and .png files in root directory

#### `void displayErrorMessage(const char* message, LCD_Display& display)`
**Purpose:** Show formatted error message on LCD  
**Parameters:**
- `message` - Error text to display
- `display` - Reference to LCD display object  
**Features:** Automatic text wrapping and system styling

### Utility Functions

#### `uint16_t transformColorForLCD(uint16_t color)`
**Purpose:** Apply bit inversion for GC9107 LCD compatibility  
**Parameters:** `color` - Standard RGB565 color value  
**Returns:** Inverted color value for bit-inverting LCDs  
**Note:** Handles the ~color transformation automatically

#### `void displayInfoOnDisplay(const char* filename, size_t fileSize, LCD_Display& display)`
**Purpose:** Show file information when format is unsupported  
**Parameters:**
- `filename` - Name of file
- `fileSize` - Size in bytes  
- `display` - Reference to LCD display object

## 🚀 Usage Examples

### Basic Image Loading
```cpp
#include "ImageDisplay.h"

LCD_Display myDisplay(cs, dc, rst, bl);

void setup() {
    // Initialize display and SD card...
    
    // Load single image
    if (displayImageOnDisplay("/my_image.raw", myDisplay)) {
        Serial.println("Image loaded successfully!");
    }
}
```

### Multiple Image Search
```cpp
void loadMultipleImages() {
    String imageFiles[5];
    int count = findImageFiles(imageFiles, 5);
    
    Serial.printf("Found %d images\n", count);
    for (int i = 0; i < count; i++) {
        Serial.println("Image: " + imageFiles[i]);
    }
}
```

### Error Handling
```cpp
void loadWithErrorHandling(LCD_Display& display) {
    if (!displayImageOnDisplay("/image.raw", display)) {
        displayErrorMessage("LOAD FAILED", display);
        Serial.println("Check SD card and file format");
    }
}
```

### Custom Color Processing
```cpp
void processCustomColors() {
    uint16_t red = 0xF800;      // Standard red
    uint16_t lcdRed = transformColorForLCD(red);  // LCD-compatible red
    
    // Use lcdRed for direct pixel operations
    display.setPixel(x, y, lcdRed);
}
```

## 📁 File Format Support

### Raw RGB565 Binary (.raw)
- **Size:** Exactly 32,768 bytes (128×128×2)
- **Format:** Big-endian RGB565 data
- **Creation:** Use `png_to_raw_converter.py` 
- **Example:** `davinci_icon.raw`

### C++ Header Files (.h)
- **Format:** PROGMEM array declarations
- **Pattern:** `const uint8_t image_data[] PROGMEM = { 0x12, 0x34, ... };`
- **Size:** Variable (contains C++ syntax)
- **Example:** `davinci_icon.h`

## 🔧 Integration Steps

### 1. Add Library Files
Copy to your new project directory:
- `ImageDisplay.h`
- `ImageDisplay.cpp`

### 2. Use Example as Template
Copy `ImageDisplay_Example.cpp` to your new project:
- Rename it to `YourProject.ino`
- Adjust pin definitions for your hardware
- Modify examples as needed

### 3. Update Include Statements
```cpp
#include "ImageDisplay.h"  // Add this line
```

### 3. Replace Function Calls
**Old:** `displayImageOnDisplay(filename, displayNumber)`  
**New:** `displayImageOnDisplay(filename, displayObject)`

**Old:** `displayErrorMessage(message, displayNumber)`  
**New:** `displayErrorMessage(message, displayObject)`

### 4. Remove Duplicate Functions
Delete from main file (now in library):
- `transformColorForLCD()`
- `loadRawRGB565ImageToDisplay()`
- `loadHeaderFileImageToDisplay()`
- `findImageFiles()`
- `displayErrorMessage()`
- `displayInfoOnDisplay()`

## 📊 Library Configuration

### Memory Management
```cpp
#define MIN_FREE_HEAP     10000  // Minimum heap for safe operation
#define PROGRESS_INTERVAL 16     // Progress reporting frequency
#define YIELD_INTERVAL    16     // Watchdog yield frequency
```

### Image Specifications
```cpp
#define IMAGE_WIDTH       128    // LCD width in pixels
#define IMAGE_HEIGHT      128    // LCD height in pixels
#define RAW_IMAGE_SIZE    32768  // Expected raw file size
```

### Color Definitions
```cpp
#define LCD_WHITE   0xFFFF  // Standard RGB565 colors
#define LCD_BLACK   0x0000  // Automatically compatible with
#define LCD_RED     0xF800  // bit inversion transformation
#define LCD_GREEN   0x07E0
#define LCD_BLUE    0x001F
#define LCD_YELLOW  0xFFE0
#define LCD_CYAN    0x07FF
#define LCD_MAGENTA 0xF81F
```

## 🎨 Benefits of Modular Design

### Code Organization
- **Separation of Concerns** - Display logic isolated from application
- **Reusability** - Use in multiple projects without duplication
- **Maintainability** - Updates in one place benefit all projects
- **Testing** - Library can be tested independently

### Project Scalability
- **Clean Main File** - Application focuses on business logic
- **Library Evolution** - Add features without touching main code
- **Multiple Displays** - Easy to support different LCD types
- **Format Extensions** - Add new image formats in library

## 🔍 Technical Details

### Bit Inversion Discovery
The library automatically handles the GC9107 LCD bit inversion behavior discovered during development:
- **Problem:** LCD displays inverted colors (~RGB565)
- **Solution:** `transformColorForLCD()` applies `~color` compensation
- **Result:** Standard RGB565 colors display correctly

### Memory Optimization
- **Heap Monitoring** - Tracks available memory during loading
- **Progress Yielding** - Prevents watchdog timeouts on large images  
- **Bounded Parsing** - Fixed buffers prevent memory overflow
- **Error Recovery** - Graceful handling of low memory conditions

## 📝 Migration Notes

### From Monolithic to Modular

**Before (LCD_0inch85.ino):**
- 730+ lines with embedded image functions
- Display-specific implementations
- Mixed concerns (app logic + image processing)

**After (LCD_0inch85.ino + ImageDisplay library):**
- ~200 lines focused on application logic
- Reusable library with comprehensive API
- Clear separation of responsibilities

### Compatibility
- ✅ **Fully Compatible** - Same functionality, cleaner structure
- ✅ **No Behavior Changes** - Identical bit inversion and loading
- ✅ **Enhanced Error Handling** - Better user feedback
- ✅ **Performance Maintained** - Same memory usage patterns

The **ImageDisplay** library transforms your image loading code from project-specific functions into a professional, reusable component ready for any LCD display project! 🎯📱