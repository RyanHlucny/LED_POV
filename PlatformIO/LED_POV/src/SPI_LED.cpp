#include "SPI_LED.h"

static const uint8_t K_0x00 = 0x00;
static const uint8_t K_0xFF = 0xFF;

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
SPI_LED::SPI_LED(uint16_t numLEDs, uint8_t pixelType, SPIClassRP2040* spi, SPISettings spiSettings) : 
    numLEDs(numLEDs), freq(freq), pixelArrayPtr(NULL), redOffset(pixelType & 3), 
    greenOffset((pixelType >> 2) & 3), blueOffset((pixelType >> 4) & 3),
    spi(spi), spiSettings(spiSettings) { 

        free(pixelArrayPtr);
        uint16_t bytes = numLEDs * 3;
        if ((pixelArrayPtr = (uint8_t*)malloc(bytes))) {
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
        pixelArrayPtr[n * 3 + redOffset] = r;
        pixelArrayPtr[n * 3 + greenOffset] = g;
        pixelArrayPtr[n * 3 + blueOffset] = b;
    }
}

/**
 * @brief Copies the values of a given array to the SPI_LED's pixelArrayPtr. 
 * Note that the color order (RGB, BRG, RBG, etc) in the array must match the pixelType this object was constructed with.
 * @param newPixelArrayPtr pointer to the first pixel of the new pixel array
 * @param size the length of the new pixel array in bytes
 */
void SPI_LED::setStrip(uint8_t* newPixelArrayPtr, uint16_t size) {
    std::copy(newPixelArrayPtr, newPixelArrayPtr + size, this->pixelArrayPtr);
}

/**
 * @brief Overload setStrip function which allows you to specify the starting pixel location to set only parts of a strip
 * @param newPixelArrayPtr pointer to the first pixel of the new pixel array
 * @param size the length of the new pixel array in bytes
 * @param startPixel the index of the first pixel to write (the first led index is 0)
 */
void SPI_LED::setStrip(uint8_t* newPixelArrayPtr, uint16_t size, uint16_t startPixel) {
    std::copy(newPixelArrayPtr, newPixelArrayPtr + size, this->pixelArrayPtr + startPixel * 3);
}

/**
 * @brief Sets the LED strip's pixel array pointer to new pixel array pointer in memory
 * @param newPixelArrayPtr pointer to the first index of the new pixel array
 */
void SPI_LED::setPixelArrayPtr(uint8_t* newPixelArrayPtr) {
    this->pixelArrayPtr = newPixelArrayPtr;
}

void SPI_LED::write() {
    if (!pixelArrayPtr)
        return;

    uint8_t *ptr = pixelArrayPtr;
    uint16_t n = numLEDs, i;

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

void SPI_LED::writeAsync(size_t size) {
    if (!pixelArrayPtr)
        return;

    uint8_t *ptr = pixelArrayPtr;
    uint16_t n = numLEDs, i;

    spi->beginTransaction(spiSettings);

    // *** Start Frame ***
    for (i = 0; i < 4; i++)
        spi->transferAsync(&K_0x00, nullptr, sizeof(K_0x00));
    
    // *** Pixel Data ***
    do {
        spi->transferAsync(&K_0xFF, nullptr, sizeof(K_0xFF)); // Pixel start (setting global brightness frame to full)
        // for (i = 0; i < 3; i++)
        //     spi->transferAsync(*ptr++, nullptr, size); // Color Data
    } while (--n);

    // *** End Frame *** 
    // The number of bytes in the end frame is determined by the length of the strip,
    // Since its function is to allow the strip colors to propogate through the rest of the strip 
    for (i = 0; i < ((numLEDs + 15) / 16); i++)
        spi->transferAsync(&K_0xFF, nullptr, sizeof(K_0xFF));

    spi->endTransaction();
}

/**
 * @brief Sets the whole strip to off.
 */
void SPI_LED::clear() {
    memset(pixelArrayPtr, 0, numLEDs * 3); // 3 bytes/pixel
}
