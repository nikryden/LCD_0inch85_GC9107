/*****************************************************************************
* | File        :   LCD_Display.h
* | Author      :   Modified for multiple display support
* | Function    :   Multi-display LCD driver class
* | Info        :   Supports multiple 0.85" LCD displays simultaneously
******************************************************************************/
#ifndef __LCD_DISPLAY_H
#define __LCD_DISPLAY_H

#include <SPI.h>
#include <stdint.h>
#include "fonts.h"

#define UBYTE   uint8_t
#define UWORD   uint16_t
#define UDOUBLE uint32_t

#define LCD_WIDTH   128 //LCD width
#define LCD_HEIGHT  128 //LCD height

// Color definitions
#define WHITE   0xFFFF
#define BLACK   0x0000
#define BLUE    0x001F
#define BRED    0XF81F
#define GRED    0XFFE0
#define GBLUE   0X07FF
#define RED     0xF800
#define MAGENTA 0xF81F
#define GREEN   0x07E0
#define CYAN    0x7FFF
#define YELLOW  0xFFE0
#define BROWN   0XBC40
#define BRRED   0XFC07
#define GRAY    0X8430

// Display rotation
#define ROTATE_0            0
#define ROTATE_90           90
#define ROTATE_180          180
#define ROTATE_270          270

// Drawing enums
typedef enum {
    DOT_PIXEL_1X1  = 1,
    DOT_PIXEL_2X2,
    DOT_PIXEL_3X3,
    DOT_PIXEL_4X4,
    DOT_PIXEL_5X5,
    DOT_PIXEL_6X6,
    DOT_PIXEL_7X7,
    DOT_PIXEL_8X8,
} DOT_PIXEL;

typedef enum {
    DOT_FILL_AROUND  = 1,
    DOT_FILL_RIGHTUP,
} DOT_STYLE;

typedef enum {
    LINE_STYLE_SOLID = 0,
    LINE_STYLE_DOTTED,
} LINE_STYLE;

typedef enum {
    DRAW_FILL_EMPTY = 0,
    DRAW_FILL_FULL,
} DRAW_FILL;

typedef enum {
    MIRROR_NONE  = 0x00,
    MIRROR_HORIZONTAL = 0x01,
    MIRROR_VERTICAL = 0x02,
    MIRROR_ORIGIN = 0x03,
} MIRROR_IMAGE;

class LCD_Display {
private:
    // Hardware pins
    uint8_t _csPin;
    uint8_t _dcPin;
    uint8_t _rstPin;
    uint8_t _blPin;
    
    // Display buffer
    UBYTE *_imageBuffer;
    UWORD _width;
    UWORD _height;
    UWORD _widthByte;
    UWORD _heightByte;
    UWORD _rotate;
    UWORD _mirror;
    UWORD _backgroundColor;
    
    // Private hardware interface methods
    void writeData_Byte(UBYTE data);
    void writeData_Word(UWORD data);
    void writeReg(UBYTE reg);
    void reset();
    void initRegisters();
    void setCursor(UWORD x1, UWORD y1, UWORD x2, UWORD y2);
    void setPixel_Hardware(UWORD x, UWORD y, UWORD color);
    
public:
    // Constructor
    LCD_Display(uint8_t csPin, uint8_t dcPin, uint8_t rstPin, uint8_t blPin);
    
    // Destructor
    ~LCD_Display();
    
    // Initialization
    bool begin();
    void end();
    
    // Display control
    void setBacklight(UWORD value);
    void clear(UWORD color);
    void clearWindow(UWORD xStart, UWORD yStart, UWORD xEnd, UWORD yEnd, UWORD color);
    void display(); // Send buffer to LCD
    
    // Image/Paint setup
    void newImage(UWORD width, UWORD height, UWORD rotate, UWORD color);
    void setRotate(UWORD rotate);
    void setMirroring(UBYTE mirror);
    
    // Drawing methods
    void setPixel(UWORD x, UWORD y, UWORD color);
    UWORD getPixel(UWORD x, UWORD y);
    void drawPoint(UWORD x, UWORD y, UWORD color, DOT_PIXEL dotPixel = DOT_PIXEL_1X1, DOT_STYLE dotStyle = DOT_FILL_AROUND);
    void drawLine(UWORD xStart, UWORD yStart, UWORD xEnd, UWORD yEnd, UWORD color, DOT_PIXEL lineWidth = DOT_PIXEL_1X1, LINE_STYLE lineStyle = LINE_STYLE_SOLID);
    void drawRectangle(UWORD xStart, UWORD yStart, UWORD xEnd, UWORD yEnd, UWORD color, DOT_PIXEL lineWidth = DOT_PIXEL_1X1, DRAW_FILL filled = DRAW_FILL_EMPTY);
    void drawCircle(UWORD xCenter, UWORD yCenter, UWORD radius, UWORD color, DOT_PIXEL lineWidth = DOT_PIXEL_1X1, DRAW_FILL drawFill = DRAW_FILL_EMPTY);
    
    // Text drawing
    void drawChar(UWORD x, UWORD y, const char character, sFONT* font, UWORD colorBackground, UWORD colorForeground);
    void drawString(UWORD x, UWORD y, const char* string, sFONT* font, UWORD colorBackground, UWORD colorForeground);
    void drawNumber(UWORD x, UWORD y, int32_t number, sFONT* font, UWORD colorBackground, UWORD colorForeground);
    
    // Image drawing
    void drawImage(const unsigned char* image, UWORD x, UWORD y, UWORD width, UWORD height);
    
    // Utility methods
    UWORD getWidth() const { return _width; }
    UWORD getHeight() const { return _height; }
    bool isInitialized() const { return _imageBuffer != nullptr; }
};

#endif // __LCD_DISPLAY_H