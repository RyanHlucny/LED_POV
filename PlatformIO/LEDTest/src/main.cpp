#include <Arduino.h>
#include "SPI_LED.h"
#include <SPI.h>

#define freq 1000000


SPI_LED ledStrip0(53, RBG, &SPI, SPISettings(freq, MSBFIRST, SPI_MODE0));
// SPI_LED ledStrip1(3, BGR, &SPI1, SPISettings(freq, MSBFIRST, SPI_MODE0));
uint8_t c1[3][3] = {{0, 0, 255},{0, 255, 0},{255, 0, 0}};
uint8_t c2[3][3] = {{0, 255, 0},{0, 0, 255},{0, 255, 0}};
uint8_t c4[2][3] = {{255, 0, 0}, {0, 0, 255}};
int ball_id = 0;
int dir = 1;

void setup() {   
    // Serial.begin(9600);
    pinMode(PIN_SPI0_MOSI, OUTPUT);
    pinMode(PIN_SPI0_SCK, OUTPUT);
    pinMode(PIN_SPI0_SS, OUTPUT);
    pinMode(PIN_SPI0_MISO, INPUT);

    SPI.setSCK(PIN_SPI0_SCK);
    SPI.setTX(PIN_SPI0_MOSI);
    SPI.setRX(PIN_SPI0_MISO);
    SPI.setCS(PIN_SPI0_SS);

    digitalWrite(PIN_SPI0_SS, LOW);

    // SPI.setSCK(6u);
    // SPI.setTX(7u);
    // SPI.setRX(8u);
    // SPI.setCS(9u);

    // SPI1.setSCK(PIN_SPI1_SCK);
    // SPI1.setTX(PIN_SPI1_MOSI);
    // SPI1.setRX(PIN_SPI1_MISO);
    // SPI1.setCS(PIN_SPI1_SS);

    ledStrip0.begin();
    ledStrip0.clear();
    ledStrip0.write();

    // ledStrip1.begin();
    // ledStrip1.clear();
    // ledStrip1.write();

    pinMode(PIN_LED, OUTPUT);
}

void loop() {
    // // Serial.println("LED On");
    // digitalWrite(PIN_LED, 1);
    // // ledStrip0.setStrip(&c1[0][0], sizeof(c1));
    // for(int i = 0; i < 53; i++) {
    //     ledStrip0.setPixelColor(i,255,0,0);
    // }
    // ledStrip0.write();
    // // ledStrip1.setStrip(&c1[0][0], sizeof(c1));
    // // ledStrip1.write();
    // delay(500);
    // // Serial.println("LED Off");
    // digitalWrite(PIN_LED,0);
    // // ledStrip0.setStrip(&c2[0][0], sizeof(c2));
    // for(int i = 0; i < 53; i++) {
    //     ledStrip0.setPixelColor(i,0,255,0);
    // }
    // ledStrip0.write();
    // // ledStrip1.setStrip(&c2[0][0], sizeof(c2));
    // // ledStrip1.write();
    // delay(500);
    // // Serial.println("test 1");
    // // ledStrip0.setStrip(&c4[0][0], sizeof(c4),0);
    // for(int i = 0; i < 53; i++) {
    //     ledStrip0.setPixelColor(i,0,0,255);
    // }
    // ledStrip0.write();
    // // ledStrip1.setStrip(&c4[0][0], sizeof(c4),0);
    // // ledStrip1.write();
    // delay(500);
    // // Serial.println("test 2");
    // // ledStrip0.setStrip(&c4[0][0], sizeof(c4),1);
    // for(int i = 0; i < 53; i++) {
    //     ledStrip0.setPixelColor(i,255,255,255);
    // }
    // ledStrip0.write();
    // // ledStrip1.setStrip(&c4[0][0], sizeof(c4),1);
    // // ledStrip1.write();
    // delay(500);
    // // Serial.println("clear");
    // ledStrip0.clear();
    // ledStrip0.write();
    // // ledStrip1.clear();
    // // ledStrip1.write();
    // delay(500);

    for (int i = 0; i < 53; i++) {
        ledStrip0.setPixelColor(i, 63, 63, 0);
    }
    ledStrip0.setPixelColor(ball_id, 0, 255, 0);
    ledStrip0.write();
    
    if (dir == 1) {
        ball_id++;
    }
    if (dir == -1) {
        ball_id--;
    }
    if (ball_id == 52) {
        dir = -1;
    }
    if (ball_id == 0) {
        dir = 1;
    }

    delay(20);
    

    // digitalWrite(PIN_LED, 1);
    // ledStrip0.setPixelColor(0,255,0,0);
    // ledStrip0.write();
    // delay(500);
    // digitalWrite(PIN_LED, 0);
    // ledStrip0.setPixelColor(0,0,0,0);
    // ledStrip0.write();
    // delay(500);
}
