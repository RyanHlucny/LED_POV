#include "SPI_LED.h"
#include <iostream>

/**
 * The SPI interface passed in must have the MOSI, MISO, clokc, and data lines set prior to construction of this object.
 * @param
*/

/**
 * @brief The SPI interface passed in must have the MOSI, MISO, clokc, and data lines set prior to construction of this object.
 * @param numLEDs the number of LEDs in the strip
 * @param freq the SPI clock frequency to be used
 * @param pixelType pixel color ordering
 * @param spi pointer to the hardware SPI class. (Must have ports defined prior to creation of this object)
 */
SPI_LED::SPI_LED(uint16_t numLEDs, uint8_t pixelType, SPIClass* spi, SPISettings spiSettings) : 
    numLEDs(numLEDs), freq(freq), pixelArray(NULL), redOffset(pixelType & 3), 
    greenOffset((pixelType >> 2) & 3), blueOffset((pixelType >> 4) & 3),
    spi(spi), spiSettings(spiSettings) { 

        free(pixelArray);
        uint16_t bytes = numLEDs * 3;
        if ((pixelArray = (uint8_t*)malloc(bytes))) {
            this->numLEDs = numLEDs;
            clear();
        } else {
            this->numLEDs = 0;
        }

    }

/**
 * @brief Begins the SPI interface passed in at the creation of this object
 */
void SPI_LED::begin() { spi -> begin(); }

/**
 * @brief Sets the color of a given pixel in the color array
 * @param n the pixel index (0 is the first led)
 * @param r Red value [0-255]
 * @param g Green value [0-255]
 * @param b Blue value [0-255]
 */
void SPI_LED::setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b) {
    if (n < numLEDs) {
        pixelArray[n * 3 + redOffset] = r;
        pixelArray[n * 3 + greenOffset] = g;
        pixelArray[n * 3 + blueOffset] = b;
    }
}

/**
 * @brief Sets the led pixelArray to a given color array composed of numLEDs*3 bytes. 
 * Note that the color order (RGB, BRG, RBG, etc) in the array must match the pixelType this object was constructed with.
 * @param pixelArray pointer to the pixel color array
 */
void SPI_LED::setStrip(uint8_t* pixelArray) {
    this->pixelArray = pixelArray;
}

void SPI_LED::write() {
    if (!pixelArray)
        return;

    uint8_t *ptr = pixelArray, i;
    uint16_t n = numLEDs;

    spi->beginTransaction(spiSettings);

    // *** Start Frame ***
    for (i = 0; i < 4; i++)
        spi->transfer(0x00);
    
    // *** Pixel Data ***
    do {
        spi->transfer(0xFF); // Pixel start (setting global brightness frame to full)
        for (i = 0; i < 3; i++)
            spi->transfer(*ptr++); // Color Data
    } while (--n);

    // *** End Frame *** 
    // The number of bytes in the end frame is determined by the length of the strip,
    // Since its function is to allow the strip colors to propogate through the rest of the strip 
    for (i = 0; i < ((numLEDs + 15) / 16); i++)
        spi->transfer(0xFF);

    spi->endTransaction();
}

/**
 * @brief Sets the whole strip to off.
 */
void SPI_LED::clear() {
    // memset(pixelArray, 0, numLEDs * 3); // 3 bytes/pixel
    uint8_t zeros[numLEDs][3] = {{0}}; 
    cout << zeros;
    setStrip(*zeros);
}

