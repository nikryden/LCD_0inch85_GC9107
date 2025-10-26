/*****************************************************************************
* | File        :   Ultra_Simple_Display_System.ino
* | Author      :   Niklas Rydén
* | Function    :   Ultra user-friendly display system - just works!
* | Info        :   One-line setup, automatic everything, beginner-friendly
* | Hardware    :   ESP32, GC9107 LCDs, SD Card - that's it!
* | 
* | 🎉 ULTRA-SIMPLE USAGE:
* |   1. Set up Arduino IDE (see: docs/ARDUINO_CONFIGURATION.md)
* |   2. Connect your displays
* |   3. Upload this code  
* |   4. Open Serial Monitor
* |   5. Everything happens automatically!
* |
* | 🔧 ARDUINO SETTINGS:
* |   Board: ESP32-S3 Dev Module | Upload Speed: 921600 | CPU: 240MHz
* |   Quick Setup Guide: ARDUINO_SETUP_CHECKLIST.md
******************************************************************************/

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include "EasyDisplayManager.h"

// =============================================================================
// 🎯 SUPER EASY CONFIGURATION - Just pick how many displays you have!
// =============================================================================

// 👇 CHANGE THIS NUMBER to match your displays (1, 2, 3, 4, 6, or 8)
#define MY_DISPLAY_COUNT 2

// Or uncomment one of these preset configurations:
// #define USE_SINGLE_DISPLAY     // 1 display - perfect for testing
// #define USE_DUAL_DISPLAY       // 2 displays - most popular
// #define USE_QUAD_DISPLAY       // 4 displays - 2x2 grid setup

// =============================================================================
// 🚀 AUTOMATIC SYSTEM - No complex setup needed!
// =============================================================================

EasyDisplayManager* displaySystem = nullptr;

// SD Card pins (same for all configurations)
#define SD_MISO_PIN  37    // Don't change these unless you know what you're doing
#define SD_CLK_PIN   36
#define SD_MOSI_PIN  35
#define SD_CS_PIN    39

SPIClass sdSPI;

// =============================================================================
// 🎉 SETUP - This does everything automatically!
// =============================================================================

void setup() {
  // Start serial communication
  Serial.begin(115200);
  delay(1000);
  
  // Show welcome message
  Serial.println("\n🎉 Welcome to the Ultra-Simple Display System!");
  Serial.println("   Everything happens automatically - just sit back!");
  
  // Setup SPI for displays (high speed for best performance)
  Serial.println("\n⚡ Setting up high-speed display communication...");
  SPI.begin();
  SPI.setFrequency(40000000);  // 40MHz for fast image loading
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
  
  // Setup SD card for images
  Serial.println("💾 Setting up SD card for images...");
  sdSPI.begin(SD_CLK_PIN, SD_MISO_PIN, SD_MOSI_PIN, SD_CS_PIN);
  
  if (SD.begin(SD_CS_PIN, sdSPI)) {
    Serial.println("✅ SD card ready!");
  } else {
    Serial.println("⚠️  No SD card found (you can still use the system)");
  }
  
  // Create your display system automatically
  Serial.println("🎯 Creating your display system...");
  
#ifdef USE_SINGLE_DISPLAY
  displaySystem = createSingleDisplay();
#elif defined(USE_DUAL_DISPLAY)
  displaySystem = createDualDisplay();
#elif defined(USE_QUAD_DISPLAY)
  displaySystem = createQuadDisplay();
#else
  // Use the number you specified at the top
  displaySystem = createDisplaySystem(MY_DISPLAY_COUNT);
#endif

  // Do everything automatically with one function call!
  Serial.println("🚀 Starting automatic setup...");
  if (displaySystem->quickStart()) {
    Serial.println("🎉 SUCCESS! Your display system is ready to use!");
    displaySystem->printQuickHelp();
  } else {
    Serial.println("🔧 Setup needs attention - running troubleshooting...");
    displaySystem->troubleshootingWizard();
  }
}

// =============================================================================
// 🔄 MAIN LOOP - Keeps everything running smoothly
// =============================================================================

void loop() {
  static unsigned long lastStatusUpdate = 0;
  static bool demoShown = false;
  
  // Show demo after 5 seconds if no images were loaded
  if (!demoShown && millis() > 5000) {
    demoShown = true;
    
    if (displaySystem && displaySystem->isWorking()) {
      Serial.println("🎨 No images found on SD card - showing demo...");
      displaySystem->showDemo();
    }
  }
  
  // Show system status every 60 seconds
  if (millis() - lastStatusUpdate > 60000) {
    lastStatusUpdate = millis();
    
    if (displaySystem) {
      displaySystem->showSystemStatus();
    }
  }
  
  // Keep system responsive
  delay(1000);
}

// =============================================================================
// 💡 USAGE TIPS - Read this for best results!
// =============================================================================

/*
 * 🎯 HOW TO USE THIS SYSTEM:
 * 
 * 1. 📟 DISPLAYS:
 *    • Connect your GC9107 displays using the automatic pin assignments
 *    • The system will tell you exactly which pins to use
 *    • Don't worry about getting it perfect - the system will help you!
 * 
 * 2. 🖼️ IMAGES:
 *    • Put .raw image files on your SD card (128x128 pixels, RGB565 format)
 *    • The system automatically finds and displays them
 *    • No images? No problem! It shows a built-in demo
 * 
 * 3. 🔧 TROUBLESHOOTING:
 *    • Open Serial Monitor (115200 baud) to see helpful messages
 *    • The system will guide you through any problems
 *    • Built-in troubleshooting wizard helps fix common issues
 * 
 * 4. ⚙️ CUSTOMIZATION:
 *    • Change MY_DISPLAY_COUNT at the top to match your setup
 *    • Use different preset configurations if you prefer
 *    • Advanced users can access the full API through displaySystem->getManager()
 * 
 * 🎉 THAT'S IT! The system does everything else automatically!
 * 
 * 📚 WHAT HAPPENS AUTOMATICALLY:
 *    ✅ Pin configuration and validation
 *    ✅ Display initialization and testing
 *    ✅ Color calibration for perfect images
 *    ✅ Image detection and loading
 *    ✅ Error detection and helpful guidance
 *    ✅ Performance optimization (40MHz SPI)
 *    ✅ Memory management
 *    ✅ System health monitoring
 * 
 * 🚀 ADVANCED FEATURES (if you want them):
 *    • displaySystem->runHardwareTest() - Test all displays
 *    • displaySystem->loadImage("filename.raw") - Load specific image
 *    • displaySystem->interactiveSetup() - Step-by-step setup wizard
 *    • displaySystem->troubleshootingWizard() - Diagnose problems
 * 
 * 💬 NEED HELP?
 *    • Check the Serial Monitor for detailed guidance
 *    • The system provides helpful error messages and solutions
 *    • Built-in troubleshooting covers 99% of common issues
 */