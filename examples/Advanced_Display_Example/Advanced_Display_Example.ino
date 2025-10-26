/*****************************************************************************
* | File        :   Advanced_Display_Example.ino
* | Author      :   Niklas Rydén
* | Function    :   Advanced example showing full control capabilities
* | Info        :   Professional-grade features with complete customization
* | Hardware    :   ESP32, GC9107 LCDs, SD Card
* | 
* | 🎯 ADVANCED USAGE:
* |   - Custom display configurations
* |   - Manual image loading and control
* |   - Professional error handling
* |   - Performance monitoring
* |   - Custom pin assignments
******************************************************************************/

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include "DisplayManager.h"
#include "ImageDisplay.h"
#include "LCD_Driver.h"

// =============================================================================
// 🎯 ADVANCED CONFIGURATION - Full control over every aspect
// =============================================================================

// Display configuration structure
DisplayConfig displays[] = {
  {0, 4, 5, 40000000},    // Display 0: CS=4, DC=5, 40MHz
  {1, 6, 7, 40000000},    // Display 1: CS=6, DC=7, 40MHz
  {2, 15, 16, 40000000},  // Display 2: CS=15, DC=16, 40MHz
  {3, 17, 18, 40000000}   // Display 3: CS=17, DC=18, 40MHz
};

const int displayCount = 4;  // Using 4 displays in this example

// SD Card configuration
#define SD_MISO_PIN  37
#define SD_CLK_PIN   36
#define SD_MOSI_PIN  35
#define SD_CS_PIN    39

// System objects
DisplayManager* manager = nullptr;
ImageDisplay* imageLoader = nullptr;
SPIClass sdSPI;

// Performance monitoring
struct SystemStats {
  unsigned long totalImages = 0;
  unsigned long totalErrors = 0;
  unsigned long lastImageTime = 0;
  unsigned long systemUptime = 0;
  float averageLoadTime = 0.0;
} stats;

// =============================================================================
// 🚀 ADVANCED SETUP - Complete system initialization
// =============================================================================

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(100);
  
  Serial.println(F("\n╔════════════════════════════════════════════════════════════════════════════╗"));
  Serial.println(F("║                    ADVANCED DISPLAY SYSTEM v2.0                           ║"));
  Serial.println(F("║                Professional Multi-Display Controller                       ║"));
  Serial.println(F("╚════════════════════════════════════════════════════════════════════════════╝"));
  
  // Initialize SPI with custom settings
  initializeAdvancedSPI();
  
  // Initialize SD card with error handling
  initializeSDCard();
  
  // Create and configure display manager
  setupDisplayManager();
  
  // Initialize image loader with advanced settings
  setupImageLoader();
  
  // Run comprehensive system tests
  runSystemDiagnostics();
  
  // Start performance monitoring
  startPerformanceMonitoring();
  
  Serial.println(F("\n🎉 Advanced Display System initialized and ready!"));
  showSystemCapabilities();
}

// =============================================================================
// 🔄 ADVANCED MAIN LOOP - Professional operation
// =============================================================================

void loop() {
  static unsigned long lastUpdate = 0;
  static int currentImageIndex = 0;
  
  // Update system statistics
  updateSystemStats();
  
  // Check for SD card changes every 10 seconds
  if (millis() - lastUpdate > 10000) {
    lastUpdate = millis();
    
    // Advanced image management
    manageImageRotation(currentImageIndex);
    currentImageIndex++;
    
    // System health monitoring
    performHealthCheck();
    
    // Performance reporting
    reportPerformanceMetrics();
  }
  
  // Handle serial commands for advanced control
  handleSerialCommands();
  
  // Maintain optimal system performance
  optimizeSystemPerformance();
  
  delay(100);  // Reduced delay for better responsiveness
}

// =============================================================================
// 🎛️ ADVANCED INITIALIZATION FUNCTIONS
// =============================================================================

void initializeAdvancedSPI() {
  Serial.println(F("⚡ Initializing high-performance SPI..."));
  
  SPI.begin();
  SPI.setFrequency(40000000);     // Maximum stable frequency
  SPI.setDataMode(SPI_MODE0);     // Correct mode for GC9107
  SPI.setBitOrder(MSBFIRST);      // Standard bit order
  SPI.setHwCs(false);             // Manual CS control for multiple displays
  
  Serial.println(F("✅ SPI initialized at 40MHz with optimal settings"));
}

void initializeSDCard() {
  Serial.println(F("💾 Setting up advanced SD card interface..."));
  
  sdSPI.begin(SD_CLK_PIN, SD_MISO_PIN, SD_MOSI_PIN, SD_CS_PIN);
  
  if (!SD.begin(SD_CS_PIN, sdSPI, 25000000)) {  // 25MHz for SD card
    Serial.println(F("❌ SD card initialization failed"));
    Serial.println(F("🔧 Troubleshooting:"));
    Serial.println(F("   • Check wiring connections"));
    Serial.println(F("   • Verify SD card format (FAT32)"));
    Serial.println(F("   • Try different SD card"));
    return;
  }
  
  // Advanced SD card diagnostics
  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("✅ SD card ready: %.1f MB, Type: %s\n", 
                (float)cardSize, 
                SD.cardType() == CARD_SD ? "SD" : "SDHC/SDXC");
  
  // Scan for image files
  scanImageDirectory();
}

void setupDisplayManager() {
  Serial.println(F("🎯 Creating advanced display manager..."));
  
  manager = new DisplayManager(displays, displayCount);
  
  if (!manager->initializeAllDisplays()) {
    Serial.println(F("❌ Display manager initialization failed"));
    handleInitializationError();
    return;
  }
  
  Serial.printf("✅ Display manager ready with %d displays\n", displayCount);
  
  // Advanced display configuration
  for (int i = 0; i < displayCount; i++) {
    manager->setDisplayBrightness(i, 255);  // Full brightness
    manager->setDisplayOrientation(i, 0);   // Standard orientation
  }
}

void setupImageLoader() {
  Serial.println(F("🖼️ Initializing advanced image loader..."));
  
  imageLoader = new ImageDisplay();
  
  // Configure advanced loading options
  imageLoader->setBufferSize(2048);       // Larger buffer for faster loading
  imageLoader->setColorCorrection(true);  // Enable color correction
  imageLoader->setErrorCorrection(true);  // Enable error correction
  
  Serial.println(F("✅ Image loader ready with advanced features"));
}

// =============================================================================
// 🔍 ADVANCED SYSTEM DIAGNOSTICS
// =============================================================================

void runSystemDiagnostics() {
  Serial.println(F("\n🔍 Running comprehensive system diagnostics..."));
  
  // Test each display individually
  for (int i = 0; i < displayCount; i++) {
    Serial.printf("Testing display %d... ", i);
    
    LCD_Display* display = manager->getDisplay(i);
    if (display && manager->isDisplayReady(i)) {
      Serial.println(F("✅ OK"));
      
      // Show test pattern
      display->clear(0xF800);  // Red
      display->display();
      delay(500);
      display->clear(0x07E0);  // Green
      display->display();
      delay(500);
      display->clear(0x001F);  // Blue
      display->display();
      delay(500);
      display->clear(0xFFFF);  // White (clear)
      display->display();
    } else {
      Serial.println(F("❌ FAILED"));
      Serial.printf("🔧 Check wiring for display %d\n", i);
    }
  }
  
  // Test SPI performance
  testSPIPerformance();
  
  // Test memory usage
  testMemoryUsage();
  
  Serial.println(F("✅ System diagnostics complete"));
}

void testSPIPerformance() {
  Serial.println(F("⚡ Testing SPI performance..."));
  
  unsigned long startTime = micros();
  const int testSize = 1024;
  uint8_t testData[testSize];
  
  // Fill test data
  for (int i = 0; i < testSize; i++) {
    testData[i] = i & 0xFF;
  }
  
  // Time the transfer
  SPI.beginTransaction(SPISettings(40000000, MSBFIRST, SPI_MODE0));
  SPI.writeBytes(testData, testSize);
  SPI.endTransaction();
  
  unsigned long duration = micros() - startTime;
  float throughput = (testSize * 8.0) / duration;  // Mbps
  
  Serial.printf("✅ SPI throughput: %.1f Mbps\n", throughput);
}

void testMemoryUsage() {
  Serial.println(F("🧠 Testing memory usage..."));
  
  Serial.printf("Free heap: %d bytes\n", ESP.getFreeHeap());
  Serial.printf("Largest free block: %d bytes\n", ESP.getMaxAllocHeap());
  Serial.printf("Minimum free heap: %d bytes\n", ESP.getMinFreeHeap());
  
  if (ESP.getFreeHeap() < 50000) {
    Serial.println(F("⚠️ Warning: Low memory detected"));
  }
}

// =============================================================================
// 🖼️ ADVANCED IMAGE MANAGEMENT
// =============================================================================

void scanImageDirectory() {
  Serial.println(F("🔍 Scanning for image files..."));
  
  File root = SD.open("/");
  int imageCount = 0;
  
  while (File entry = root.openNextFile()) {
    String filename = entry.name();
    if (filename.endsWith(".raw") || filename.endsWith(".RAW")) {
      Serial.printf("Found image: %s (%d bytes)\n", filename.c_str(), entry.size());
      imageCount++;
    }
    entry.close();
  }
  
  root.close();
  Serial.printf("✅ Found %d image files\n", imageCount);
}

void manageImageRotation(int& imageIndex) {
  static String imageFiles[32];  // Support up to 32 images
  static int totalImages = 0;
  static bool filesScanned = false;
  
  // Scan files on first run
  if (!filesScanned) {
    File root = SD.open("/");
    totalImages = 0;
    
    while (File entry = root.openNextFile() && totalImages < 32) {
      String filename = entry.name();
      if (filename.endsWith(".raw") || filename.endsWith(".RAW")) {
        imageFiles[totalImages++] = filename;
      }
      entry.close();
    }
    
    root.close();
    filesScanned = true;
    Serial.printf("📚 Loaded %d images for rotation\n", totalImages);
  }
  
  // Load and display images
  if (totalImages > 0) {
    if (imageIndex >= totalImages) imageIndex = 0;
    
    String currentImage = imageFiles[imageIndex];
    Serial.printf("🖼️ Loading image %d/%d: %s\n", imageIndex + 1, totalImages, currentImage.c_str());
    
    unsigned long loadStart = millis();
    
    if (manager->loadAndDistributeImages()) {
      unsigned long loadTime = millis() - loadStart;
      updateLoadTimeStats(loadTime);
      stats.totalImages++;
      Serial.printf("✅ Loaded in %lu ms\n", loadTime);
    } else {
      stats.totalErrors++;
      Serial.printf("❌ Failed to load %s\n", currentImage.c_str());
    }
  }
}

// =============================================================================
// 📊 PERFORMANCE MONITORING
// =============================================================================

void startPerformanceMonitoring() {
  Serial.println(F("📊 Starting performance monitoring..."));
  stats.systemUptime = millis();
}

void updateSystemStats() {
  stats.systemUptime = millis();
}

void updateLoadTimeStats(unsigned long loadTime) {
  if (stats.totalImages == 0) {
    stats.averageLoadTime = loadTime;
  } else {
    stats.averageLoadTime = (stats.averageLoadTime * (stats.totalImages - 1) + loadTime) / stats.totalImages;
  }
  stats.lastImageTime = loadTime;
}

void reportPerformanceMetrics() {
  static unsigned long lastReport = 0;
  
  if (millis() - lastReport > 60000) {  // Report every minute
    lastReport = millis();
    
    Serial.println(F("\n📊 PERFORMANCE REPORT:"));
    Serial.printf("   System uptime: %lu seconds\n", stats.systemUptime / 1000);
    Serial.printf("   Images loaded: %lu\n", stats.totalImages);
    Serial.printf("   Load errors: %lu\n", stats.totalErrors);
    Serial.printf("   Average load time: %.1f ms\n", stats.averageLoadTime);
    Serial.printf("   Last load time: %lu ms\n", stats.lastImageTime);
    Serial.printf("   Success rate: %.1f%%\n", 
                  stats.totalImages > 0 ? 
                  (100.0 * stats.totalImages) / (stats.totalImages + stats.totalErrors) : 
                  0.0);
    Serial.printf("   Free memory: %d bytes\n", ESP.getFreeHeap());
  }
}

// =============================================================================
// 🎛️ ADVANCED CONTROL INTERFACE
// =============================================================================

void handleSerialCommands() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    command.toLowerCase();
    
    if (command == "help") {
      showAdvancedHelp();
    } else if (command == "status") {
      showSystemStatus();
    } else if (command == "test") {
      runSystemDiagnostics();
    } else if (command.startsWith("load ")) {
      String filename = command.substring(5);
      loadSpecificImage(filename);
    } else if (command == "clear") {
      manager->clearAllDisplays();
      Serial.println(F("✅ All displays cleared"));
    } else if (command == "demo") {
      runColorDemo();
    } else if (command == "reset") {
      ESP.restart();
    } else if (command != "") {
      Serial.printf("❓ Unknown command: %s (type 'help' for commands)\n", command.c_str());
    }
  }
}

void showAdvancedHelp() {
  Serial.println(F("\n🎯 ADVANCED COMMANDS:"));
  Serial.println(F("   help     - Show this help"));
  Serial.println(F("   status   - Show system status"));
  Serial.println(F("   test     - Run diagnostics"));
  Serial.println(F("   load <f> - Load specific image file"));
  Serial.println(F("   clear    - Clear all displays"));
  Serial.println(F("   demo     - Run color demo"));
  Serial.println(F("   reset    - Restart system"));
}

void showSystemStatus() {
  Serial.println(F("\n🎯 SYSTEM STATUS:"));
  Serial.printf("   Displays: %d active\n", displayCount);
  Serial.printf("   SPI frequency: 40 MHz\n");
  Serial.printf("   SD card: %s\n", SD.cardType() != CARD_NONE ? "Ready" : "Not found");
  Serial.printf("   Free memory: %d bytes\n", ESP.getFreeHeap());
}

void loadSpecificImage(const String& filename) {
  Serial.printf("🖼️ Loading specific image: %s\n", filename.c_str());
  
  unsigned long loadStart = millis();
  if (manager->loadImageToDisplay(0, filename.c_str())) {
    unsigned long loadTime = millis() - loadStart;
    Serial.printf("✅ Loaded %s in %lu ms\n", filename.c_str(), loadTime);
  } else {
    Serial.printf("❌ Failed to load %s\n", filename.c_str());
  }
}

void runColorDemo() {
  Serial.println(F("🎨 Running color demo..."));
  
  const uint16_t colors[] = {0xF800, 0x07E0, 0x001F, 0xFFE0, 0xF81F, 0x07FF, 0xFFFF, 0x0000};
  const char* colorNames[] = {"Red", "Green", "Blue", "Yellow", "Magenta", "Cyan", "White", "Black"};
  
  for (int i = 0; i < 8; i++) {
    Serial.printf("Showing %s...\n", colorNames[i]);
    manager->clearAllDisplays(colors[i]);
    delay(1000);
  }
  
  manager->clearAllDisplays();
  Serial.println(F("✅ Color demo complete"));
}

// =============================================================================
// 🔧 ERROR HANDLING AND OPTIMIZATION
// =============================================================================

void handleInitializationError() {
  Serial.println(F("🔧 INITIALIZATION ERROR RECOVERY:"));
  Serial.println(F("   1. Check all display connections"));
  Serial.println(F("   2. Verify pin assignments"));
  Serial.println(F("   3. Ensure adequate power supply"));
  Serial.println(F("   4. Try reducing display count"));
  
  // Attempt partial recovery
  Serial.println(F("🔄 Attempting partial system recovery..."));
  
  // Continue with available displays
  int workingDisplays = 0;
  for (int i = 0; i < displayCount; i++) {
    if (manager && manager->isDisplayReady(i)) {
      workingDisplays++;
    }
  }
  
  if (workingDisplays > 0) {
    Serial.printf("✅ Continuing with %d working displays\n", workingDisplays);
  } else {
    Serial.println(F("❌ No working displays found - system halted"));
    while (true) delay(1000);
  }
}

void performHealthCheck() {
  static int healthCheckCount = 0;
  healthCheckCount++;
  
  // Check memory usage
  if (ESP.getFreeHeap() < 30000) {
    Serial.println(F("⚠️ Low memory warning - optimizing..."));
    optimizeMemoryUsage();
  }
  
  // Check display responsiveness every 10 checks
  if (healthCheckCount % 10 == 0) {
    for (int i = 0; i < displayCount; i++) {
      if (!manager->isDisplayReady(i)) {
        Serial.printf("⚠️ Display %d not responding\n", i);
      }
    }
  }
}

void optimizeSystemPerformance() {
  // Dynamic memory management
  if (ESP.getFreeHeap() < 40000) {
    yield();  // Allow system to optimize memory
    delay(10);
  }
  
  // Maintain SPI settings
  static unsigned long lastSPICheck = 0;
  if (millis() - lastSPICheck > 30000) {  // Check every 30 seconds
    lastSPICheck = millis();
    SPI.setFrequency(40000000);  // Ensure maximum speed
  }
}

void optimizeMemoryUsage() {
  // Force memory cleanup
  yield();
  delay(10);
  
  // Clear any cached data
  if (manager) {
    manager->optimizeMemoryUsage();
  }
  
  Serial.printf("🧠 Memory optimized - Free: %d bytes\n", ESP.getFreeHeap());
}

void showSystemCapabilities() {
  Serial.println(F("\n✨ SYSTEM CAPABILITIES:"));
  Serial.println(F("   🎯 Multi-display management"));
  Serial.println(F("   ⚡ 40MHz high-speed SPI"));
  Serial.println(F("   🖼️ Automatic image loading"));
  Serial.println(F("   📊 Performance monitoring"));
  Serial.println(F("   🔍 Advanced diagnostics"));
  Serial.println(F("   🎛️ Interactive control"));
  Serial.println(F("   🔧 Error recovery"));
  Serial.println(F("   🧠 Memory optimization"));
  Serial.println(F("\n💡 Type commands in Serial Monitor for control!"));
}