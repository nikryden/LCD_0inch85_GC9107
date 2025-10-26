/*****************************************************************************
* | File        :   ImageDisplay.cpp
* | Author      :   Niklas Rydén
* | Function    :   Implementation of reusable image display functionality
* | Info        :   Modular library for loading images from SD to LCD displays
* | Hardware    :   ESP32, GC9107 LCDs, SD Card
* | 
* | Dependencies:
* |   - Arduino.h (ESP32 core)
* |   - SD.h (SD card support)
* |   - LCD_Display.h (LCD driver)
* |   - fonts.h (Font definitions)
******************************************************************************/

#include "ImageDisplay.h"

// =============================================================================
// COLOR TRANSFORMATION IMPLEMENTATION
// =============================================================================

uint16_t transformColorForLCD(uint16_t color) {
    // Apply complete bit inversion to compensate for LCD hardware behavior
    // This ensures standard RGB565 colors display correctly on bit-inverting GC9107 LCDs
    return ~color;
}

// =============================================================================
// TRANSPARENCY SUPPORT FUNCTIONS
// =============================================================================

bool isTransparentColor(uint16_t color) {
    // Check if color matches the transparency key (before transformation)
    if (color == TRANSPARENT_COLOR) {
        return true;
    }
    
    // Also check byte-swapped version in case of endianness issues
    uint16_t swappedColor = ((color & 0xFF) << 8) | ((color >> 8) & 0xFF);
    if (swappedColor == TRANSPARENT_COLOR) {
        return true;
    }
    
    return false;
}

uint16_t blendColors(uint16_t foreground, uint16_t background, uint8_t alpha) {
    if (alpha == 0) return background;  // Fully transparent
    if (alpha == 255) return foreground; // Fully opaque
    
    // Extract RGB components from RGB565 format
    // Foreground color
    uint8_t fR = (foreground >> 11) & 0x1F;  // 5 bits
    uint8_t fG = (foreground >> 5) & 0x3F;   // 6 bits  
    uint8_t fB = foreground & 0x1F;          // 5 bits
    
    // Background color
    uint8_t bR = (background >> 11) & 0x1F;  // 5 bits
    uint8_t bG = (background >> 5) & 0x3F;   // 6 bits
    uint8_t bB = background & 0x1F;          // 5 bits
    
    // Alpha blend each component
    uint8_t blendR = ((fR * alpha) + (bR * (255 - alpha))) / 255;
    uint8_t blendG = ((fG * alpha) + (bG * (255 - alpha))) / 255;
    uint8_t blendB = ((fB * alpha) + (bB * (255 - alpha))) / 255;
    
    // Reconstruct RGB565
    return ((blendR & 0x1F) << 11) | ((blendG & 0x3F) << 5) | (blendB & 0x1F);
}

// =============================================================================
// PUBLIC IMAGE LOADING FUNCTIONS
// =============================================================================

bool displayImageOnDisplay(const char* filename, LCD_Display& display) {
    // Quick validation
    if (!filename || strlen(filename) == 0 || !SD.cardType()) {
        return false;
    }
    
    if (!validateDisplayForImageLoading(display)) {
        return false;
    }
    
    // Open file with minimal logging
    File file = SD.open(filename);
    if (!file) {
        Serial.printf("❌ File not found: %s\n", filename);
        return false;
    }
    
    bool result = false;
    
    // Fast format detection and loading
    if (file.size() == RAW_IMAGE_SIZE) {
        result = loadRawRGB565ImageToDisplay(file, display);
    }
    else if (getFileExtension(filename) == "h") {
        result = loadHeaderFileImageToDisplay(file, display);
    }
    else {
        displayInfoOnDisplay(filename, file.size(), display);
        file.close();
        return true;
    }
    
    return result;
}

int findImageFiles(String* imageFiles, int maxFiles) {
    if (!imageFiles || maxFiles <= 0) {
        return 0;
    }
    
    File root = SD.open("/");
    if (!root) {
        return 0;
    }
    
    int fileCount = 0;
    File file = root.openNextFile();
    
    // Fast file scanning with minimal logging
    while (file && fileCount < maxFiles) {
        if (!file.isDirectory()) {
            String filename = String(file.name());
            
            if (isSupportedImageFormat(filename.c_str())) {
                imageFiles[fileCount] = "/" + filename;
                fileCount++;
            }
        }
        file = root.openNextFile();
    }
    
    root.close();
    Serial.printf("📂 Found %d image files\n", fileCount);
    return fileCount;
}

void displayErrorMessage(const char* message, LCD_Display& display) {
    if (!display.isInitialized()) {
        Serial.println("⚠️  Cannot display error - display not initialized");
        return;
    }
    
    display.clear(LCD_WHITE);
    display.newImage(IMAGE_WIDTH, IMAGE_HEIGHT, ROTATE_0, LCD_WHITE);
    
    // Error header
    display.drawString(10, 20, "ERROR", &Font16, LCD_RED, LCD_WHITE);
    
    // Error message (split long messages)
    String msg = String(message);
    if (msg.length() <= 12) {
        display.drawString(10, 50, message, &Font16, LCD_BLACK, LCD_WHITE);
    } else {
        // Split message for better display
        display.drawString(10, 45, msg.substring(0, 12).c_str(), &Font16, LCD_BLACK, LCD_WHITE);
        if (msg.length() > 12) {
            display.drawString(10, 65, msg.substring(12).c_str(), &Font16, LCD_BLACK, LCD_WHITE);
        }
    }
    
    // Status info
    display.drawString(10, 90, "Check SD card", &Font8, LCD_BLUE, LCD_WHITE);
    display.drawString(10, 105, "and files", &Font8, LCD_BLUE, LCD_WHITE);
    
    display.display();
}

void displayInfoOnDisplay(const char* filename, size_t fileSize, LCD_Display& display) {
    if (!display.isInitialized()) {
        Serial.println("⚠️  Cannot display info - display not initialized");
        return;
    }
    
    display.clear(LCD_WHITE);
    display.newImage(IMAGE_WIDTH, IMAGE_HEIGHT, ROTATE_0, LCD_WHITE);
    
    // Header
    display.drawString(10, 10, "FILE INFO", &Font16, LCD_BLACK, LCD_WHITE);
    
    // Extract and display filename (shortened if needed)
    String shortName = String(filename);
    if (shortName.startsWith("/")) {
        shortName = shortName.substring(1);
    }
    if (shortName.length() > 15) {
        shortName = shortName.substring(0, 15) + "...";
    }
    
    display.drawString(5, 35, shortName.c_str(), &Font8, LCD_BLACK, LCD_WHITE);
    display.drawString(5, 50, ("Size: " + String(fileSize)).c_str(), &Font8, LCD_RED, LCD_WHITE);
    
    // Expected format info
    display.drawString(5, 70, "Expected:", &Font8, LCD_BLACK, LCD_WHITE);
    display.drawString(5, 85, "32768 bytes", &Font8, LCD_BLACK, LCD_WHITE);
    display.drawString(5, 100, "RGB565 raw", &Font8, LCD_MAGENTA, LCD_WHITE);
    display.drawString(5, 115, "or .h file", &Font8, LCD_MAGENTA, LCD_WHITE);
    
    display.display();
}

// =============================================================================
// FORMAT-SPECIFIC LOADING IMPLEMENTATIONS
// =============================================================================

bool loadRawRGB565ImageToDisplay(File& file, LCD_Display& display) {
    // Validate prerequisites (minimal logging)
    if (!validateDisplayForImageLoading(display)) {
        file.close();
        return false;
    }
    
    // Initialize image buffer with black background for transparency
    display.newImage(IMAGE_WIDTH, IMAGE_HEIGHT, ROTATE_0, transformColorForLCD(LCD_BLACK));
    
    // Pre-allocate buffers for optimized loading
    uint8_t rowBuffer[ROW_BUFFER_SIZE * BYTES_PER_PIXEL];  // Buffer for one complete row
    uint16_t pixelBuffer[ROW_BUFFER_SIZE];                 // Converted pixel buffer
    
    Serial.printf("🚀 Loading %dx%d image with transparency support\n", IMAGE_WIDTH, IMAGE_HEIGHT);
    
    // Transparency tracking
    int transparentPixelCount = 0;
    int totalPixelCount = 0;
    
    // Optimized row-by-row loading
    for (int y = 0; y < IMAGE_HEIGHT; y++) {
        // Read entire row in one operation
        size_t bytesToRead = IMAGE_WIDTH * BYTES_PER_PIXEL;
        size_t bytesRead = file.read(rowBuffer, bytesToRead);
        
        if (bytesRead != bytesToRead) {
            Serial.printf("❌ Row %d read error: got %d/%d bytes\n", y, bytesRead, bytesToRead);
            file.close();
            return false;
        }
        
        // Convert entire row with transparency support
        for (int x = 0; x < IMAGE_WIDTH; x++) {
            int bufferIndex = x * BYTES_PER_PIXEL;
            uint16_t color = (rowBuffer[bufferIndex] << 8) | rowBuffer[bufferIndex + 1];
            
            // Store original color for transparency check and transformed color for display
            uint16_t originalColor = color;
            totalPixelCount++;
            
            // Check for transparency BEFORE transformation
            if (isTransparentColor(originalColor)) {
                transparentPixelCount++;
    
                // Use a safe marker that's very unlikely to occur naturally
                pixelBuffer[x] = 0xDEAD; // Dead marker - very unlikely to naturally occur
            } else {
                pixelBuffer[x] = transformColorForLCD(originalColor);
            }
        }
        
        // Write entire row to display in batch (skip transparent pixels)
        for (int x = 0; x < IMAGE_WIDTH; x++) {
            if (pixelBuffer[x] != 0xDEAD) { // Not the transparent marker
                display.setPixel(x, y, pixelBuffer[x]);
            }
            // Transparent pixels are left as-is (preserving background)
        }
        
        // Minimal progress reporting (less frequent)
        if ((y % PROGRESS_INTERVAL) == 0) {
            Serial.printf("⚡ Row %d/%d\n", y, IMAGE_HEIGHT);
        }
        
        // Reduced yield frequency for better performance
        if ((y % YIELD_INTERVAL) == 0) {
            yield();
        }
    }
    
    // Single display update at the end
    display.display();
    file.close();
    
    // Report transparency statistics
    if (transparentPixelCount > 0) {
        float transparentPercentage = (transparentPixelCount * 100.0f / totalPixelCount);
        Serial.printf("✅ Found %d transparent pixels (%.1f%%)\n", transparentPixelCount, transparentPercentage);
    }
    
    Serial.println("✅ Fast load complete");
    return true;
}

bool loadHeaderFileImageToDisplay(File& file, LCD_Display& display) {
    Serial.printf("🔄 Starting header file parsing - Free heap: %d bytes\n", ESP.getFreeHeap());
    
    // Validate prerequisites
    if (!validateDisplayForImageLoading(display)) {
        file.close();
        return false;
    }
    
    // Initialize image buffer with black background for transparency
    display.newImage(IMAGE_WIDTH, IMAGE_HEIGHT, ROTATE_0, transformColorForLCD(LCD_BLACK));
    
    // Parsing state variables
    char lineBuffer[MAX_LINE_BUFFER];
    int linePos = 0;
    bool foundDataStart = false;
    int pixelCount = 0;
    int x = 0, y = 0;
    
    // Pixel assembly variables (non-static for thread safety)
    uint8_t pixelData[BYTES_PER_PIXEL] = {0, 0};
    int byteIndex = 0;
    
    Serial.println("📄 Parsing header file with bounded buffer...");
    
    while (file.available() && pixelCount < (IMAGE_WIDTH * IMAGE_HEIGHT)) {
        char c = file.read();
        
        // Handle line endings
        if (c == '\n' || c == '\r') {
            if (linePos > 0) {
                lineBuffer[linePos] = '\0'; // Null terminate
                
                // Look for data array start (multiple format support)
                if (!foundDataStart && (strstr(lineBuffer, "_data[] PROGMEM = {") != NULL || 
                                        strstr(lineBuffer, "_data[] = {") != NULL)) {
                    foundDataStart = true;
                    Serial.println("🎯 Found data array declaration");
                }
                // Process hex data lines
                else if (foundDataStart && strstr(lineBuffer, "0x") != NULL) {
                    // Parse hex values using safe C functions
                    char* pos = lineBuffer;
                    while (*pos && pixelCount < (IMAGE_WIDTH * IMAGE_HEIGHT)) {
                        char* hexPos = strstr(pos, "0x");
                        if (!hexPos) break;
                        
                        // Extract 2-digit hex value safely
                        char hexStr[3] = {0};
                        if (hexPos[2] && hexPos[3]) {
                            hexStr[0] = hexPos[2];
                            hexStr[1] = hexPos[3];
                            hexStr[2] = '\0';
                            
                            uint8_t byteVal = (uint8_t)strtol(hexStr, NULL, 16);
                            
                            // Assemble RGB565 pixel (2 bytes)
                            pixelData[byteIndex] = byteVal;
                            byteIndex++;
                            
                            if (byteIndex == BYTES_PER_PIXEL) {
                                // Complete pixel ready
                                uint16_t color = (pixelData[0] << 8) | pixelData[1];
                                
                                // Check for transparency before setting pixel
                                if (!isTransparentColor(color)) {
                                    // Apply LCD-specific color transformation and set pixel
                                    color = transformColorForLCD(color);
                                    display.setPixel(x, y, color);
                                }
                                // Transparent pixels are skipped, preserving background
                                
                                // Advance position
                                x++;
                                if (x >= IMAGE_WIDTH) {
                                    x = 0;
                                    y++;
                                    if (y >= IMAGE_HEIGHT) break;
                                    
                                    // Progress reporting
                                    if ((y % PROGRESS_INTERVAL) == 0) {
                                        int progress = (y * 100) / IMAGE_HEIGHT;
                                        Serial.printf("📊 Parsing progress: %d%%, Free heap: %d bytes\n", 
                                                     progress, ESP.getFreeHeap());
                                    }
                                }
                                
                                pixelCount++;
                                byteIndex = 0;
                                
                                // Reduced yield frequency for better performance
                                if ((pixelCount % (YIELD_INTERVAL * 32)) == 0) {
                                    yield();
                                }
                            }
                        }
                        
                        pos = hexPos + 4; // Move past current hex value
                    }
                }
                // Check for end of data array
                else if (foundDataStart && strstr(lineBuffer, "};") != NULL) {
                    Serial.println("🏁 Found end of data array");
                    break;
                }
                
                linePos = 0; // Reset line buffer
            }
        }
        // Add character to line buffer with bounds checking
        else if (linePos < sizeof(lineBuffer) - 1) {
            lineBuffer[linePos++] = c;
        }
        // Handle buffer overflow gracefully
        else {
            Serial.println("⚠️  Line buffer overflow - skipping to next line");
            // Skip to next newline
            while (file.available() && c != '\n' && c != '\r') {
                c = file.read();
            }
            linePos = 0;
        }
    }
    
    Serial.printf("✅ Header parsing complete - %d pixels loaded\n", pixelCount);
    
    if (pixelCount > 0) {
        Serial.printf("🖥️  Displaying parsed image... Free heap: %d bytes\n", ESP.getFreeHeap());
        display.display();
        Serial.printf("✅ Header image display complete - Free heap: %d bytes\n", ESP.getFreeHeap());
    }
    
    file.close();
    return pixelCount > 0;
}

// =============================================================================
// UTILITY FUNCTION IMPLEMENTATIONS
// =============================================================================

bool validateDisplayForImageLoading(LCD_Display& display) {
    // Check display initialization
    if (!display.isInitialized()) {
        Serial.println("❌ Display not initialized - cannot load image");
        return false;
    }
    
    // Check available heap memory
    if (ESP.getFreeHeap() < MIN_FREE_HEAP) {
        Serial.printf("❌ Insufficient heap memory: %d bytes (need %d)\n", 
                     ESP.getFreeHeap(), MIN_FREE_HEAP);
        return false;
    }
    
    return true;
}

String getFileExtension(const char* filename) {
    if (!filename) return "";
    
    String fname = String(filename);
    int dotIndex = fname.lastIndexOf('.');
    
    if (dotIndex == -1 || dotIndex == fname.length() - 1) {
        return ""; // No extension or dot at end
    }
    
    String ext = fname.substring(dotIndex + 1);
    ext.toLowerCase();
    return ext;
}

bool isSupportedImageFormat(const char* filename) {
    if (!filename) return false;
    
    String ext = getFileExtension(filename);
    
    // Supported formats
    return (ext == "raw" || ext == "h" || ext == "png");
}

// =============================================================================
// MEMORY OPTIMIZATION IMPLEMENTATIONS
// =============================================================================

void clearImageCache(bool forceCleanup) {
    Serial.println("🧹 Clearing image cache and buffers...");
    
    int beforeFree = ESP.getFreeHeap();
    
    // Force memory cleanup using available ESP32 functions
    yield();
    delay(10);
    
    if (forceCleanup) {
        // Multiple passes for aggressive cleanup
        for (int i = 0; i < 3; i++) {
            yield();
            delay(10);
        }
        
        // Clear any static buffers or cached data
        // Note: Static arrays are automatically managed
        Serial.println("🧹 Aggressive cleanup performed");
    }
    
    int afterFree = ESP.getFreeHeap();
    int recovered = afterFree - beforeFree;
    
    if (recovered > 0) {
        Serial.printf("🧹 Image cache cleared: %d bytes recovered\n", recovered);
    }
}

int optimizeImageMemory() {
    int beforeFree = ESP.getFreeHeap();
    
    // Force memory optimization using available ESP32 functions
    yield();
    delay(10);
    
    // Additional cleanup passes
    for (int i = 0; i < 2; i++) {
        yield();
        delay(5);
    }
    
    int afterFree = ESP.getFreeHeap();
    int recovered = afterFree - beforeFree;
    
    if (recovered > 0) {
        Serial.printf("💾 Memory optimized: %d bytes recovered (now %d free)\n", 
                      recovered, afterFree);
    }
    
    return recovered;
}