/**
 * @file GC9107.h
 * @brief GC9107 LCD Display Driver Library
 * 
 * This library provides support for LCD displays using the GC9107 driver chip.
 * Compatible with 0.85" and other display sizes using GC9107.
 * 
 * @author nikryden
 * @license GPL-3.0
 */

#ifndef GC9107_H
#define GC9107_H

#include <Arduino.h>
#include <SPI.h>

// GC9107 Display Commands
#define GC9107_NOP        0x00
#define GC9107_SWRESET    0x01
#define GC9107_RDDID      0x04
#define GC9107_RDDST      0x09

#define GC9107_SLPIN      0x10
#define GC9107_SLPOUT     0x11
#define GC9107_PTLON      0x12
#define GC9107_NORON      0x13

#define GC9107_INVOFF     0x20
#define GC9107_INVON      0x21
#define GC9107_DISPOFF    0x28
#define GC9107_DISPON     0x29

#define GC9107_CASET      0x2A
#define GC9107_RASET      0x2B
#define GC9107_RAMWR      0x2C
#define GC9107_RAMRD      0x2E

#define GC9107_PTLAR      0x30
#define GC9107_MADCTL     0x36
#define GC9107_COLMOD     0x3A

#define GC9107_FRMCTR1    0xB1
#define GC9107_FRMCTR2    0xB2
#define GC9107_FRMCTR3    0xB3
#define GC9107_INVCTR     0xB4

#define GC9107_PWCTR1     0xC0
#define GC9107_PWCTR2     0xC1
#define GC9107_PWCTR3     0xC2
#define GC9107_PWCTR4     0xC3
#define GC9107_PWCTR5     0xC4
#define GC9107_VMCTR1     0xC5

#define GC9107_GMCTRP1    0xE0
#define GC9107_GMCTRN1    0xE1

// Color definitions (RGB565)
#define GC9107_BLACK      0x0000
#define GC9107_WHITE      0xFFFF
#define GC9107_RED        0xF800
#define GC9107_GREEN      0x07E0
#define GC9107_BLUE       0x001F
#define GC9107_CYAN       0x07FF
#define GC9107_MAGENTA    0xF81F
#define GC9107_YELLOW     0xFFE0
#define GC9107_ORANGE     0xFC00

// Display rotation
#define GC9107_ROTATION_0   0
#define GC9107_ROTATION_90  1
#define GC9107_ROTATION_180 2
#define GC9107_ROTATION_270 3

/**
 * @class GC9107
 * @brief Main driver class for GC9107 LCD displays
 */
class GC9107 {
public:
    /**
     * @brief Constructor for GC9107 display
     * @param cs Chip Select pin
     * @param dc Data/Command pin
     * @param rst Reset pin (optional, use -1 if not connected)
     */
    GC9107(int8_t cs, int8_t dc, int8_t rst = -1);

    /**
     * @brief Initialize the display
     * @param width Display width in pixels
     * @param height Display height in pixels
     */
    void begin(uint16_t width = 128, uint16_t height = 128);

    /**
     * @brief Set the display rotation
     * @param rotation Rotation value (0-3)
     */
    void setRotation(uint8_t rotation);

    /**
     * @brief Invert display colors
     * @param invert true to invert, false for normal
     */
    void invertDisplay(bool invert);

    /**
     * @brief Fill entire screen with a color
     * @param color 16-bit RGB565 color
     */
    void fillScreen(uint16_t color);

    /**
     * @brief Draw a single pixel
     * @param x X coordinate
     * @param y Y coordinate
     * @param color 16-bit RGB565 color
     */
    void drawPixel(int16_t x, int16_t y, uint16_t color);

    /**
     * @brief Draw a filled rectangle
     * @param x X coordinate
     * @param y Y coordinate
     * @param w Width
     * @param h Height
     * @param color 16-bit RGB565 color
     */
    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);

    /**
     * @brief Draw a horizontal line
     * @param x X coordinate
     * @param y Y coordinate
     * @param w Width
     * @param color 16-bit RGB565 color
     */
    void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);

    /**
     * @brief Draw a vertical line
     * @param x X coordinate
     * @param y Y coordinate
     * @param h Height
     * @param color 16-bit RGB565 color
     */
    void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);

    /**
     * @brief Draw an RGB bitmap image
     * @param x X coordinate
     * @param y Y coordinate
     * @param bitmap Pointer to 16-bit RGB565 bitmap data
     * @param w Width
     * @param h Height
     */
    void drawRGBBitmap(int16_t x, int16_t y, const uint16_t *bitmap, int16_t w, int16_t h);

    /**
     * @brief Get display width
     * @return Width in pixels
     */
    uint16_t width() const { return _width; }

    /**
     * @brief Get display height
     * @return Height in pixels
     */
    uint16_t height() const { return _height; }

    /**
     * @brief Turn display on
     */
    void displayOn();

    /**
     * @brief Turn display off
     */
    void displayOff();

    /**
     * @brief Enter sleep mode (low power)
     */
    void sleepMode(bool enable);

protected:
    /**
     * @brief Send a command to the display
     * @param cmd Command byte
     */
    void writeCommand(uint8_t cmd);

    /**
     * @brief Send data to the display
     * @param data Data byte
     */
    void writeData(uint8_t data);

    /**
     * @brief Send 16-bit data to the display
     * @param data 16-bit data
     */
    void writeData16(uint16_t data);

    /**
     * @brief Set address window for drawing
     * @param x0 Start X
     * @param y0 Start Y
     * @param x1 End X
     * @param y1 End Y
     */
    void setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

    /**
     * @brief Hardware reset the display
     */
    void hardwareReset();

    /**
     * @brief Initialize display registers
     */
    void initRegisters();

    /**
     * @brief Start SPI transaction
     */
    void startWrite();

    /**
     * @brief End SPI transaction
     */
    void endWrite();

private:
    int8_t _cs;
    int8_t _dc;
    int8_t _rst;
    uint16_t _width;
    uint16_t _height;
    uint8_t _rotation;
    SPISettings _spiSettings;
};

#endif // GC9107_H
