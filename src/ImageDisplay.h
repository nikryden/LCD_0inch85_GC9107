/*****************************************************************************
* | File        :   ImageDisplay.h
* | Author      :   Niklas Rydén
* | Function    :   Header for reusable image display functionality
* | Info        :   Modular library for loading images from SD to LCD displays
* | Hardware    :   ESP32, GC9107 LCDs, SD Card
* | 
* | Key Features:
* |   - Support for multiple image formats (raw RGB565, header files)
* |   - Automatic color transformation for bit-inverting LCDs
* |   - Memory-optimized loading with progress tracking
* |   - Comprehensive error handling and validation
* |   - Reusable across different LCD display projects
******************************************************************************/

#ifndef IMAGE_DISPLAY_H
#define IMAGE_DISPLAY_H

#include <Arduino.h>
#include <SD.h>
#include <FS.h>
#include "LCD_Display.h"
#include "fonts.h"

// =============================================================================
// LIBRARY CONFIGURATION
// =============================================================================

// Image specifications
#define IMAGE_WIDTH       128    // LCD width in pixels
#define IMAGE_HEIGHT      128    // LCD height in pixels  
#define BYTES_PER_PIXEL   2      // RGB565 = 2 bytes per pixel
#define RAW_IMAGE_SIZE    (IMAGE_WIDTH * IMAGE_HEIGHT * BYTES_PER_PIXEL)

// Memory management and performance optimization
#define MIN_FREE_HEAP     10000  // Minimum heap required for safe operation
#define PROGRESS_INTERVAL 32     // Progress reporting every N rows (reduced frequency)
#define YIELD_INTERVAL    64     // Yield to watchdog every N pixels (reduced frequency)
#define PIXEL_BUFFER_SIZE 256    // Buffer size for batch pixel operations
#define ROW_BUFFER_SIZE   128    // Buffer one complete row for faster writing

// Color definitions - RGB565 format (compatible with bit inversion)
// These colors will be automatically transformed by transformColorForLCD()
#define LCD_WHITE   0xFFFF  // White
#define LCD_BLACK   0x0000  // Black
#define LCD_RED     0xF800  // Red
#define LCD_GREEN   0x07E0  // Green
#define LCD_BLUE    0x001F  // Blue
#define LCD_YELLOW  0xFFE0  // Yellow
#define LCD_CYAN    0x07FF  // Cyan
#define LCD_MAGENTA 0xF81F  // Magenta

// Transparency support
#define TRANSPARENT_COLOR   0x07E0  // Bright green (RGB565: 0,255,0) used as transparency key
#define ALPHA_THRESHOLD     128     // Alpha threshold for transparency (0-255)

// File format support
#define MAX_FILENAME_LENGTH  64
#define MAX_LINE_BUFFER     256

// =============================================================================
// COLOR TRANSFORMATION FUNCTION - Bit Inversion Support
// =============================================================================

/**
 * Transform colors for GC9107 LCD with bit inversion behavior
 * 
 * Some GC9107 LCD models perform complete bit inversion on incoming
 * color data. This function applies compensation to ensure standard
 * RGB565 colors display correctly.
 * 
 * @param color Standard RGB565 color value
 * @return Color value compatible with bit-inverting LCD hardware
 */
uint16_t transformColorForLCD(uint16_t color);

/**
 * Check if a color should be treated as transparent
 * 
 * @param color RGB565 color value to check
 * @return true if color should be transparent, false otherwise
 */
bool isTransparentColor(uint16_t color);

/**
 * Blend two colors with alpha blending
 * 
 * @param foreground Foreground color (RGB565)
 * @param background Background color (RGB565)  
 * @param alpha Alpha value (0-255, 0=transparent, 255=opaque)
 * @return Blended color in RGB565 format
 */
uint16_t blendColors(uint16_t foreground, uint16_t background, uint8_t alpha);

// =============================================================================
// IMAGE LOADING FUNCTIONS - Public Interface
// =============================================================================

/**
 * Display image file on specified LCD display
 * Automatically detects file format and applies appropriate loading method
 * 
 * @param filename Full path to image file on SD card
 * @param display Reference to initialized LCD display object
 * @return true if image loaded successfully, false otherwise
 */
bool displayImageOnDisplay(const char* filename, LCD_Display& display);

/**
 * Find image files on SD card
 * Searches for compatible image files in root directory
 * 
 * @param imageFiles Array to store found filenames
 * @param maxFiles Maximum number of files to find
 * @return Number of image files found
 */
int findImageFiles(String* imageFiles, int maxFiles);

/**
 * Display error message on LCD display
 * Shows formatted error message with system styling
 * 
 * @param message Error message text to display
 * @param display Reference to LCD display object
 */
void displayErrorMessage(const char* message, LCD_Display& display);

/**
 * Clear image data from memory after display
 * Frees memory used by image buffers and cached data
 * 
 * @param forceCleanup If true, performs aggressive memory cleanup
 */
void clearImageCache(bool forceCleanup = false);

/**
 * Optimize memory usage for image operations
 * Performs garbage collection and memory compaction
 * 
 * @return Number of bytes freed
 */
int optimizeImageMemory();

/**
 * Display file information on LCD display  
 * Shows file details when format is not recognized
 * 
 * @param filename Name of the file
 * @param fileSize Size of file in bytes
 * @param display Reference to LCD display object
 */
void displayInfoOnDisplay(const char* filename, size_t fileSize, LCD_Display& display);

// =============================================================================
// FORMAT-SPECIFIC LOADING FUNCTIONS - Internal Implementation
// =============================================================================

/**
 * Load raw RGB565 binary file to LCD display
 * Handles memory management and applies color transformation
 * 
 * @param file Reference to opened SD card file
 * @param display Reference to LCD display object
 * @return true if image loaded successfully, false otherwise
 */
bool loadRawRGB565ImageToDisplay(File& file, LCD_Display& display);

/**
 * Parse and load C++ header file image to LCD display
 * Safely extracts RGB565 data from PROGMEM array declarations
 * 
 * @param file Reference to opened header file
 * @param display Reference to LCD display object
 * @return true if image parsed and loaded successfully
 */
bool loadHeaderFileImageToDisplay(File& file, LCD_Display& display);

// =============================================================================
// UTILITY FUNCTIONS - Helper Functions
// =============================================================================

/**
 * Validate display initialization and heap memory
 * Checks if display is ready and sufficient memory is available
 * 
 * @param display Reference to LCD display object
 * @return true if display is ready for image loading
 */
bool validateDisplayForImageLoading(LCD_Display& display);

/**
 * Get file extension from filename
 * Extracts and returns lowercase file extension
 * 
 * @param filename Full filename with extension
 * @return File extension string (without dot)
 */
String getFileExtension(const char* filename);

/**
 * Check if file is supported image format
 * Validates file extension against supported formats
 * 
 * @param filename Full filename to check
 * @return true if file format is supported
 */
bool isSupportedImageFormat(const char* filename);

#endif // IMAGE_DISPLAY_H