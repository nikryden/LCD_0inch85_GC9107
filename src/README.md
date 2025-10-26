# ESP32 LCD Display System - Clean Version

A production-ready, Android library compatible ESP32 display system with full transparency support and professional code structure.

## 🌟 Features

- **Multi-Display Support** - Control 1-8 GC9107 LCD displays (128x128)
- **Full Transparency Support** - Advanced transparency with bright green (RGB 0,255,0) key color
- **Memory Optimized** - Efficient memory management for continuous operation
- **Professional Structure** - Clean, modular code following best practices
- **Test Images Included** - Ready-to-use test images with transparency examples
- **Android Library Ready** - Organized structure for easy integration

## 📁 Project Structure

```
LCD_0inch85/
├── src/                          # Arduino library source files
│   ├── EasyDisplayManager.h/.cpp # Main display management
│   ├── ImageDisplay.h/.cpp       # Image loading with transparency
│   ├── LCD_Display.h/.cpp        # Individual display control
│   ├── DisplayManager.h/.cpp     # Multi-display coordination
│   └── ...                       # Supporting files
├── images/                       # PNG source images
│   ├── transparency_test.png     # Color blocks with transparency
│   └── gradient_test.png         # Gradient with transparency
├── convertedimages/              # RAW images for ESP32
│   ├── transparency_test.raw     # 32KB RGB565 format
│   └── gradient_test.raw         # 32KB RGB565 format
├── docs/                         # Documentation
├── examples/                     # Example configurations
├── LCD_0inch85_Clean.ino         # Clean production code
├── generate_test_images.py       # Image generation script
└── README.md                     # This file
```

## 🚀 Quick Start

### 1. Hardware Setup

- **ESP32-S3** microcontroller
- **GC9107 LCD displays** (128x128 pixels)
- **SD card** for image storage
- **SPI connections** as configured in code

### 2. Generate Test Images

```bash
# Install dependencies
pip install pillow numpy

# Generate test images
python generate_test_images.py
```

This creates:
- PNG images in `images/` directory
- RAW images in `convertedimages/` directory

### 3. Prepare SD Card

1. Format SD card (FAT32 recommended)
2. Copy `.raw` files from `convertedimages/` to SD card root
3. Insert SD card into ESP32

### 4. Upload Arduino Code

1. Open `LCD_0inch85_Clean.ino` in Arduino IDE
2. Configure `DISPLAY_COUNT` to match your setup
3. Upload to ESP32-S3

## 🎨 Test Images

### Transparency Test Image
- **File**: `transparency_test.raw`
- **Content**: Four colored quadrants (Red, Blue, Yellow, Magenta)
- **Transparency**: Center circle, diagonal stripes, and text areas
- **Purpose**: Test basic transparency functionality

### Gradient Test Image  
- **File**: `gradient_test.raw`
- **Content**: Radial rainbow gradient from center
- **Transparency**: Diagonal stripes, center circle, and fade-out edges
- **Purpose**: Test advanced transparency with gradients

## 🔧 Configuration

### Display Count
```cpp
#define DISPLAY_COUNT 2  // Change to match your setup (1-8)
```

### SD Card Pins
```cpp
#define SD_MISO_PIN  37
#define SD_CLK_PIN   36  
#define SD_MOSI_PIN  35
#define SD_CS_PIN    39
```

## 💡 Transparency System

### How It Works
1. **Transparency Key**: Bright green (RGB 0,255,0) pixels are transparent
2. **Background**: Black background appears white on LCD due to color transformation
3. **Detection**: Enhanced transparency detection with endianness support
4. **Rendering**: Transparent pixels preserve background, opaque pixels overlay

### Creating Custom Images
1. Create 128x128 PNG images
2. Use bright green (RGB 0,255,0) for transparent areas  
3. Convert to RAW using the provided Python script
4. Copy to SD card

## 📊 Memory Management

- **Automatic Optimization**: Background memory cleanup every 30 seconds
- **Efficient Loading**: Images loaded with minimal memory footprint
- **Continuous Operation**: System designed for 24/7 operation
- **Monitoring**: Built-in memory usage reporting

## 🛠️ Android Library Integration

### Code Structure
- **Modular Design**: Clean separation of concerns
- **Professional Error Handling**: Comprehensive error checking
- **Minimal Dependencies**: Standard Arduino libraries only
- **Optimized Performance**: 40MHz SPI, efficient algorithms

### Integration Steps
1. Copy `src/` directory to your Android project
2. Include header files as needed
3. Use `EasyDisplayManager` for simple integration
4. Customize configuration for your requirements

## 📈 Performance

- **SPI Speed**: 40MHz for fast image loading
- **Memory Usage**: ~50-70KB typical usage
- **Image Loading**: ~1-2 seconds for 128x128 image
- **Transparency**: Hardware-accelerated color transformation

## 🔍 Troubleshooting

### Common Issues

**Images not displaying:**
- Check SD card formatting (FAT32)
- Verify .raw file size is exactly 32,768 bytes
- Ensure SPI connections are correct

**White backgrounds instead of transparency:**
- This is normal behavior - black background appears white due to color transformation
- Transparent areas will show this white background

**Memory issues:**
- Monitor Serial output for memory reports
- System automatically optimizes memory usage
- Restart if persistent issues occur

### Debug Output
Enable Serial monitor (115200 baud) for detailed system information:
```
🎉 ESP32 LCD Display System - Clean Version
Initial memory: 245832 bytes
✅ SD card initialized successfully  
✅ Display system initialized successfully
🖼️ Loading images...
✅ Found 6893 transparent pixels (42.0%)
✅ Images loaded successfully
```

## 📋 Requirements

### Hardware
- ESP32-S3 (or compatible ESP32)
- GC9107 LCD displays (128x128)
- SD card (any size, FAT32 format)
- Proper SPI connections

### Software  
- Arduino IDE with ESP32 support
- Python 3.7+ (for image generation)
- Pillow and NumPy libraries

## 🤝 Contributing

1. Fork the repository
2. Create feature branch
3. Follow existing code style
4. Test thoroughly with provided test images
5. Submit pull request

## 📄 License

This project is provided as-is for educational and commercial use. Please maintain attribution when using in derivative works.

## 🎯 Next Steps

1. **Custom Images**: Create your own 128x128 images with transparency
2. **Multiple Displays**: Expand to larger display arrays
3. **Animation**: Implement image cycling and transitions
4. **Integration**: Use as library in larger Android projects

---

**Ready to create amazing transparent displays! 🎨✨**