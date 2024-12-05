
#pragma once

#include <Arduino.h>
#include <SPI.h>

// Color order flags. This depends on LED chips specification in its datasheet. 
#define RGB (0 | (1 << 2) | (2 << 4))
#define RBG (0 | (2 << 2) | (1 << 4))
#define GRB (1 | (0 << 2) | (2 << 4))
#define GBR (2 | (0 << 2) | (1 << 4))
#define BRG (1 | (2 << 2) | (0 << 4))
#define BGR (2 | (1 << 2) | (0 << 4))

class SPI_LED
{
    public: 
        SPI_LED(uint16_t numLEDs, uint8_t pixelType = BRG, SPIClassRP2040* spi = &SPI, SPISettings spiSettings = SPISettings(1000000, MSBFIRST, SPI_MODE0));
        void begin();
        void setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b);
        void setStrip(uint8_t* newPixelArrayPtr, uint16_t size);
        void setStrip(uint8_t* newPixelArrayPtr, uint16_t size, uint16_t startPixel);
        void setPixelArrayPtr(uint8_t* newPixelArrayPtr);
        void write();
        void writeAsync(size_t size);
        void clear();

    private:
        SPIClassRP2040* spi = NULL;
        SPISettings spiSettings;
        uint16_t numLEDs;
        uint32_t freq;
        uint8_t* pixelArrayPtr;
        uint8_t redOffset;
        uint8_t greenOffset;
        uint8_t blueOffset;
};

