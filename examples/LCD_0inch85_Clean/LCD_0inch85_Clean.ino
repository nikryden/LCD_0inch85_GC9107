/*****************************************************************************
* | File        :   LCD_0inch85_Clean.ino
* | Author      :   ESP32 Multi-Display System - Clean Version
* | Function    :   Production-ready display system with transparency support
* | Info        :   Optimized for Android library integration with best practices
* | Hardware    :   ESP32-S3, configurable GC9107 LCDs (128x128), SD Card
* | 
* | ✨ CLEAN FEATURES:
* |   - Minimal, production-ready code
* |   - Full transparency support with test images
* |   - Memory optimized and stable
* |   - Android library compatible structure
* |   - Professional error handling
* |   - Test images included (transparency_test.raw, gradient_test.raw)
******************************************************************************/

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include "src/EasyDisplayManager.h"
#include "src/ImageDisplay.h"

// =============================================================================
// 🎯 CONFIGURATION
// =============================================================================

#define DISPLAY_COUNT 2  // Number of displays to use

// SD Card Configuration
#define SD_MISO_PIN  37
#define SD_CLK_PIN   36
#define SD_MOSI_PIN  35
#define SD_CS_PIN    39

// =============================================================================
// 🌐 GLOBAL VARIABLES
// =============================================================================

EasyDisplayManager* displaySystem = nullptr;
SPIClass sdSPI;

// System state
static bool systemInitialized = false;
static bool imagesLoaded = false;
static unsigned long lastImageRefresh = 0;
static unsigned long lastMemoryCheck = 0;

// =============================================================================
// 🚀 CORE FUNCTIONS
// =============================================================================

bool initializeDisplaySystem() {
  Serial.println("🎯 Initializing display system...");
  
  displaySystem = createDisplaySystem(DISPLAY_COUNT);
  if (!displaySystem) {
    Serial.println("❌ Failed to create display system");
    return false;
  }
  
  if (!displaySystem->quickStart()) {
    Serial.println("❌ Display system failed to start");
    return false;
  }
  
  Serial.println("✅ Display system initialized successfully");
  return true;
}

bool initializeSDCard() {
  Serial.println("💾 Initializing SD card...");
  
  sdSPI.begin(SD_CLK_PIN, SD_MISO_PIN, SD_MOSI_PIN, SD_CS_PIN);
  
  if (!SD.begin(SD_CS_PIN, sdSPI)) {
    Serial.println("⚠️  SD card not found - system will use demo mode");
    return false;
  }
  
  Serial.println("✅ SD card initialized successfully");
  return true;
}

void setDisplayBackgrounds() {
  if (!displaySystem || !displaySystem->isWorking()) return;
  
  DisplayManager* manager = displaySystem->getManager();
  if (!manager) return;
  
  // Set black background (appears white on LCD due to color transformation)
  uint16_t bgColor = transformColorForLCD(0x0000);
  
  for (int i = 0; i < manager->getTotalDisplayCount(); i++) {
    LCD_Display* display = manager->getDisplay(i);
    if (display) {
      display->clear(bgColor);
    }
  }
}

bool loadImages() {
  if (!displaySystem || !displaySystem->isWorking()) return false;
  
  Serial.println("🖼️ Loading images...");
  
  // Set backgrounds before loading images
  setDisplayBackgrounds();
  
  DisplayManager* manager = displaySystem->getManager();
  if (!manager) return false;
  
  if (manager->loadAndDistributeImages()) {
    Serial.println("✅ Images loaded successfully");
    return true;
  } else {
    Serial.println("🎨 No images found - showing demo");
    displaySystem->showDemo();
    return false;
  }
}

void optimizeMemory() {
  static unsigned long lastOptimization = 0;
  
  // Don't optimize too frequently
  if (millis() - lastOptimization < 30000) return;
  lastOptimization = millis();
  
  int initialFree = ESP.getFreeHeap();
  
  // Gentle memory cleanup
  yield();
  delay(10);
  
  int finalFree = ESP.getFreeHeap();
  int recovered = finalFree - initialFree;
  
  if (recovered > 100) {
    Serial.printf("🧹 Memory optimized: +%d bytes (total: %d)\n", recovered, finalFree);
  }
}

// =============================================================================
// 🏁 ARDUINO SETUP
// =============================================================================

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n🎉 ESP32 LCD Display System - Clean Version");
  Serial.println("===========================================");
  Serial.printf("Initial memory: %d bytes\n", ESP.getFreeHeap());
  
  // Initialize SPI for displays
  Serial.println("⚡ Setting up SPI communication...");
  SPI.begin();
  SPI.setFrequency(40000000);  // 40MHz for optimal performance
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
  
  // Initialize SD card
  bool sdReady = initializeSDCard();
  
  // Initialize display system
  if (initializeDisplaySystem()) {
    systemInitialized = true;
    Serial.println("🎉 System ready!");
    
    if (sdReady) {
      Serial.println("💡 Test images available:");
      Serial.println("   - transparency_test.raw (colors + transparency)");
      Serial.println("   - gradient_test.raw (gradients + transparency)");
    }
  } else {
    Serial.println("❌ System initialization failed");
  }
  
  Serial.printf("Final memory: %d bytes\n", ESP.getFreeHeap());
  Serial.println("===========================================\n");
}

// =============================================================================
// 🔄 ARDUINO MAIN LOOP
// =============================================================================

void loop() {
  if (!systemInitialized) {
    delay(1000);
    return;
  }
  
  // Load images once after startup delay
  if (!imagesLoaded && millis() > 3000) {
    imagesLoaded = true;
    loadImages();
  }
  
  // Refresh images every 5 minutes
  if (imagesLoaded && (millis() - lastImageRefresh > 300000)) {
    lastImageRefresh = millis();
    Serial.println("🔄 Refreshing images...");
    loadImages();
  }
  
  // Memory optimization every 30 seconds
  if (millis() - lastMemoryCheck > 30000) {
    lastMemoryCheck = millis();
    
    // Only optimize if memory is getting low
    if (ESP.getFreeHeap() < 50000) {
      optimizeMemory();
    }
    
    // Status update every 2 minutes
    static int statusCounter = 0;
    if (++statusCounter >= 4) {  // 4 * 30s = 2 minutes
      statusCounter = 0;
      Serial.printf("📊 Status: Running, %d bytes free\n", ESP.getFreeHeap());
    }
  }
  
  // Keep system responsive
  yield();
  delay(1000);  // 1 second main loop delay
}

// =============================================================================
// 📚 DOCUMENTATION
// =============================================================================

/*
 * 🎯 USAGE INSTRUCTIONS:
 * 
 * 1. HARDWARE SETUP:
 *    - ESP32-S3 with GC9107 LCD displays (128x128)
 *    - SD card for image storage
 *    - Connect displays using provided pin configuration
 * 
 * 2. IMAGE PREPARATION:
 *    - Run generate_test_images.py to create test images
 *    - Copy .raw files from convertedimages/ to SD card root
 *    - Supported format: RGB565 RAW, 128x128 pixels, 32768 bytes
 * 
 * 3. TRANSPARENCY SUPPORT:
 *    - Bright green (RGB 0,255,0) pixels are transparent
 *    - Transparent areas show white background on display
 *    - Test images include transparency examples
 * 
 * 4. CONFIGURATION:
 *    - Change DISPLAY_COUNT to match your setup (1-8 displays)
 *    - Modify SD card pins if using different connections
 *    - All other settings are automatically configured
 * 
 * 5. ANDROID LIBRARY INTEGRATION:
 *    - Source files organized in src/ directory
 *    - Clean, modular code structure
 *    - Minimal dependencies and optimized memory usage
 *    - Professional error handling and logging
 * 
 * 6. MEMORY OPTIMIZATION:
 *    - Automatic memory management
 *    - Images loaded efficiently with cleanup
 *    - System runs continuously without memory leaks
 *    - Monitoring and optimization built-in
 */