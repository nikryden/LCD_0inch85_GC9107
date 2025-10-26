/**
 * @file RotationTest.ino
 * @brief Display rotation test example for GC9107 LCD
 * 
 * This example demonstrates the display rotation functionality.
 * It shows how to rotate the display orientation in 90-degree increments.
 * 
 * Hardware Setup:
 * - Connect your GC9107 display to your Arduino/ESP32
 * - Default pins used in this example:
 *   CS  = 10 (or 5 for ESP32)
 *   DC  = 9  (or 4 for ESP32)
 *   RST = 8  (or 2 for ESP32)
 *   SDA = MOSI (11 on Arduino, GPIO23 on ESP32)
 *   SCL = SCK  (13 on Arduino, GPIO18 on ESP32)
 */

#include <GC9107.h>
#include <SPI.h>

// Pin definitions - adjust for your board
#if defined(ESP32)
  #define TFT_CS   5
  #define TFT_DC   4
  #define TFT_RST  2
#else
  #define TFT_CS   10
  #define TFT_DC   9
  #define TFT_RST  8
#endif

// Create display object
GC9107 display(TFT_CS, TFT_DC, TFT_RST);

void drawTestPattern() {
  display.fillScreen(GC9107_BLACK);
  
  // Draw colored rectangles in corners
  display.fillRect(0, 0, 30, 30, GC9107_RED);        // Top-left
  display.fillRect(98, 0, 30, 30, GC9107_GREEN);     // Top-right
  display.fillRect(0, 98, 30, 30, GC9107_BLUE);      // Bottom-left
  display.fillRect(98, 98, 30, 30, GC9107_YELLOW);   // Bottom-right
  
  // Draw a line across the middle
  display.drawFastHLine(0, 64, 128, GC9107_WHITE);
  display.drawFastVLine(64, 0, 128, GC9107_WHITE);
}

void setup() {
  Serial.begin(115200);
  Serial.println("GC9107 Rotation Test");

  // Initialize the display
  display.begin(128, 128);
  
  Serial.println("Display initialized");
}

void loop() {
  // Test each rotation
  for (int rotation = 0; rotation < 4; rotation++) {
    Serial.print("Rotation: ");
    Serial.println(rotation);
    
    display.setRotation(rotation);
    drawTestPattern();
    
    delay(2000);
  }
  
  // Test display inversion
  Serial.println("Testing display inversion");
  display.setRotation(0);
  drawTestPattern();
  delay(1000);
  
  display.invertDisplay(true);
  delay(1000);
  
  display.invertDisplay(false);
  delay(1000);
}
