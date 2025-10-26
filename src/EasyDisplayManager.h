/*****************************************************************************
* | File        :   EasyDisplayManager.h  
* | Author      :   Niklas Rydén
* | Function    :   Ultra user-friendly display management system
* | Info        :   Auto-detection, simple setup, comprehensive user guidance
* | Hardware    :   ESP32, multiple GC9107 LCDs, automatic configuration
******************************************************************************/

#ifndef EASY_DISPLAY_MANAGER_H
#define EASY_DISPLAY_MANAGER_H

#include "DisplayManager.h"
#include <vector>

// =============================================================================
// PREDEFINED DISPLAY CONFIGURATIONS - Just Pick One!
// =============================================================================

enum DisplayConfiguration {
    SINGLE_DISPLAY,     // 1 display - perfect for testing
    DUAL_DISPLAY,       // 2 displays - most common setup  
    TRIPLE_DISPLAY,     // 3 displays - extended setup
    QUAD_DISPLAY,       // 4 displays - 2x2 grid
    HEXA_DISPLAY,       // 6 displays - 2x3 grid
    OCTA_DISPLAY        // 8 displays - 2x4 grid
};

// =============================================================================
// EASY DISPLAY MANAGER CLASS - Ultra User-Friendly
// =============================================================================

class EasyDisplayManager {
private:
    DisplayManager* manager;
    DisplayConfiguration config;
    bool autoDetectPins;
    std::vector<String> setupMessages;
    
    // Auto-detection and validation
    bool validatePinConfiguration();
    void generateSetupGuide();
    void showWelcomeMessage();
    
public:
    // Ultra-simple constructors
    EasyDisplayManager(DisplayConfiguration config = DUAL_DISPLAY);
    EasyDisplayManager(int numDisplays);  // Automatic pin assignment
    ~EasyDisplayManager();
    
    // One-line setup functions
    bool quickStart();                    // Does everything automatically
    bool smartSetup();                    // Setup with guidance
    bool autoDetectAndSetup();           // Try to detect hardware
    
    // User-friendly status functions
    void showSetupGuide();               // Print helpful setup instructions
    void showPinout();                   // Display pin assignments
    void runHardwareTest();              // Test all displays with colors
    void showSystemStatus();             // Comprehensive status display
    
    // Simplified image operations
    bool loadImages();                   // Load all available images
    bool loadImage(const char* filename); // Load specific image
    bool showDemo();                     // Show built-in demo
    void clearAllDisplays();             // Clear all displays to black
    
    // Interactive functions
    void interactiveSetup();             // Walk user through setup
    void troubleshootingWizard();        // Help diagnose problems
    
    // Access underlying manager for advanced users
    DisplayManager* getManager() { return manager; }
    
    // User-friendly info functions
    int getDisplayCount();
    bool isWorking();
    String getConfigurationName();
    void printQuickHelp();
};

// =============================================================================
// GLOBAL CONVENIENCE FUNCTIONS - One-Liner Setup
// =============================================================================

/**
 * Ultra-simple one-line setup functions
 */
EasyDisplayManager* createSingleDisplay();
EasyDisplayManager* createDualDisplay(); 
EasyDisplayManager* createQuadDisplay();
EasyDisplayManager* createDisplaySystem(int numDisplays);

/**
 * Automatic system detection and setup
 */
EasyDisplayManager* autoDetectDisplays();

/**
 * Interactive setup wizard
 */
EasyDisplayManager* runSetupWizard();

#endif // EASY_DISPLAY_MANAGER_H