#include <Arduino.h>
#include "SPI_LED.h"
#include <SPI.h>

#define freq 1000000


SPI_LED ledStrip0(3, BGR, &SPI, SPISettings(freq, MSBFIRST, SPI_MODE0));
// SPI_LED ledStrip1(43, BGR, &SPI1, SPISettings(freq, MSBFIRST, SPI_MODE0));
uint8_t c1[3][3] = {{0, 0, 255},{0, 255, 0},{255, 0, 0}};
uint8_t c2[3][3] = {{0, 255, 0},{0, 0, 255},{0, 255, 0}};
uint8_t c4[2][3] = {{255, 0, 0}, {0, 0, 255}};

void setup() {   
    SPI.setSCK(PIN_SPI0_SCK);
    SPI.setTX(PIN_SPI0_MOSI);
    SPI.setRX(PIN_SPI0_MISO);
    SPI.setCS(PIN_SPI0_SS);

    ledStrip0.begin();
    ledStrip0.clear();
    ledStrip0.write();

    pinMode(PIN_LED, OUTPUT);
}

void loop() {

    digitalWrite(PIN_LED, 1);
    ledStrip0.setStrip(&c1[0][0], sizeof(c1));
    ledStrip0.write();
    delay(500);
    digitalWrite(PIN_LED,0);
    ledStrip0.setStrip(&c2[0][0], sizeof(c2));
    ledStrip0.write();
    delay(500);
    ledStrip0.setStrip(&c4[0][0], sizeof(c4),0);
    ledStrip0.write();
    delay(500);
    ledStrip0.setStrip(&c4[0][0], sizeof(c4),1);
    ledStrip0.write();
    delay(500);
    ledStrip0.clear();
    ledStrip0.write();
    delay(500);
}
