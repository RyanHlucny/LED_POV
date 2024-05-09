#include <Arduino.h>
#include "SPI_LED.h"
#include <SPI.h>

#define freq 1000000


SPI_LED ledStrip0(2, BGR, &SPI, SPISettings(freq, MSBFIRST, SPI_MODE0));
// SPI_LED ledStrip1(43, BGR, &SPI1, SPISettings(freq, MSBFIRST, SPI_MODE0));
uint8_t c1[2][3] = {{0, 0, 255},{0, 255, 0}};
uint8_t c2[2][3] = {{0, 255, 0},{0, 0, 255}};
uint8_t c3[2][3] = {{0, 0, 0},{0, 0, 0}};

void setup() {   
    SPI.setSCK(PIN_SPI0_SCK);
    SPI.setTX(PIN_SPI0_MOSI);
    SPI.setRX(PIN_SPI0_MISO);
    SPI.setCS(PIN_SPI0_SS);

    // SPI1.setSCK(PIN_SPI1_SCK);
    // SPI1.setTX(PIN_SPI1_MOSI);
    // SPI1.setRX(PIN_SPI1_MISO);
    // SPI1.setCS(PIN_SPI1_SS);

    ledStrip0.begin();
    // ledStrip1.begin();

    ledStrip0.clear();
    // ledStrip1.clear();

    pinMode(PIN_LED, OUTPUT);
}

void loop() {
//   ledStrip1.write();
    digitalWrite(PIN_LED, 1);
    // ledStrip0.setPixelColor(0,255,0,0);
    // ledStrip0.setPixelColor(1,0,255,0);
    ledStrip0.setStrip(*c1);
    ledStrip0.write();
    delay(500);
    digitalWrite(PIN_LED,0);
    // ledStrip0.setPixelColor(1,255,0,0);
    // ledStrip0.setPixelColor(0,0,255,0);
    ledStrip0.setStrip(*c2);
    ledStrip0.write();
    delay(500);
    ledStrip0.clear();
    ledStrip0.write();
    delay(500);
}
