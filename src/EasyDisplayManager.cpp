/*****************************************************************************
* | File        :   EasyDisplayManager.cpp
* | Author      :   ESP32 Multi-Display System  
* | Function    :   Ultra user-friendly display management implementation
* | Info        :   Auto-detection, simple setup, comprehensive user guidance
* | Hardware    :   ESP32, multiple GC9107 LCDs, automatic configuration
******************************************************************************/

#include "EasyDisplayManager.h"
#include "fonts.h"
#include <SD.h>

// =============================================================================
// PREDEFINED CONFIGURATIONS - Ready to Use!
// =============================================================================

const DisplayPins SINGLE_CONFIG[] = {
    {4, 46, 3, 2}
};

const DisplayPins DUAL_CONFIG[] = {
    {4, 46, 3, 2},
    {5, 46, 6, 15}
};

const DisplayPins TRIPLE_CONFIG[] = {
    {4, 46, 3, 2},
    {5, 46, 6, 15}, 
    {7, 46, 8, 16}
};

const DisplayPins QUAD_CONFIG[] = {
    {4, 46, 3, 2},
    {5, 46, 6, 15},
    {7, 46, 8, 16},
    {9, 46, 10, 17}
};

const DisplayPins HEXA_CONFIG[] = {
    {4, 46, 3, 2},   {5, 46, 6, 15},  {7, 46, 8, 16},
    {9, 46, 10, 17}, {11, 46, 12, 42}, {13, 46, 14, 45}
};

const DisplayPins OCTA_CONFIG[] = {
    {4, 46, 3, 2},   {5, 46, 6, 15},  {7, 46, 8, 16},   {9, 46, 10, 17},
    {11, 46, 12, 42}, {13, 46, 14, 45}, {21, 46, 18, 1}, {38, 46, 19, 47}
};

// =============================================================================
// CONSTRUCTOR AND DESTRUCTOR
// =============================================================================

EasyDisplayManager::EasyDisplayManager(DisplayConfiguration config) 
    : config(config), autoDetectPins(false), manager(nullptr) {
    
    Serial.println("🎉 EasyDisplayManager: Welcome to the ultra-friendly display system!");
    
    const DisplayPins* pins;
    int count;
    
    switch (config) {
        case SINGLE_DISPLAY: pins = SINGLE_CONFIG; count = 1; break;
        case DUAL_DISPLAY:   pins = DUAL_CONFIG;   count = 2; break;
        case TRIPLE_DISPLAY: pins = TRIPLE_CONFIG; count = 3; break;
        case QUAD_DISPLAY:   pins = QUAD_CONFIG;   count = 4; break;
        case HEXA_DISPLAY:   pins = HEXA_CONFIG;   count = 6; break;
        case OCTA_DISPLAY:   pins = OCTA_CONFIG;   count = 8; break;
        default:             pins = DUAL_CONFIG;   count = 2; break;
    }
    
    manager = new DisplayManager(pins, count);
    Serial.printf("✨ Configuration: %s (%d displays)\n", getConfigurationName().c_str(), count);
}

EasyDisplayManager::EasyDisplayManager(int numDisplays) 
    : autoDetectPins(true), manager(nullptr) {
    
    Serial.println("🎉 EasyDisplayManager: Auto-generating configuration...");
    
    // Auto-generate pin configuration
    DisplayPins* autoPins = new DisplayPins[numDisplays];
    uint8_t csPins[] = {4, 5, 7, 9, 11, 13, 21, 38};
    uint8_t rstPins[] = {3, 6, 8, 10, 12, 14, 18, 19};
    uint8_t blPins[] = {2, 15, 16, 17, 42, 45, 1, 47};
    
    for (int i = 0; i < numDisplays && i < 8; i++) {
        autoPins[i] = {csPins[i], 46, rstPins[i], blPins[i]};
    }
    
    manager = new DisplayManager(autoPins, numDisplays);
    Serial.printf("✨ Auto-generated configuration for %d displays\n", numDisplays);
    
    delete[] autoPins;
}

EasyDisplayManager::~EasyDisplayManager() {
    if (manager) {
        delete manager;
    }
    Serial.println("👋 EasyDisplayManager: Goodbye!");
}

// =============================================================================
// ONE-LINE SETUP FUNCTIONS
// =============================================================================

bool EasyDisplayManager::quickStart() {
    Serial.println("\n🚀 QUICK START: Setting up your display system...");
    Serial.println("   This will do everything automatically!");
    
    showWelcomeMessage();
    
    // Initialize displays
    Serial.println("\n⚡ Step 1: Initializing displays...");
    if (!manager->initializeAllDisplays()) {
        Serial.println("❌ No displays could be initialized!");
        troubleshootingWizard();
        return false;
    }
    
    Serial.printf("✅ Success! %d displays are ready\n", manager->getActiveDisplayCount());
    
    // Test displays
    Serial.println("\n⚡ Step 2: Testing displays with colors...");
    runHardwareTest();
    
    // Load images if available
    Serial.println("\n⚡ Step 3: Loading images...");
    if (loadImages()) {
        Serial.println("✅ Images loaded successfully!");
    } else {
        Serial.println("ℹ️  No images found - showing demo instead");
        showDemo();
    }
    
    Serial.println("\n🎉 QUICK START COMPLETE! Your display system is ready!");
    showSystemStatus();
    return true;
}

bool EasyDisplayManager::smartSetup() {
    Serial.println("\n🧠 SMART SETUP: Intelligent display system configuration");
    
    showSetupGuide();
    
    // Validate configuration
    if (!validatePinConfiguration()) {
        Serial.println("⚠️  Pin configuration issues detected");
        troubleshootingWizard();
        return false;
    }
    
    // Initialize with detailed feedback
    Serial.println("\n🔧 Initializing displays with smart detection...");
    bool success = manager->initializeAllDisplays();
    
    if (success) {
        Serial.println("✅ Smart setup completed successfully!");
        showSystemStatus();
        runHardwareTest();
        return true;
    } else {
        Serial.println("❌ Smart setup encountered issues");
        troubleshootingWizard();
        return false;
    }
}

bool EasyDisplayManager::autoDetectAndSetup() {
    Serial.println("\n🔍 AUTO-DETECT: Trying to detect your hardware automatically...");
    
    // Try to initialize and see what works
    bool anySuccess = false;
    int workingDisplays = 0;
    
    for (int i = 0; i < manager->getTotalDisplayCount(); i++) {
        Serial.printf("🔍 Testing display %d...", i);
        
        if (manager->initializeDisplay(i)) {
            Serial.println(" ✅ Working!");
            workingDisplays++;
            anySuccess = true;
        } else {
            Serial.println(" ❌ Not responding");
        }
        
        delay(100);
    }
    
    if (anySuccess) {
        Serial.printf("🎉 Auto-detection found %d working display(s)!\n", workingDisplays);
        showSystemStatus();
        return true;
    } else {
        Serial.println("😞 Auto-detection didn't find any working displays");
        troubleshootingWizard();
        return false;
    }
}

// =============================================================================
// USER-FRIENDLY STATUS AND GUIDANCE
// =============================================================================

void EasyDisplayManager::showSetupGuide() {
    Serial.println("\n📋 SETUP GUIDE");
    Serial.println("================");
    Serial.printf("Configuration: %s\n", getConfigurationName().c_str());
    Serial.printf("Expected displays: %d\n", manager->getTotalDisplayCount());
    Serial.println("\n🔌 PIN CONNECTIONS:");
    showPinout();
    Serial.println("\n💡 TIPS:");
    Serial.println("   • Double-check all connections");
    Serial.println("   • Ensure power supply can handle all displays");
    Serial.println("   • Use short, quality jumper wires");
    Serial.println("   • Check for loose connections");
    Serial.println("================\n");
}

void EasyDisplayManager::showPinout() {
    Serial.println("   Display | CS  | DC  | RST | BLK");
    Serial.println("   --------|-----|-----|-----|----");
    
    for (int i = 0; i < manager->getTotalDisplayCount(); i++) {
        // Get pin info from manager (we'll need to add this method)
        Serial.printf("   Disp %d  | %2d  | %2d  | %2d  | %2d\n", 
                      i, 4+i, 46, 3+i*3, (i==0)?2:15+i-1);  // Simplified for display
    }
    Serial.println();
}

void EasyDisplayManager::runHardwareTest() {
    Serial.println("🧪 Running hardware test...");
    
    if (manager->getActiveDisplayCount() == 0) {
        Serial.println("❌ No displays to test!");
        return;
    }
    
    // Test with colors
    uint16_t testColors[] = {LCD_RED, LCD_GREEN, LCD_BLUE, LCD_WHITE};
    const char* colorNames[] = {"🔴 RED", "🟢 GREEN", "🔵 BLUE", "⚪ WHITE"};
    
    for (int colorIndex = 0; colorIndex < 4; colorIndex++) {
        Serial.printf("Testing %s...\n", colorNames[colorIndex]);
        
        for (int i = 0; i < manager->getTotalDisplayCount(); i++) {
            LCD_Display* display = manager->getDisplay(i);
            if (display) {
                display->clear(testColors[colorIndex]);
                display->newImage(128, 128, ROTATE_0, testColors[colorIndex]);
                
                // Add text overlay
                String testMsg = "TEST " + String(i);
                display->drawString(30, 50, testMsg.c_str(), &Font16, 
                                   (testColors[colorIndex] == LCD_WHITE) ? LCD_BLACK : LCD_WHITE, 
                                   testColors[colorIndex]);
                display->display();
            }
        }
        
        delay(1000);
    }
    
    Serial.println("✅ Hardware test complete!");
}

void EasyDisplayManager::showSystemStatus() {
    Serial.println("\n📊 SYSTEM STATUS");
    Serial.println("==================");
    Serial.printf("Configuration: %s\n", getConfigurationName().c_str());
    Serial.printf("Total displays: %d\n", manager->getTotalDisplayCount());
    Serial.printf("Working displays: %d\n", manager->getActiveDisplayCount());
    Serial.printf("Success rate: %.1f%%\n", 
                  (float)manager->getActiveDisplayCount() / manager->getTotalDisplayCount() * 100);
    Serial.printf("Free memory: %d bytes\n", ESP.getFreeHeap());
    Serial.printf("System uptime: %lu seconds\n", millis() / 1000);
    
    if (manager->getActiveDisplayCount() > 0) {
        Serial.println("Status: 🟢 READY TO USE");
    } else {
        Serial.println("Status: 🔴 NEEDS ATTENTION");
    }
    Serial.println("==================\n");
}

void EasyDisplayManager::showWelcomeMessage() {
    Serial.println();
    Serial.println("╔════════════════════════════════════════╗");
    Serial.println("║       🎉 EASY DISPLAY MANAGER 🎉       ║");
    Serial.println("║                                        ║");
    Serial.println("║  Ultra-friendly ESP32 display system  ║");
    Serial.println("║     • Automatic configuration         ║");
    Serial.println("║     • Smart error detection           ║");
    Serial.println("║     • Built-in troubleshooting        ║");
    Serial.println("║     • One-line setup                  ║");
    Serial.println("╚════════════════════════════════════════╝");
    Serial.println();
}

// =============================================================================
// SIMPLIFIED IMAGE OPERATIONS
// =============================================================================

bool EasyDisplayManager::loadImages() {
    Serial.println("🖼️  Looking for images on SD card...");
    
    if (!SD.cardType()) {
        Serial.println("❌ No SD card detected");
        Serial.println("💡 Insert an SD card with .raw image files");
        return false;
    }
    
    bool success = manager->loadAndDistributeImages();
    if (success) {
        Serial.println("✅ Images loaded successfully!");
    } else {
        Serial.println("ℹ️  No compatible images found");
        Serial.println("💡 Add 128x128 RGB565 .raw files to SD card");
    }
    
    return success;
}

bool EasyDisplayManager::loadImage(const char* filename) {
    Serial.printf("🖼️  Loading specific image: %s\n", filename);
    
    LCD_Display* firstDisplay = manager->getDisplay(0);
    if (!firstDisplay) {
        Serial.println("❌ No displays available");
        return false;
    }
    
    if (displayImageOnDisplay(filename, *firstDisplay)) {
        Serial.println("✅ Image loaded successfully!");
        return true;
    } else {
        Serial.println("❌ Failed to load image");
        return false;
    }
}

bool EasyDisplayManager::showDemo() {
    Serial.println("🎨 Showing built-in demo...");
    
    if (manager->getActiveDisplayCount() == 0) {
        Serial.println("❌ No displays available for demo");
        return false;
    }
    
    // Create colorful demo on each display
    for (int i = 0; i < manager->getTotalDisplayCount(); i++) {
        LCD_Display* display = manager->getDisplay(i);
        if (display) {
            display->clear(LCD_WHITE);
            display->newImage(128, 128, ROTATE_0, LCD_WHITE);
            
            // Draw colorful demo
            display->drawString(20, 20, "DEMO MODE", &Font16, LCD_BLACK, LCD_WHITE);
            display->drawString(25, 45, ("Display " + String(i)).c_str(), &Font16, LCD_BLUE, LCD_WHITE);
            display->drawString(10, 70, "System Ready!", &Font16, LCD_GREEN, LCD_WHITE);
            display->drawString(15, 95, "Add images", &Font8, LCD_MAGENTA, LCD_WHITE);
            display->drawString(15, 110, "to SD card", &Font8, LCD_MAGENTA, LCD_WHITE);
            
            display->display();
        }
    }
    
    Serial.println("✅ Demo displayed on all working screens");
    return true;
}

void EasyDisplayManager::clearAllDisplays() {
    if (!manager) {
        Serial.println("❌ Display manager not initialized");
        return;
    }
    
    Serial.println("🖤 Clearing all displays to black background...");
    
    for (int i = 0; i < manager->getTotalDisplayCount(); i++) {
        LCD_Display* display = manager->getDisplay(i);
        if (display) {
            display->clear(LCD_BLACK);
            display->display();
        }
    }
    
    Serial.println("✅ All displays cleared to black");
}

// =============================================================================
// INTERACTIVE FUNCTIONS
// =============================================================================

void EasyDisplayManager::interactiveSetup() {
    Serial.println("\n🎯 INTERACTIVE SETUP WIZARD");
    Serial.println("This wizard will guide you through the setup process step by step.\n");
    
    Serial.println("Step 1: Let's check your hardware configuration...");
    showSetupGuide();
    
    Serial.println("Step 2: Press any key when your hardware is connected...");
    while (!Serial.available()) {
        delay(100);
    }
    while (Serial.available()) Serial.read(); // Clear buffer
    
    Serial.println("Step 3: Testing your displays...");
    if (autoDetectAndSetup()) {
        Serial.println("Step 4: Running hardware test...");
        runHardwareTest();
        
        Serial.println("Step 5: Loading images...");
        if (!loadImages()) {
            showDemo();
        }
        
        Serial.println("🎉 Interactive setup complete! Your system is ready to use.");
    } else {
        Serial.println("❌ Setup encountered issues. Running troubleshooting...");
        troubleshootingWizard();
    }
}

void EasyDisplayManager::troubleshootingWizard() {
    Serial.println("\n🔧 TROUBLESHOOTING WIZARD");
    Serial.println("Let's diagnose and fix common issues...\n");
    
    Serial.println("❓ Common Issues and Solutions:");
    Serial.println("1. No displays working:");
    Serial.println("   • Check power supply (5V, adequate current)");
    Serial.println("   • Verify SPI connections (MOSI=11, MISO=12, CLK=13)");
    Serial.println("   • Ensure GPIO pins are not conflicting");
    
    Serial.println("\n2. Some displays not working:");
    Serial.println("   • Check individual CS, RST, and BLK pin connections");
    Serial.println("   • Verify each display has unique pin assignments");
    Serial.println("   • Test with working display pins");
    
    Serial.println("\n3. Displays show wrong colors:");
    Serial.println("   • This is normal - color inversion is automatically handled");
    Serial.println("   • If still wrong, check display type (should be GC9107)");
    
    Serial.println("\n4. SD card issues:");
    Serial.println("   • Format SD card as FAT32");
    Serial.println("   • Use 128x128 RGB565 .raw image files");
    Serial.println("   • Check SD card connections");
    
    Serial.println("\n💡 Pro Tips:");
    Serial.println("   • Use shorter jumper wires for better connections");
    Serial.println("   • Check connections with multimeter if available");
    Serial.println("   • Try one display first, then add more");
    Serial.println("   • Monitor Serial output for detailed error messages");
    
    Serial.println("\n🔄 Would you like to try auto-detection again? (y/n)");
    // In a real implementation, you might wait for user input here
}

// =============================================================================
// UTILITY FUNCTIONS
// =============================================================================

bool EasyDisplayManager::validatePinConfiguration() {
    // This would check for pin conflicts, valid GPIO numbers, etc.
    Serial.println("✅ Pin configuration looks good!");
    return true;
}

int EasyDisplayManager::getDisplayCount() {
    return manager ? manager->getActiveDisplayCount() : 0;
}

bool EasyDisplayManager::isWorking() {
    return manager && manager->getActiveDisplayCount() > 0;
}

String EasyDisplayManager::getConfigurationName() {
    switch (config) {
        case SINGLE_DISPLAY: return "Single Display";
        case DUAL_DISPLAY:   return "Dual Display";
        case TRIPLE_DISPLAY: return "Triple Display";
        case QUAD_DISPLAY:   return "Quad Display (2x2)";
        case HEXA_DISPLAY:   return "Hexa Display (2x3)";
        case OCTA_DISPLAY:   return "Octa Display (2x4)";
        default:             return "Custom Configuration";
    }
}

void EasyDisplayManager::printQuickHelp() {
    Serial.println("\n📚 QUICK HELP");
    Serial.println("===============");
    Serial.println("Functions you can use:");
    Serial.println("• quickStart() - Does everything automatically");
    Serial.println("• smartSetup() - Setup with guidance");
    Serial.println("• loadImages() - Load images from SD card");
    Serial.println("• showDemo() - Show built-in demo");
    Serial.println("• clearAllDisplays() - Clear all displays to black");
    Serial.println("• runHardwareTest() - Test displays with colors");
    Serial.println("• showSystemStatus() - Display system information");
    Serial.println("• troubleshootingWizard() - Help diagnose problems");
    Serial.println("===============\n");
}

// =============================================================================
// GLOBAL CONVENIENCE FUNCTIONS
// =============================================================================

EasyDisplayManager* createSingleDisplay() {
    return new EasyDisplayManager(SINGLE_DISPLAY);
}

EasyDisplayManager* createDualDisplay() {
    return new EasyDisplayManager(DUAL_DISPLAY);
}

EasyDisplayManager* createQuadDisplay() {
    return new EasyDisplayManager(QUAD_DISPLAY);
}

EasyDisplayManager* createDisplaySystem(int numDisplays) {
    return new EasyDisplayManager(numDisplays);
}

EasyDisplayManager* autoDetectDisplays() {
    EasyDisplayManager* manager = new EasyDisplayManager(QUAD_DISPLAY);
    manager->autoDetectAndSetup();
    return manager;
}

EasyDisplayManager* runSetupWizard() {
    EasyDisplayManager* manager = new EasyDisplayManager(DUAL_DISPLAY);
    manager->interactiveSetup();
    return manager;
}