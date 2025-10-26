# ImageDisplay Library

A modular ESP32 library for loading images from SD cards to GC9107 LCD displays.

## Library Files

### Core Library Files (Required)
- `ImageDisplay.h` - Header with function declarations and constants
- `ImageDisplay.cpp` - Implementation of all image loading functions
- `fonts.h` - Font definitions header

### Font Files (Required)
- `font8.cpp` - Small ASCII font (8pt)
- `font16.cpp` - Medium ASCII font (16pt, used by error messages)
- `font20.cpp` - Large ASCII font (20pt)
- `font24.cpp` - Extra large ASCII font (24pt)

### LCD Driver Files (Required)
- `LCD_Display.h` - LCD driver header
- `LCD_Driver.cpp` - LCD driver implementation

### Usage Examples
- `ImageDisplay_Example.txt` - Standalone project template (rename to .ino for new projects)

## Key Features

- **Multiple Image Formats**: Raw RGB565 binary files and C header files
- **Color Transformation**: Automatic bit inversion compensation for GC9107 displays
- **Memory Management**: Optimized loading with heap monitoring
- **Error Handling**: Comprehensive validation and user-friendly error messages
- **Progress Tracking**: Real-time loading progress via Serial output

## Quick Start

1. Include the library in your Arduino project:
   ```cpp
   #include "ImageDisplay.h"
   ```

2. Load an image to a display:
   ```cpp
   displayImageOnDisplay("/image.raw", display1);
   ```

3. Find available images:
   ```cpp
   String imageFiles[10];
   int count = findImageFiles(imageFiles, 10);
   ```

## Hardware Requirements

- ESP32 microcontroller
- GC9107 LCD displays (128x128)
- SD card module
- SPI connections for both LCD and SD card

## Supported Image Formats

- **Raw RGB565**: 32,768 bytes (128x128x2)
- **C Header Files**: Arrays of RGB565 hex data
- **Future**: PNG support (placeholder)

## Chinese Font Issue Resolution

**Note**: This library version has removed Chinese font support to prevent compilation issues:
- Removed `font12.cpp` (contained Chinese Font12CN)
- Removed `font24CN.cpp` (contained Chinese Font24CN)  
- Updated `fonts.h` to remove Chinese font declarations
- Replaced Font12 usage with Font16 for ASCII-only support

If you need Chinese character support, you'll need to implement it separately or use a different font library.