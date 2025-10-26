/**
 * @file BasicTest.ino
 * @brief Basic test example for GC9107 LCD display
 * 
 * This example demonstrates basic usage of the GC9107 library.
 * It fills the screen with different colors and draws some shapes.
 * 
 * Hardware Setup:
 * - Connect your GC9107 display to your Arduino/ESP32
 * - Default pins used in this example:
 *   CS  = 10 (or 5 for ESP32)
 *   DC  = 9  (or 4 for ESP32)
 *   RST = 8  (or 2 for ESP32)
 *   SDA = MOSI (11 on Arduino, GPIO23 on ESP32)
 *   SCL = SCK  (13 on Arduino, GPIO18 on ESP32)
 * 
 * Adjust the pin definitions below for your setup.
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

void setup() {
  Serial.begin(115200);
  Serial.println("GC9107 Basic Test");

  // Initialize the display
  display.begin(128, 128);
  
  Serial.println("Display initialized");
}

void loop() {
  // Fill screen with different colors
  Serial.println("Red");
  display.fillScreen(GC9107_RED);
  delay(1000);
  
  Serial.println("Green");
  display.fillScreen(GC9107_GREEN);
  delay(1000);
  
  Serial.println("Blue");
  display.fillScreen(GC9107_BLUE);
  delay(1000);
  
  Serial.println("White");
  display.fillScreen(GC9107_WHITE);
  delay(1000);
  
  Serial.println("Black");
  display.fillScreen(GC9107_BLACK);
  delay(1000);
  
  // Draw some rectangles
  Serial.println("Drawing rectangles");
  display.fillScreen(GC9107_BLACK);
  display.fillRect(10, 10, 30, 30, GC9107_RED);
  display.fillRect(50, 10, 30, 30, GC9107_GREEN);
  display.fillRect(90, 10, 30, 30, GC9107_BLUE);
  display.fillRect(10, 50, 30, 30, GC9107_YELLOW);
  display.fillRect(50, 50, 30, 30, GC9107_CYAN);
  display.fillRect(90, 50, 30, 30, GC9107_MAGENTA);
  delay(2000);
  
  // Draw lines
  Serial.println("Drawing lines");
  display.fillScreen(GC9107_BLACK);
  for (int i = 0; i < 128; i += 10) {
    display.drawFastHLine(0, i, 128, GC9107_WHITE);
  }
  delay(1000);
  
  display.fillScreen(GC9107_BLACK);
  for (int i = 0; i < 128; i += 10) {
    display.drawFastVLine(i, 0, 128, GC9107_WHITE);
  }
  delay(1000);
  
  // Draw pixels
  Serial.println("Drawing random pixels");
  display.fillScreen(GC9107_BLACK);
  for (int i = 0; i < 500; i++) {
    int x = random(128);
    int y = random(128);
    uint16_t color = random(0xFFFF);
    display.drawPixel(x, y, color);
  }
  delay(2000);
}
