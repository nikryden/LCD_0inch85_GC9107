/*****************************************************************************
* | File        :   LCD_Display.cpp
* | Author      :   Modified for multiple display support
* | Function    :   Multi-display LCD driver class implementation
******************************************************************************/
#include "LCD_Display.h"
#include <Arduino.h>

// Constructor
LCD_Display::LCD_Display(uint8_t csPin, uint8_t dcPin, uint8_t rstPin, uint8_t blPin)
    : _csPin(csPin), _dcPin(dcPin), _rstPin(rstPin), _blPin(blPin),
      _imageBuffer(nullptr), _width(LCD_WIDTH), _height(LCD_HEIGHT),
      _rotate(ROTATE_0), _mirror(MIRROR_NONE), _backgroundColor(WHITE) {
    
    Serial.print("Initializing LCD Display - CS:");
    Serial.print(_csPin);
    Serial.print(" DC:");
    Serial.print(_dcPin);
    Serial.print(" RST:");
    Serial.print(_rstPin);
    Serial.print(" BL:");
    Serial.println(_blPin);
    
    // Initialize pins
    pinMode(_csPin, OUTPUT);
    pinMode(_dcPin, OUTPUT);
    pinMode(_rstPin, OUTPUT);
    pinMode(_blPin, OUTPUT);
    
    // Set default states
    digitalWrite(_csPin, HIGH);
    digitalWrite(_dcPin, HIGH);
    digitalWrite(_rstPin, HIGH);
    
    // Initialize backlight to off (don't use analogWrite in constructor)
    digitalWrite(_blPin, LOW);
    
    Serial.println("Pin initialization complete");
}

// Destructor
LCD_Display::~LCD_Display() {
    end();
}

// Initialize the display
bool LCD_Display::begin() {
    Serial.print("Free heap before init: ");
    Serial.println(ESP.getFreeHeap());
    
    // Initialize display hardware first (no buffer allocation yet)
    Serial.println("Performing hardware reset...");
    reset();
    
    Serial.println("Initializing LCD registers...");
    initRegisters();
    
    Serial.println("Clearing display hardware...");
    clear(_backgroundColor);
    
    Serial.println("Display hardware initialization complete");
    return true;
}

// Cleanup
void LCD_Display::end() {
    if (_imageBuffer != nullptr) {
        free(_imageBuffer);
        _imageBuffer = nullptr;
    }
}

// Hardware reset
void LCD_Display::reset() {
    digitalWrite(_csPin, LOW);
    delay(20);
    digitalWrite(_rstPin, LOW);
    delay(20);
    digitalWrite(_rstPin, HIGH);
    delay(20);
}

// Set backlight
void LCD_Display::setBacklight(UWORD value) {
    // Ensure value is within valid range
    if (value > 255) value = 255;
    
    // Use analogWrite for PWM control
    analogWrite(_blPin, value);
}

// Write data byte
void LCD_Display::writeData_Byte(UBYTE data) {
    digitalWrite(_csPin, LOW);
    digitalWrite(_dcPin, HIGH);
    SPI.transfer(data);
    digitalWrite(_csPin, HIGH);
}

// Write data word
void LCD_Display::writeData_Word(UWORD data) {
    UBYTE high = (data >> 8) & 0xff;
    digitalWrite(_csPin, LOW);
    digitalWrite(_dcPin, HIGH);
    SPI.transfer(high);
    SPI.transfer(data & 0xff);
    digitalWrite(_csPin, HIGH);
}

// Write register
void LCD_Display::writeReg(UBYTE reg) {
    digitalWrite(_csPin, LOW);
    digitalWrite(_dcPin, LOW);
    SPI.transfer(reg);
    digitalWrite(_csPin, HIGH);
}

// Initialize LCD registers
void LCD_Display::initRegisters() {
    writeReg(0x36);
    writeData_Byte(0xC8);

    // Start Initial Sequence
    writeReg(0xB0);
    writeData_Byte(0xC0);

    writeReg(0xB1);
    writeData_Byte(0x01);
    writeData_Byte(0x2C);
    writeData_Byte(0x2D);

    writeReg(0xB2);
    writeData_Byte(0x01);
    writeData_Byte(0x2C);
    writeData_Byte(0x2D);

    writeReg(0xB3);
    writeData_Byte(0x01);
    writeData_Byte(0x2C);
    writeData_Byte(0x2D);
    writeData_Byte(0x01);
    writeData_Byte(0x2C);
    writeData_Byte(0x2D);

    writeReg(0xB4);
    writeData_Byte(0x07);

    writeReg(0xC0);
    writeData_Byte(0xA2);
    writeData_Byte(0x02);
    writeData_Byte(0x84);
    writeReg(0xC1);
    writeData_Byte(0xC5);

    writeReg(0xC2);
    writeData_Byte(0x0A);
    writeData_Byte(0x00);

    writeReg(0xC3);
    writeData_Byte(0x8A);
    writeData_Byte(0x2A);
    writeReg(0xC4);
    writeData_Byte(0x8A);
    writeData_Byte(0xEE);

    writeReg(0xC5);
    writeData_Byte(0x0E);

    writeReg(0x3A);
    writeData_Byte(0x05);

    writeReg(0xE0);
    writeData_Byte(0x02);
    writeData_Byte(0x1c);
    writeData_Byte(0x07);
    writeData_Byte(0x12);
    writeData_Byte(0x37);
    writeData_Byte(0x32);
    writeData_Byte(0x29);
    writeData_Byte(0x2d);
    writeData_Byte(0x29);
    writeData_Byte(0x25);
    writeData_Byte(0x2B);
    writeData_Byte(0x39);
    writeData_Byte(0x00);
    writeData_Byte(0x01);
    writeData_Byte(0x03);
    writeData_Byte(0x10);

    writeReg(0xE1);
    writeData_Byte(0x03);
    writeData_Byte(0x1d);
    writeData_Byte(0x07);
    writeData_Byte(0x06);
    writeData_Byte(0x2E);
    writeData_Byte(0x2C);
    writeData_Byte(0x29);
    writeData_Byte(0x2D);
    writeData_Byte(0x2E);
    writeData_Byte(0x2E);
    writeData_Byte(0x37);
    writeData_Byte(0x3F);
    writeData_Byte(0x00);
    writeData_Byte(0x00);
    writeData_Byte(0x02);
    writeData_Byte(0x10);

    writeReg(0x13);
    writeReg(0x11);
    delay(120);
    writeReg(0x29);
}

// Set cursor for drawing area
void LCD_Display::setCursor(UWORD x1, UWORD y1, UWORD x2, UWORD y2) {
    writeReg(0x2a);
    writeData_Byte(0x00);
    writeData_Byte(x1 + 2);
    writeData_Byte(0x00);
    writeData_Byte(x2 + 2);

    writeReg(0x2b);
    writeData_Byte(0x00);
    writeData_Byte(y1 + 1);
    writeData_Byte(0x00);
    writeData_Byte(y2 + 1);
    writeReg(0x2c);
}

// Set pixel directly to hardware
void LCD_Display::setPixel_Hardware(UWORD x, UWORD y, UWORD color) {
    setCursor(x, y, x, y);
    writeData_Word(color);
}

// Clear display
void LCD_Display::clear(UWORD color) {
    Serial.print("Clearing display to color: 0x");
    Serial.println(color, HEX);
    
    setCursor(0, 0, _width - 1, _height - 1);
    
    digitalWrite(_csPin, LOW);
    digitalWrite(_dcPin, HIGH);
    
    UBYTE colorHigh = (color >> 8) & 0xff;
    UBYTE colorLow = color & 0xff;
    UWORD pixelCount = 0;
    
    for (UWORD i = 0; i < _width; i++) {
        for (UWORD j = 0; j < _height; j++) {
            SPI.transfer(colorHigh);
            SPI.transfer(colorLow);
            
            pixelCount++;
            // Yield every 128 pixels to prevent watchdog reset
            if (pixelCount % 128 == 0) {
                yield();
            }
        }
    }
    
    digitalWrite(_csPin, HIGH);
    
    Serial.println("Display hardware cleared");
}

// Clear a window area
void LCD_Display::clearWindow(UWORD xStart, UWORD yStart, UWORD xEnd, UWORD yEnd, UWORD color) {
    setCursor(xStart, yStart, xEnd, yEnd);
    
    digitalWrite(_csPin, LOW);
    digitalWrite(_dcPin, HIGH);
    
    for (UWORD i = yStart; i <= yEnd; i++) {
        for (UWORD j = xStart; j <= xEnd; j++) {
            SPI.transfer((color >> 8) & 0xff);
            SPI.transfer(color & 0xff);
        }
    }
    
    digitalWrite(_csPin, HIGH);
}

// Create new image buffer
void LCD_Display::newImage(UWORD width, UWORD height, UWORD rotate, UWORD color) {
    Serial.print("Creating image buffer - Size: ");
    Serial.print(width);
    Serial.print("x");
    Serial.print(height);
    
    _width = width;
    _height = height;
    _rotate = rotate;
    _backgroundColor = color;
    
    // Free existing buffer if it exists
    if (_imageBuffer != nullptr) {
        Serial.println(" (freeing old buffer)");
        free(_imageBuffer);
        _imageBuffer = nullptr;
    }
    
    // Calculate buffer size
    UDOUBLE bufferSize = (UDOUBLE)_width * _height * 2; // 16-bit color
    Serial.print("Buffer size needed: ");
    Serial.print(bufferSize);
    Serial.println(" bytes");
    
    Serial.print("Free heap: ");
    Serial.println(ESP.getFreeHeap());
    
    if (bufferSize > ESP.getFreeHeap() - 10000) { // Leave 10KB safety margin
        Serial.println("ERROR: Not enough memory for buffer!");
        return;
    }
    
    _imageBuffer = (UBYTE*)malloc(bufferSize);
    if (_imageBuffer == nullptr) {
        Serial.println("ERROR: Failed to allocate image buffer!");
        return;
    }
    
    Serial.println("Buffer allocated successfully");
    
    // Initialize buffer efficiently using memset instead of loop
    Serial.println("Initializing buffer...");
    if (color == 0x0000) {
        // For black, use memset which is faster
        memset(_imageBuffer, 0x00, bufferSize);
    } else if (color == 0xFFFF) {
        // For white, use memset which is faster
        memset(_imageBuffer, 0xFF, bufferSize);
    } else {
        // For other colors, we need to set 16-bit values
        UBYTE colorHigh = (color >> 8) & 0xFF;
        UBYTE colorLow = color & 0xFF;
        
        for (UDOUBLE i = 0; i < bufferSize; i += 2) {
            _imageBuffer[i] = colorHigh;
            _imageBuffer[i + 1] = colorLow;
            
            // Yield to watchdog every 1000 pixels to prevent reset
            if (i % 2000 == 0) {
                yield();
            }
        }
    }
    
    Serial.println("Image buffer initialization complete");
}

// Set rotation
void LCD_Display::setRotate(UWORD rotate) {
    _rotate = rotate;
}

// Set mirroring
void LCD_Display::setMirroring(UBYTE mirror) {
    _mirror = mirror;
}

// Set pixel in buffer
void LCD_Display::setPixel(UWORD x, UWORD y, UWORD color) {
    if (_imageBuffer == nullptr || x >= _width || y >= _height) {
        return;
    }
    
    // Apply rotation and mirroring transformations
    UWORD newX = x, newY = y;
    
    switch (_rotate) {
        case ROTATE_0:
            newX = x;
            newY = y;
            break;
        case ROTATE_90:
            newX = _width - y - 1;
            newY = x;
            break;
        case ROTATE_180:
            newX = _width - x - 1;
            newY = _height - y - 1;
            break;
        case ROTATE_270:
            newX = y;
            newY = _height - x - 1;
            break;
    }
    
    // Apply mirroring
    if (_mirror & MIRROR_HORIZONTAL) {
        newX = _width - newX - 1;
    }
    if (_mirror & MIRROR_VERTICAL) {
        newY = _height - newY - 1;
    }
    
    UDOUBLE addr = (newY * _width + newX) * 2;
    _imageBuffer[addr] = (color >> 8) & 0xff;
    _imageBuffer[addr + 1] = color & 0xff;
}

// Get pixel from buffer
UWORD LCD_Display::getPixel(UWORD x, UWORD y) {
    if (_imageBuffer == nullptr || x >= _width || y >= _height) {
        return 0;
    }
    
    UDOUBLE addr = (y * _width + x) * 2;
    return (_imageBuffer[addr] << 8) | _imageBuffer[addr + 1];
}

// Display buffer contents
void LCD_Display::display() {
    if (_imageBuffer == nullptr) {
        return;
    }
    
    setCursor(0, 0, _width - 1, _height - 1);
    
    digitalWrite(_csPin, LOW);
    digitalWrite(_dcPin, HIGH);
    
    for (UDOUBLE i = 0; i < _width * _height * 2; i++) {
        SPI.transfer(_imageBuffer[i]);
    }
    
    digitalWrite(_csPin, HIGH);
}

// Draw point with style
void LCD_Display::drawPoint(UWORD x, UWORD y, UWORD color, DOT_PIXEL dotPixel, DOT_STYLE dotStyle) {
    if (x >= _width || y >= _height) {
        return;
    }
    
    int16_t xDir_Num, yDir_Num;
    if (dotStyle == DOT_FILL_AROUND) {
        for (xDir_Num = 0; xDir_Num < 2 * dotPixel - 1; xDir_Num++) {
            for (yDir_Num = 0; yDir_Num < 2 * dotPixel - 1; yDir_Num++) {
                if (x + xDir_Num - dotPixel < _width && y + yDir_Num - dotPixel < _height) {
                    setPixel(x + xDir_Num - dotPixel, y + yDir_Num - dotPixel, color);
                }
            }
        }
    } else {
        for (xDir_Num = 0; xDir_Num < dotPixel; xDir_Num++) {
            for (yDir_Num = 0; yDir_Num < dotPixel; yDir_Num++) {
                if (x + xDir_Num < _width && y + yDir_Num < _height) {
                    setPixel(x + xDir_Num, y + yDir_Num, color);
                }
            }
        }
    }
}

// Draw line
void LCD_Display::drawLine(UWORD xStart, UWORD yStart, UWORD xEnd, UWORD yEnd, UWORD color, DOT_PIXEL lineWidth, LINE_STYLE lineStyle) {
    UWORD xPoint = xStart;
    UWORD yPoint = yStart;
    int dx = (int)xEnd - (int)xStart >= 0 ? xEnd - xStart : xStart - xEnd;
    int dy = (int)yEnd - (int)yStart <= 0 ? yEnd - yStart : yStart - yEnd;

    // Increment direction
    int xAddway = xStart < xEnd ? 1 : -1;
    int yAddway = yStart < yEnd ? 1 : -1;

    // Cumulative error
    int esp = dx + dy;
    char dotted_len = 0;

    for (;;) {
        dotted_len++;
        // Determine whether to draw
        if (lineStyle == LINE_STYLE_DOTTED && dotted_len % 3 == 0) {
            drawPoint(xPoint, yPoint, _backgroundColor, lineWidth, DOT_FILL_AROUND);
            dotted_len = 0;
        } else {
            drawPoint(xPoint, yPoint, color, lineWidth, DOT_FILL_AROUND);
        }
        if (2 * esp >= dy) {
            if (xPoint == xEnd) break;
            esp += dy;
            xPoint += xAddway;
        }
        if (2 * esp <= dx) {
            if (yPoint == yEnd) break;
            esp += dx;
            yPoint += yAddway;
        }
    }
}

// Draw rectangle
void LCD_Display::drawRectangle(UWORD xStart, UWORD yStart, UWORD xEnd, UWORD yEnd, UWORD color, DOT_PIXEL lineWidth, DRAW_FILL filled) {
    if (filled == DRAW_FILL_FULL) {
        for (UWORD yPoint = yStart; yPoint < yEnd; yPoint++) {
            drawLine(xStart, yPoint, xEnd, yPoint, color, lineWidth, LINE_STYLE_SOLID);
        }
    } else {
        drawLine(xStart, yStart, xEnd, yStart, color, lineWidth, LINE_STYLE_SOLID);
        drawLine(xStart, yStart, xStart, yEnd, color, lineWidth, LINE_STYLE_SOLID);
        drawLine(xEnd, yEnd, xEnd, yStart, color, lineWidth, LINE_STYLE_SOLID);
        drawLine(xEnd, yEnd, xStart, yEnd, color, lineWidth, LINE_STYLE_SOLID);
    }
}

// Draw circle
void LCD_Display::drawCircle(UWORD xCenter, UWORD yCenter, UWORD radius, UWORD color, DOT_PIXEL lineWidth, DRAW_FILL drawFill) {
    if (xCenter >= _width || yCenter >= _height) {
        return;
    }

    // Bresenham algorithm
    int16_t xCurrent, yCurrent;
    int16_t esp;

    int16_t sCountY;
    if (drawFill == DRAW_FILL_FULL) {
        for (xCurrent = xCenter - radius; xCurrent <= xCenter + radius; xCurrent++) {
            for (yCurrent = yCenter - radius; yCurrent <= yCenter + radius; yCurrent++) {
                if ((xCurrent - xCenter) * (xCurrent - xCenter) + (yCurrent - yCenter) * (yCurrent - yCenter) <= radius * radius) {
                    drawPoint(xCurrent, yCurrent, color, lineWidth, DOT_FILL_AROUND);
                }
            }
        }
    } else {
        xCurrent = 0;
        yCurrent = radius;
        esp = 3 - (radius << 1);

        while (xCurrent <= yCurrent) {
            drawPoint(xCenter + xCurrent, yCenter + yCurrent, color, lineWidth, DOT_FILL_AROUND);
            drawPoint(xCenter - xCurrent, yCenter + yCurrent, color, lineWidth, DOT_FILL_AROUND);
            drawPoint(xCenter - yCurrent, yCenter + xCurrent, color, lineWidth, DOT_FILL_AROUND);
            drawPoint(xCenter - yCurrent, yCenter - xCurrent, color, lineWidth, DOT_FILL_AROUND);
            drawPoint(xCenter - xCurrent, yCenter - yCurrent, color, lineWidth, DOT_FILL_AROUND);
            drawPoint(xCenter + xCurrent, yCenter - yCurrent, color, lineWidth, DOT_FILL_AROUND);
            drawPoint(xCenter + yCurrent, yCenter - xCurrent, color, lineWidth, DOT_FILL_AROUND);
            drawPoint(xCenter + yCurrent, yCenter + xCurrent, color, lineWidth, DOT_FILL_AROUND);
            if (esp < 0)
                esp += 4 * xCurrent + 6;
            else {
                esp += 10 + 4 * (xCurrent - yCurrent);
                yCurrent--;
            }
            xCurrent++;
        }
    }
}

// Draw character
void LCD_Display::drawChar(UWORD x, UWORD y, const char character, sFONT* font, UWORD colorBackground, UWORD colorForeground) {
    UWORD page, column;

    if (x >= _width || y >= _height) {
        return;
    }

    uint32_t char_offset = (character - ' ') * font->Height * (font->Width / 8 + (font->Width % 8 ? 1 : 0));
    const unsigned char* ptr = &font->table[char_offset];

    for (page = 0; page < font->Height; page++) {
        for (column = 0; column < font->Width; column++) {
            // To determine whether the font background color and screen background color is consistent
            if (pgm_read_byte(ptr) & (0x80 >> (column % 8))) {
                setPixel(x + column, y + page, colorForeground);
            } else if (colorBackground != colorForeground) {
                setPixel(x + column, y + page, colorBackground);
            }
            // One pixel is 8 bits
            if (column % 8 == 7) {
                ptr++;
            }
        }
        if (font->Width % 8 != 0) {
            ptr++;
        }
    }
}

// Draw string
void LCD_Display::drawString(UWORD x, UWORD y, const char* string, sFONT* font, UWORD colorBackground, UWORD colorForeground) {
    UWORD xStart = x;

    while (*string != '\0') {
        // If newline
        if (*string == '\n') {
            xStart = x;
            y += font->Height;
        } else {
            // If the display exceeds the screen, wrap around to the next line
            if ((xStart + font->Width) > _width) {
                xStart = x;
                y += font->Height;
            }

            // If the display exceeds the screen, it will not display
            if ((xStart + font->Width) > _width || (y + font->Height) > _height) {
                break;
            }

            drawChar(xStart, y, *string, font, colorBackground, colorForeground);
            xStart += font->Width;
        }
        string++;
    }
}

// Draw number
void LCD_Display::drawNumber(UWORD x, UWORD y, int32_t number, sFONT* font, UWORD colorBackground, UWORD colorForeground) {
    int16_t numberLength = 0;
    int32_t temp = number;
    UBYTE isNegative = 0;
    
    // Handle negative numbers
    if (number < 0) {
        isNegative = 1;
        number = -number;
    }

    // Calculate number length
    if (number == 0) {
        numberLength = 1;
    } else {
        while (temp) {
            numberLength++;
            temp /= 10;
        }
    }

    if (isNegative) {
        numberLength++;
    }

    char* str = (char*)malloc(numberLength + 1);
    if (str == nullptr) {
        return;
    }

    sprintf(str, "%ld", (long)number);
    if (isNegative) {
        str[0] = '-';
        sprintf(&str[1], "%ld", (long)(-number));
    }

    drawString(x, y, str, font, colorBackground, colorForeground);
    free(str);
}

// Draw image
void LCD_Display::drawImage(const unsigned char* image, UWORD x, UWORD y, UWORD width, UWORD height) {
    UWORD i, j;
    const unsigned char* ptr = image;

    for (j = y; j < y + height; j++) {
        for (i = x; i < x + width; i++) {
            if (i < _width && j < _height) {
                UWORD color = (pgm_read_byte(ptr) << 8) | pgm_read_byte(ptr + 1);
                setPixel(i, j, color);
                ptr += 2;
            }
        }
    }
}