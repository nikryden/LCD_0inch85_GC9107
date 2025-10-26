/*****************************************************************************
* | File        :   DisplayManager.h
* | Author      :   Niklas Rydén
* | Function    :   Header for scalable N-display management system
* | Info        :   Modular display management with automatic setup and control
* | Hardware    :   ESP32, multiple GC9107 LCDs, configurable pin assignments
******************************************************************************/

#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Arduino.h>
#include "LCD_Display.h"
#include "ImageDisplay.h"

// =============================================================================
// DISPLAY CONFIGURATION STRUCTURES
// =============================================================================

/**
 * Pin configuration structure for individual displays
 */
struct DisplayPins {
    uint8_t cs_pin;   // SPI Chip Select (unique per display)
    uint8_t dc_pin;   // Data/Command (can be shared)
    uint8_t rst_pin;  // Reset (unique per display)
    uint8_t bl_pin;   // Backlight (unique per display)
};

// =============================================================================
// DISPLAY MANAGER CLASS
// =============================================================================

class DisplayManager {
private:
    LCD_Display** displays;         // Array of display pointers
    bool* displayReady;            // Status tracking array
    DisplayPins* pinConfigs;       // Pin configuration array
    int numDisplays;               // Total configured displays
    int activeDisplayCount;        // Number of successfully initialized displays
    
public:
    // Constructor and Destructor
    DisplayManager(const DisplayPins* configs, int count);
    ~DisplayManager();
    
    // Initialization and Setup
    bool initializeAllDisplays();
    bool initializeDisplay(int index);
    void showStartupMessages();
    
    // Display Access and Control
    LCD_Display* getDisplay(int index);
    int getActiveDisplayCount() const { return activeDisplayCount; }
    int getTotalDisplayCount() const { return numDisplays; }
    bool isDisplayReady(int index) const;
    
    // Image Management
    bool loadAndDistributeImages();
    bool loadImageToDisplay(int displayIndex, const char* filename);
    void showErrorOnAllDisplays(const char* errorMsg);
    void showStatusOnRemainingDisplays(int imagesLoaded);
    
    // System Information and Status
    void showSystemInfo();
    void showDisplayStatus();
    void printSystemSummary();
    
    // Utility Functions
    void clearAllDisplays(uint16_t color = 0xFFFF);
    void setAllBacklights(uint8_t brightness);
    void testAllDisplays();
    
    // Memory Optimization Functions
    void clearImageCache();
    void clearAllCaches();
    void optimizeMemoryUsage();
    int getMemoryUsage() const;
};

// =============================================================================
// CONVENIENCE FUNCTIONS FOR COMMON OPERATIONS
// =============================================================================

/**
 * Create a display manager with standard configuration
 */
DisplayManager* createDisplayManager(const DisplayPins* configs, int count);

/**
 * Quick setup function for common 2-display configuration
 */
DisplayManager* createDualDisplayManager();

/**
 * Quick setup function for common 4-display configuration
 */
DisplayManager* createQuadDisplayManager();

#endif // DISPLAY_MANAGER_H