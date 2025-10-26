/**
 * @file GC9107.cpp
 * @brief GC9107 LCD Display Driver Library Implementation
 * 
 * @author nikryden
 * @license GPL-3.0
 */

#include "GC9107.h"

// SPI settings: 40MHz, MSB first, SPI Mode 0
#define GC9107_SPI_SPEED 40000000

GC9107::GC9107(int8_t cs, int8_t dc, int8_t rst)
    : _cs(cs), _dc(dc), _rst(rst), _width(128), _height(128), _rotation(0),
      _spiSettings(GC9107_SPI_SPEED, MSBFIRST, SPI_MODE0) {
}

void GC9107::begin(uint16_t width, uint16_t height) {
    _width = width;
    _height = height;

    // Initialize pins
    pinMode(_cs, OUTPUT);
    pinMode(_dc, OUTPUT);
    if (_rst >= 0) {
        pinMode(_rst, OUTPUT);
    }

    digitalWrite(_cs, HIGH);
    digitalWrite(_dc, HIGH);

    SPI.begin();

    hardwareReset();
    initRegisters();
}

void GC9107::hardwareReset() {
    if (_rst >= 0) {
        digitalWrite(_rst, HIGH);
        delay(10);
        digitalWrite(_rst, LOW);
        delay(10);
        digitalWrite(_rst, HIGH);
        delay(120);
    }
}

void GC9107::initRegisters() {
    startWrite();

    // Software reset
    writeCommand(GC9107_SWRESET);
    delay(120);

    // Sleep out
    writeCommand(GC9107_SLPOUT);
    delay(120);

    // Interface Pixel Format - 16bit color (RGB565)
    writeCommand(GC9107_COLMOD);
    writeData(0x05);

    // Memory Data Access Control
    writeCommand(GC9107_MADCTL);
    writeData(0x00);

    // Frame Rate Control (In normal mode/Full colors)
    writeCommand(GC9107_FRMCTR1);
    writeData(0x01);
    writeData(0x2C);
    writeData(0x2D);

    // Frame Rate Control (In Idle mode/8-colors)
    writeCommand(GC9107_FRMCTR2);
    writeData(0x01);
    writeData(0x2C);
    writeData(0x2D);

    // Frame Rate Control (In Partial mode/full colors)
    writeCommand(GC9107_FRMCTR3);
    writeData(0x01);
    writeData(0x2C);
    writeData(0x2D);
    writeData(0x01);
    writeData(0x2C);
    writeData(0x2D);

    // Display Inversion Control
    writeCommand(GC9107_INVCTR);
    writeData(0x07);

    // Power Control 1
    writeCommand(GC9107_PWCTR1);
    writeData(0xA2);
    writeData(0x02);
    writeData(0x84);

    // Power Control 2
    writeCommand(GC9107_PWCTR2);
    writeData(0xC5);

    // Power Control 3
    writeCommand(GC9107_PWCTR3);
    writeData(0x0A);
    writeData(0x00);

    // Power Control 4
    writeCommand(GC9107_PWCTR4);
    writeData(0x8A);
    writeData(0x2A);

    // Power Control 5
    writeCommand(GC9107_PWCTR5);
    writeData(0x8A);
    writeData(0xEE);

    // VCOM Control 1
    writeCommand(GC9107_VMCTR1);
    writeData(0x0E);

    // Positive Gamma Control
    writeCommand(GC9107_GMCTRP1);
    writeData(0x02);
    writeData(0x1C);
    writeData(0x07);
    writeData(0x12);
    writeData(0x37);
    writeData(0x32);
    writeData(0x29);
    writeData(0x2D);
    writeData(0x29);
    writeData(0x25);
    writeData(0x2B);
    writeData(0x39);
    writeData(0x00);
    writeData(0x01);
    writeData(0x03);
    writeData(0x10);

    // Negative Gamma Control
    writeCommand(GC9107_GMCTRN1);
    writeData(0x03);
    writeData(0x1D);
    writeData(0x07);
    writeData(0x06);
    writeData(0x2E);
    writeData(0x2C);
    writeData(0x29);
    writeData(0x2D);
    writeData(0x2E);
    writeData(0x2E);
    writeData(0x37);
    writeData(0x3F);
    writeData(0x00);
    writeData(0x00);
    writeData(0x02);
    writeData(0x10);

    // Normal Display Mode On
    writeCommand(GC9107_NORON);
    delay(10);

    // Display On
    writeCommand(GC9107_DISPON);
    delay(100);

    endWrite();
}

void GC9107::setRotation(uint8_t rotation) {
    _rotation = rotation % 4;

    startWrite();
    writeCommand(GC9107_MADCTL);

    switch (_rotation) {
        case GC9107_ROTATION_0:
            writeData(0x00);
            _width = 128;
            _height = 128;
            break;
        case GC9107_ROTATION_90:
            writeData(0x60);
            _width = 128;
            _height = 128;
            break;
        case GC9107_ROTATION_180:
            writeData(0xC0);
            _width = 128;
            _height = 128;
            break;
        case GC9107_ROTATION_270:
            writeData(0xA0);
            _width = 128;
            _height = 128;
            break;
    }

    endWrite();
}

void GC9107::invertDisplay(bool invert) {
    startWrite();
    writeCommand(invert ? GC9107_INVON : GC9107_INVOFF);
    endWrite();
}

void GC9107::displayOn() {
    startWrite();
    writeCommand(GC9107_DISPON);
    endWrite();
}

void GC9107::displayOff() {
    startWrite();
    writeCommand(GC9107_DISPOFF);
    endWrite();
}

void GC9107::sleepMode(bool enable) {
    startWrite();
    writeCommand(enable ? GC9107_SLPIN : GC9107_SLPOUT);
    endWrite();
    delay(120);
}

void GC9107::setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    // Column address set
    writeCommand(GC9107_CASET);
    writeData16(x0);
    writeData16(x1);

    // Row address set
    writeCommand(GC9107_RASET);
    writeData16(y0);
    writeData16(y1);

    // Write to RAM
    writeCommand(GC9107_RAMWR);
}

void GC9107::fillScreen(uint16_t color) {
    fillRect(0, 0, _width, _height, color);
}

void GC9107::drawPixel(int16_t x, int16_t y, uint16_t color) {
    if ((x < 0) || (x >= _width) || (y < 0) || (y >= _height)) {
        return;
    }

    startWrite();
    setAddrWindow(x, y, x, y);
    writeData16(color);
    endWrite();
}

void GC9107::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    if ((x >= _width) || (y >= _height)) {
        return;
    }

    if ((x + w - 1) >= _width) {
        w = _width - x;
    }
    if ((y + h - 1) >= _height) {
        h = _height - y;
    }

    startWrite();
    setAddrWindow(x, y, x + w - 1, y + h - 1);

    uint8_t hi = color >> 8;
    uint8_t lo = color;

    for (int32_t i = (int32_t)w * h; i > 0; i--) {
        SPI.transfer(hi);
        SPI.transfer(lo);
    }

    endWrite();
}

void GC9107::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
    if ((x >= _width) || (y >= _height)) {
        return;
    }

    if ((x + w - 1) >= _width) {
        w = _width - x;
    }

    fillRect(x, y, w, 1, color);
}

void GC9107::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
    if ((x >= _width) || (y >= _height)) {
        return;
    }

    if ((y + h - 1) >= _height) {
        h = _height - y;
    }

    fillRect(x, y, 1, h, color);
}

void GC9107::drawRGBBitmap(int16_t x, int16_t y, const uint16_t *bitmap, int16_t w, int16_t h) {
    if ((x >= _width) || (y >= _height)) {
        return;
    }

    int16_t actualW = w;
    int16_t actualH = h;

    if ((x + w - 1) >= _width) {
        actualW = _width - x;
    }
    if ((y + h - 1) >= _height) {
        actualH = _height - y;
    }

    startWrite();
    setAddrWindow(x, y, x + actualW - 1, y + actualH - 1);

    for (int16_t j = 0; j < actualH; j++) {
        for (int16_t i = 0; i < actualW; i++) {
            uint16_t color = bitmap[j * w + i];
            writeData16(color);
        }
    }

    endWrite();
}

void GC9107::writeCommand(uint8_t cmd) {
    digitalWrite(_dc, LOW);
    digitalWrite(_cs, LOW);
    SPI.transfer(cmd);
    digitalWrite(_cs, HIGH);
    digitalWrite(_dc, HIGH);
}

void GC9107::writeData(uint8_t data) {
    digitalWrite(_cs, LOW);
    SPI.transfer(data);
    digitalWrite(_cs, HIGH);
}

void GC9107::writeData16(uint16_t data) {
    digitalWrite(_cs, LOW);
    SPI.transfer(data >> 8);
    SPI.transfer(data);
    digitalWrite(_cs, HIGH);
}

void GC9107::startWrite() {
    SPI.beginTransaction(_spiSettings);
}

void GC9107::endWrite() {
    SPI.endTransaction();
}
