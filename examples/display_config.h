/**
 * Example Configuration for ESP32 LCD Display System
 * Copy and modify these settings in your main Arduino file
 */

#ifndef DISPLAY_CONFIG_H
#define DISPLAY_CONFIG_H

// =============================================================================
// DISPLAY CONFIGURATION
// =============================================================================

// Number of displays (1-8 supported)
#define DISPLAY_COUNT 2

// Display arrangement (for multi-display setups)
// Options: SINGLE, DUAL_HORIZONTAL, DUAL_VERTICAL, QUAD_2x2, CUSTOM
#define DISPLAY_ARRANGEMENT DUAL_HORIZONTAL

// =============================================================================
// SPI CONFIGURATION  
// =============================================================================

// SPI frequency (Hz) - 40MHz recommended for best performance
#define SPI_FREQUENCY 40000000

// SPI mode for GC9107 displays
#define SPI_MODE SPI_MODE0

// =============================================================================
// SD CARD CONFIGURATION
// =============================================================================

// SD card SPI pins (adjust for your hardware)
#define SD_MISO_PIN  37
#define SD_CLK_PIN   36
#define SD_MOSI_PIN  35
#define SD_CS_PIN    39

// SD card SPI frequency (Hz) - can be lower than display SPI
#define SD_SPI_FREQUENCY 25000000

// =============================================================================
// MEMORY CONFIGURATION
// =============================================================================

// Memory optimization interval (milliseconds)
#define MEMORY_OPTIMIZATION_INTERVAL 30000

// Low memory threshold for optimization trigger (bytes)
#define LOW_MEMORY_THRESHOLD 50000

// Critical memory threshold for aggressive cleanup (bytes)  
#define CRITICAL_MEMORY_THRESHOLD 30000

// =============================================================================
// IMAGE CONFIGURATION
// =============================================================================

// Image refresh interval (milliseconds) - 5 minutes default
#define IMAGE_REFRESH_INTERVAL 300000

// Maximum image file size (bytes) - should be 32768 for 128x128 RGB565
#define MAX_IMAGE_FILE_SIZE 32768

// Transparency key color (RGB565) - bright green
#define TRANSPARENCY_KEY_COLOR 0x07E0

// =============================================================================
// SERIAL CONFIGURATION
// =============================================================================

// Serial baud rate for debug output
#define SERIAL_BAUD_RATE 115200

// Enable verbose debug output (comment out to disable)
#define ENABLE_DEBUG_OUTPUT

// Status update interval (milliseconds)
#define STATUS_UPDATE_INTERVAL 120000

// =============================================================================
// TIMING CONFIGURATION
// =============================================================================

// Startup delay before loading images (milliseconds)
#define STARTUP_DELAY 3000

// Main loop delay (milliseconds)
#define MAIN_LOOP_DELAY 1000

// Display update timeout (milliseconds)
#define DISPLAY_TIMEOUT 5000

#endif // DISPLAY_CONFIG_H