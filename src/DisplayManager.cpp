/*****************************************************************************
* | File        :   DisplayManager.cpp
* | Author      :   ESP32 Multi-Display System
* | Function    :   Implementation of scalable N-display management system
* | Info        :   Modular display management with automatic setup and control
* | Hardware    :   ESP32, multiple GC9107 LCDs, configurable pin assignments
******************************************************************************/

#include "DisplayManager.h"
#include "fonts.h"
#include <SD.h>

// =============================================================================
// CONSTRUCTOR AND DESTRUCTOR
// =============================================================================

DisplayManager::DisplayManager(const DisplayPins* configs, int count) 
    : numDisplays(count), activeDisplayCount(0) {
    
    // Allocate arrays
    displays = new LCD_Display*[numDisplays];
    displayReady = new bool[numDisplays];
    pinConfigs = new DisplayPins[numDisplays];
    
    // Copy pin configurations
    for (int i = 0; i < numDisplays; i++) {
        pinConfigs[i] = configs[i];
        displays[i] = nullptr;
        displayReady[i] = false;
    }
    
    Serial.printf("DisplayManager: Configured for %d displays\n", numDisplays);
}

DisplayManager::~DisplayManager() {
    // Clean up display objects
    for (int i = 0; i < numDisplays; i++) {
        if (displays[i]) {
            delete displays[i];
        }
    }
    
    // Clean up arrays
    delete[] displays;
    delete[] displayReady;
    delete[] pinConfigs;
    
    Serial.println("DisplayManager: Cleanup complete");
}

// =============================================================================
// INITIALIZATION AND SETUP
// =============================================================================

bool DisplayManager::initializeAllDisplays() {
    activeDisplayCount = 0;
    
    Serial.printf("=== DisplayManager: Initializing %d Displays ===\n", numDisplays);
    
    for (int i = 0; i < numDisplays; i++) {
        if (initializeDisplay(i)) {
            activeDisplayCount++;
        }
        
        // Small delay between initializations
        delay(50);
    }
    
    Serial.printf("=== DisplayManager: %d/%d Displays Active ===\n", 
                  activeDisplayCount, numDisplays);
    
    showStartupMessages();
    return activeDisplayCount > 0;
}

bool DisplayManager::initializeDisplay(int index) {
    if (index < 0 || index >= numDisplays) {
        return false;
    }
    
    Serial.printf("Display %d: Initializing (CS:%d, DC:%d, RST:%d, BL:%d)...", 
                  index, pinConfigs[index].cs_pin, pinConfigs[index].dc_pin,
                  pinConfigs[index].rst_pin, pinConfigs[index].bl_pin);
    
    // Create display object
    displays[index] = new LCD_Display(
        pinConfigs[index].cs_pin,
        pinConfigs[index].dc_pin,
        pinConfigs[index].rst_pin,
        pinConfigs[index].bl_pin
    );
    
    // Initialize hardware
    if (displays[index]->begin()) {
        displays[index]->setBacklight(255);
        displays[index]->clear(LCD_WHITE);
        displays[index]->newImage(128, 128, ROTATE_0, LCD_WHITE);
        
        if (displays[index]->isInitialized()) {
            // Show display identification
            String displayId = "DISP " + String(index);
            displays[index]->drawString(25, 50, displayId.c_str(), &Font16, LCD_GREEN, LCD_WHITE);
            displays[index]->drawString(30, 70, "READY", &Font16, LCD_BLUE, LCD_WHITE);
            displays[index]->display();
            
            displayReady[index] = true;
            Serial.println(" ✓ Ready");
            return true;
        } else {
            Serial.println(" ❌ Buffer Failed");
        }
    } else {
        Serial.println(" ❌ Hardware Failed");
    }
    
    // Cleanup on failure
    displayReady[index] = false;
    if (displays[index]) {
        delete displays[index];
        displays[index] = nullptr;
    }
    
    return false;
}

void DisplayManager::showStartupMessages() {
    for (int i = 0; i < numDisplays; i++) {
        if (displayReady[i]) {
            displays[i]->clear(LCD_WHITE);
            displays[i]->newImage(128, 128, ROTATE_0, LCD_WHITE);
            
            displays[i]->drawString(20, 30, "SYSTEM", &Font16, LCD_BLACK, LCD_WHITE);
            displays[i]->drawString(25, 50, "READY", &Font16, LCD_GREEN, LCD_WHITE);
            displays[i]->drawString(5, 80, ("Active: " + String(activeDisplayCount)).c_str(), 
                                   &Font8, LCD_BLUE, LCD_WHITE);
            displays[i]->drawString(5, 95, ("Display: " + String(i)).c_str(), 
                                   &Font8, LCD_MAGENTA, LCD_WHITE);
            displays[i]->display();
        }
    }
}

// =============================================================================
// DISPLAY ACCESS AND CONTROL
// =============================================================================

LCD_Display* DisplayManager::getDisplay(int index) {
    if (index >= 0 && index < numDisplays && displayReady[index]) {
        return displays[index];
    }
    return nullptr;
}

bool DisplayManager::isDisplayReady(int index) const {
    return (index >= 0 && index < numDisplays && displayReady[index]);
}

// =============================================================================
// IMAGE MANAGEMENT
// =============================================================================

bool DisplayManager::loadAndDistributeImages() {
    Serial.printf("\n--- Loading Images for %d Active Displays ---\n", activeDisplayCount);
    
    // Validate SD card
    if (!SD.cardType()) {
        Serial.println("❌ No SD card detected");
        showErrorOnAllDisplays("NO SD CARD");
        return false;
    }
    
    // Find available images
    String* imageFiles = new String[activeDisplayCount];
    int fileCount = findImageFiles(imageFiles, activeDisplayCount);
    
    if (fileCount == 0) {
        Serial.println("❌ No compatible image files found");
        showErrorOnAllDisplays("NO IMAGES");
        delete[] imageFiles;
        return false;
    }
    
    Serial.printf("📂 Found %d image file(s) for %d display(s)\n", fileCount, activeDisplayCount);
    
    // Distribute images to displays
    int imageIndex = 0;
    bool anySuccess = false;
    
    for (int i = 0; i < numDisplays && imageIndex < fileCount; i++) {
        if (displayReady[i]) {
            if (loadImageToDisplay(i, imageFiles[imageIndex].c_str())) {
                anySuccess = true;
            }
            imageIndex++;
            yield();
        }
    }
    
    // Handle remaining displays
    if (fileCount < activeDisplayCount) {
        showStatusOnRemainingDisplays(fileCount);
    }
    
    Serial.printf("✅ Image loading complete - %d image(s) processed\n", 
                  min(fileCount, activeDisplayCount));
    
    delete[] imageFiles;
    return anySuccess;
}

bool DisplayManager::loadImageToDisplay(int displayIndex, const char* filename) {
    if (!isDisplayReady(displayIndex)) {
        return false;
    }
    
    Serial.printf("⚡ Display %d loading: %s\n", displayIndex, filename);
    
    if (displayImageOnDisplay(filename, *displays[displayIndex])) {
        Serial.printf("✅ Display %d loaded successfully\n", displayIndex);
        return true;
    } else {
        Serial.printf("❌ Failed to load image on Display %d\n", displayIndex);
        displayErrorMessage("LOAD ERROR", *displays[displayIndex]);
        return false;
    }
}

void DisplayManager::showErrorOnAllDisplays(const char* errorMsg) {
    for (int i = 0; i < numDisplays; i++) {
        if (displayReady[i]) {
            displayErrorMessage(errorMsg, *displays[i]);
        }
    }
}

void DisplayManager::showStatusOnRemainingDisplays(int imagesLoaded) {
    int displayIndex = 0;
    
    for (int i = 0; i < numDisplays; i++) {
        if (displayReady[i]) {
            if (displayIndex >= imagesLoaded) {
                // This display doesn't have an image, show status
                displays[i]->clear(LCD_WHITE);
                displays[i]->newImage(128, 128, ROTATE_0, LCD_WHITE);
                
                String statusMsg = String(imagesLoaded) + " IMAGE";
                if (imagesLoaded != 1) statusMsg += "S";
                
                displays[i]->drawString(20, 50, statusMsg.c_str(), &Font16, LCD_BLUE, LCD_WHITE);
                displays[i]->drawString(25, 70, "LOADED", &Font16, LCD_GREEN, LCD_WHITE);
                displays[i]->display();
                
                Serial.printf("Display %d showing status: %s\n", i, statusMsg.c_str());
            }
            displayIndex++;
        }
    }
}

// =============================================================================
// SYSTEM INFORMATION AND STATUS
// =============================================================================

void DisplayManager::showSystemInfo() {
    // Show on first active display
    for (int i = 0; i < numDisplays; i++) {
        if (displayReady[i]) {
            displays[i]->clear(LCD_WHITE);
            displays[i]->newImage(128, 128, ROTATE_0, LCD_WHITE);
            
            displays[i]->drawString(10, 20, "SYSTEM INFO", &Font16, LCD_BLACK, LCD_WHITE);
            displays[i]->drawString(10, 45, ("Displays: " + String(activeDisplayCount) + "/" + String(numDisplays)).c_str(), 
                                   &Font8, LCD_BLUE, LCD_WHITE);
            displays[i]->drawString(10, 60, ("Free RAM: " + String(ESP.getFreeHeap())).c_str(), 
                                   &Font8, LCD_GREEN, LCD_WHITE);
            displays[i]->drawString(10, 75, ("Uptime: " + String(millis()/1000) + "s").c_str(), 
                                   &Font8, LCD_MAGENTA, LCD_WHITE);
            displays[i]->display();
            break;
        }
    }
}

void DisplayManager::showDisplayStatus() {
    for (int i = 0; i < numDisplays; i++) {
        if (displayReady[i]) {
            displays[i]->clear(LCD_WHITE);
            displays[i]->newImage(128, 128, ROTATE_0, LCD_WHITE);
            
            String displayNum = "DISPLAY " + String(i);
            displays[i]->drawString(10, 40, displayNum.c_str(), &Font16, LCD_BLACK, LCD_WHITE);
            displays[i]->drawString(20, 60, "ACTIVE", &Font16, LCD_GREEN, LCD_WHITE);
            displays[i]->drawString(5, 85, ("CS:" + String(pinConfigs[i].cs_pin)).c_str(), 
                                   &Font8, LCD_BLUE, LCD_WHITE);
            displays[i]->drawString(5, 100, ("RST:" + String(pinConfigs[i].rst_pin)).c_str(), 
                                    &Font8, LCD_BLUE, LCD_WHITE);
            displays[i]->display();
        }
    }
}

void DisplayManager::printSystemSummary() {
    Serial.println("\n=== DisplayManager System Summary ===");
    Serial.printf("Total Configured: %d displays\n", numDisplays);
    Serial.printf("Active Displays: %d\n", activeDisplayCount);
    Serial.printf("Free Heap: %d bytes\n", ESP.getFreeHeap());
    
    for (int i = 0; i < numDisplays; i++) {
        const char* status = displayReady[i] ? "✓ Ready" : "❌ Failed";
        Serial.printf("Display %d: %s (CS:%d, RST:%d)\n", 
                      i, status, pinConfigs[i].cs_pin, pinConfigs[i].rst_pin);
    }
    Serial.println("========================================\n");
}

// =============================================================================
// UTILITY FUNCTIONS
// =============================================================================

void DisplayManager::clearAllDisplays(uint16_t color) {
    for (int i = 0; i < numDisplays; i++) {
        if (displayReady[i]) {
            displays[i]->clear(color);
            displays[i]->display();
        }
    }
}

void DisplayManager::setAllBacklights(uint8_t brightness) {
    for (int i = 0; i < numDisplays; i++) {
        if (displayReady[i]) {
            displays[i]->setBacklight(brightness);
        }
    }
}

void DisplayManager::testAllDisplays() {
    Serial.println("Testing all displays...");
    
    // Test colors
    uint16_t testColors[] = {LCD_RED, LCD_GREEN, LCD_BLUE, LCD_WHITE};
    const char* colorNames[] = {"RED", "GREEN", "BLUE", "WHITE"};
    
    for (int colorIndex = 0; colorIndex < 4; colorIndex++) {
        Serial.printf("Testing color: %s\n", colorNames[colorIndex]);
        
        for (int i = 0; i < numDisplays; i++) {
            if (displayReady[i]) {
                displays[i]->clear(testColors[colorIndex]);
                displays[i]->newImage(128, 128, ROTATE_0, testColors[colorIndex]);
                displays[i]->display();
            }
        }
        
        delay(1000);
    }
    
    Serial.println("Display test complete");
}

// =============================================================================
// CONVENIENCE FUNCTIONS
// =============================================================================

DisplayManager* createDisplayManager(const DisplayPins* configs, int count) {
    return new DisplayManager(configs, count);
}

DisplayManager* createDualDisplayManager() {
    static const DisplayPins dualConfig[] = {
        {4,  46, 3,  2},   // Display 0
        {5,  46, 6,  15},  // Display 1
    };
    
    return new DisplayManager(dualConfig, 2);
}

DisplayManager* createQuadDisplayManager() {
    static const DisplayPins quadConfig[] = {
        {4,  46, 3,  2},   // Display 0
        {5,  46, 6,  15},  // Display 1
        {7,  46, 8,  16},  // Display 2
        {9,  46, 10, 17},  // Display 3
    };
    
    return new DisplayManager(quadConfig, 4);
}

// =============================================================================
// MEMORY OPTIMIZATION IMPLEMENTATIONS
// =============================================================================

void DisplayManager::clearImageCache() {
    Serial.println("🧹 DisplayManager: Clearing image caches (preserving display content)...");
    
    int beforeFree = ESP.getFreeHeap();
    
    // Clear any cached data in ImageDisplay library
    ::clearImageCache(false);
    
    // DON'T clear the actual display content - just do memory cleanup
    // The displays should keep showing their current content
    
    // Force memory cleanup
    yield();
    delay(10);
    
    int afterFree = ESP.getFreeHeap();
    int recovered = afterFree - beforeFree;
    
    if (recovered > 0) {
        Serial.printf("🧹 DisplayManager cache cleared: %d bytes recovered (display preserved)\n", recovered);
    }
}

void DisplayManager::clearAllCaches() {
    Serial.println("🧹 DisplayManager: Aggressive cache clearing...");
    
    int beforeFree = ESP.getFreeHeap();
    
    // Clear image library caches with force cleanup
    ::clearImageCache(true);
    
    // Clear all display buffers
    clearImageCache();
    
    // Multiple memory cleanup passes
    for (int i = 0; i < 3; i++) {
        yield();
        delay(10);
    }
    
    int afterFree = ESP.getFreeHeap();
    int recovered = afterFree - beforeFree;
    
    Serial.printf("🧹 All caches cleared: %d bytes recovered (now %d free)\n", 
                  recovered, afterFree);
}

void DisplayManager::optimizeMemoryUsage() {
    Serial.println("💾 DisplayManager: Optimizing memory usage...");
    
    int beforeFree = ESP.getFreeHeap();
    
    // Step 1: Clear image caches
    clearImageCache();
    
    // Step 2: Optimize ImageDisplay library memory
    int imageMemoryRecovered = ::optimizeImageMemory();
    
    // Step 3: Display-specific optimizations (preserve content)
    // Don't clear displays - just do memory cleanup without affecting what's shown
    
    // Step 4: System-level optimization
    yield();
    delay(10);
    
    int afterFree = ESP.getFreeHeap();
    int totalRecovered = afterFree - beforeFree;
    
    Serial.printf("💾 Memory optimization complete:\n");
    Serial.printf("   Image memory: %d bytes recovered\n", imageMemoryRecovered);
    Serial.printf("   Total recovered: %d bytes\n", totalRecovered);
    Serial.printf("   Current free: %d bytes\n", afterFree);
}

int DisplayManager::getMemoryUsage() const {
    // Estimate memory usage of DisplayManager
    int baseUsage = sizeof(DisplayManager);
    int displayUsage = activeDisplayCount * sizeof(LCD_Display*);
    int configUsage = numDisplays * sizeof(DisplayPins);
    
    return baseUsage + displayUsage + configUsage;
}