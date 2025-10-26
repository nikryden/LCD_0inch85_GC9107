# Quick Start Guide - ImageDisplay Library

## 🚀 How to Use the Library in Your Projects

### 📁 **File Organization**

Your current project structure:
```
LCD_0inch85/
├── ImageDisplay.h              # ✅ Library header
├── ImageDisplay.cpp            # ✅ Library implementation  
├── ImageDisplay_Example.cpp    # 📖 Reference example (don't compile)
├── LCD_0inch85.ino            # 🎯 Main project (compiles)
└── README_ImageDisplay.md     # 📚 Full documentation
```

### ⚠️ **Important: Arduino IDE Behavior**

Arduino IDE automatically compiles **all .ino files** in the same folder together. This is why:
- ✅ `LCD_0inch85.ino` - Main project (Arduino will compile this)
- ❌ `ImageDisplay_Example.cpp` - Example only (Arduino ignores .cpp files without .ino)

### 🔧 **Using the Library in New Projects**

#### **Step 1: Create New Project**
1. Create a new folder for your project (e.g., `MyLCDProject/`)
2. Copy these files to your new project:
   - `ImageDisplay.h` (library header)
   - `ImageDisplay.cpp` (library implementation)
   - `ImageDisplay_Example.cpp` (example template)
   - `fonts.h` (font definitions)
   - `LCD_Display.h` (LCD driver header)

#### **Step 2: Set Up Main File**
```bash
# In your new project folder
1. Rename ImageDisplay_Example.cpp to MyLCDProject.ino
2. Edit pin definitions to match your hardware
3. Customize the examples for your needs
```

#### **Step 3: Use the Library**
```cpp
// In your .ino file
#include "ImageDisplay.h"

LCD_Display myDisplay(cs, dc, rst, bl);

void setup() {
    // Initialize hardware...
    
    // Use library functions
    if (displayImageOnDisplay("/image.raw", myDisplay)) {
        Serial.println("Success!");
    }
}
```

### 📋 **Library API Quick Reference**

```cpp
// Load image from SD card to display
bool displayImageOnDisplay(const char* filename, LCD_Display& display);

// Find available image files
int findImageFiles(String* imageFiles, int maxFiles);

// Show error message on display  
void displayErrorMessage(const char* message, LCD_Display& display);

// Apply color transformation for GC9107 LCD
uint16_t transformColorForLCD(uint16_t color);

// Standard RGB565 colors (included in library)
// LCD_WHITE, LCD_BLACK, LCD_RED, LCD_GREEN, LCD_BLUE
// LCD_YELLOW, LCD_CYAN, LCD_MAGENTA
```

### 🔍 **Troubleshooting**

#### **Compilation Errors**
- ❌ **"redefinition of variable"** - You have multiple .ino files in same folder
- ✅ **Solution:** Move example to separate project folder

#### **Library Not Found**
- ❌ **"ImageDisplay.h: No such file"** - Library files not in project folder
- ✅ **Solution:** Copy ImageDisplay.h and ImageDisplay.cpp to your project

#### **Display Not Working**
- ❌ **Colors wrong or display blank** - Check pin definitions and SD card
- ✅ **Solution:** Verify hardware connections match your pin definitions

### 💡 **Example Project Structure**

For a new project using the library:
```
MyLCDProject/              # Your new project folder
├── MyLCDProject.ino       # Your main file (copied from example)
├── ImageDisplay.h         # Library header
├── ImageDisplay.cpp       # Library implementation
├── fonts.h               # Font definitions
├── LCD_Display.h         # LCD driver (if not installed as library)
└── images/               # Your image files
    ├── image1.raw
    └── image2.raw
```

### 🎯 **Ready to Go!**

The library is now properly organized to prevent compilation conflicts while providing a complete, reusable solution for LCD image display projects! 

**Main Project:** `LCD_0inch85.ino` works perfectly ✅  
**New Projects:** Use `ImageDisplay_Example.cpp` as template ✅  
**Library:** Ready for reuse across multiple projects ✅