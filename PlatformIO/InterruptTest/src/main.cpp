#include <Arduino.h>
#include <RPi_Pico_TimerInterrupt.h>
#include <LUT.cpp>
#include "SPI_LED.h"
#include <SPI.h>

#define freq 2000000
#define num_leds 53
 
SPI_LED ledStrip0(num_leds, RBG, &SPI, SPISettings(freq, MSBFIRST, SPI_MODE0));
SPI_LED ledStrip1(num_leds, RBG, &SPI1, SPISettings(freq, MSBFIRST, SPI_MODE0));

// #define FLASH_TIMER_FREQ    8877.841
#define FLASH_TIMER_FREQ_FAST    5644

RPI_PICO_Timer timer0(0);

// bool tog = 0;
uint8_t color_array0[num_leds*3];
uint8_t color_array1[num_leds*3];
const uint8_t *p_short = &(LUT::LUT_short[0]);
const uint8_t *p_long = &(LUT::LUT_long[0]);
int deg = 0;

// Timer ISR
bool flash_led_callback(struct repeating_timer *t) {
    // Add custom LED library calls here to change the LED data of the LED strips. 
    // tog = !tog;
    // digitalWriteFast(20u, tog);
    for (int i = 0; i < num_leds; i++) {
        for (int j = 0; j < 3; j++) {
            color_array0[i*3+j] = *(num_leds*3*deg + p_short + i*3 + j);
            color_array1[i*3+j] = *(num_leds*3*deg + p_long + i*3  + j);
            // Serial.print(*(p+3*i+j));
            // Serial.print("\t");
        }
        // Serial.println("");
    }
    ledStrip0.setStrip(color_array0, sizeof(color_array0));
    ledStrip1.setStrip(color_array1, sizeof(color_array1));
    ledStrip0.write();
    ledStrip1.write();
    deg = (deg + 1) % 333;
    return true;
}

void led_strip_setup() {
    // pinMode(PIN_SPI0_MOSI, OUTPUT);
    // pinMode(PIN_SPI0_SCK, OUTPUT);
    // pinMode(PIN_SPI0_SS, OUTPUT);
    // pinMode(PIN_SPI0_MISO, INPUT);

    // SPI.setSCK(PIN_SPI0_SCK);
    // SPI.setTX(PIN_SPI0_MOSI);
    // SPI.setRX(PIN_SPI0_MISO);
    // SPI.setCS(PIN_SPI0_SS);

    // digitalWrite(PIN_SPI0_SS, LOW);

    pinMode(PIN_SPI0_MOSI, OUTPUT);
    pinMode(PIN_SPI0_SCK, OUTPUT);
    pinMode(PIN_SPI0_SS, OUTPUT);
    pinMode(PIN_SPI0_MISO, INPUT);
    pinMode(PIN_SPI1_MOSI, OUTPUT);
    pinMode(PIN_SPI1_SCK, OUTPUT);
    pinMode(PIN_SPI1_SS, OUTPUT);
    pinMode(PIN_SPI1_MISO, INPUT);
    Serial.println("SPI Pin modes set");

    SPI.setSCK(PIN_SPI0_SCK);
    SPI.setTX(PIN_SPI0_MOSI);
    SPI.setRX(PIN_SPI0_MISO);
    SPI.setCS(PIN_SPI0_SS);
    SPI1.setSCK(PIN_SPI1_SCK);
    SPI1.setTX(PIN_SPI1_MOSI);
    SPI1.setRX(PIN_SPI1_MISO);
    SPI1.setCS(PIN_SPI1_SS);
    Serial.println("SPI Pins set");

    digitalWrite(PIN_SPI0_SS, LOW);
    digitalWrite(PIN_SPI1_SS, LOW);
    Serial.println("Set SPI CS pin");

    ledStrip0.begin();
    ledStrip1.begin();
    Serial.println("Beginning led strip");
    ledStrip0.clear();
    ledStrip1.clear();
    Serial.println("LED strip cleared");
    ledStrip0.write();
    ledStrip1.write();
    Serial.println("LED strip written with clear");
}

void setup() {
    Serial.begin(9600);
    Serial.println("Starting setup");
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.println("Setting up LED strips");
    led_strip_setup();
    // pinMode(20u, OUTPUT);
    Serial.println("LED strip setup complete");
    timer0.attachInterrupt(FLASH_TIMER_FREQ_FAST, flash_led_callback);
    Serial.println("Interrupt attached");
}

void loop() {
    // const uint8_t *p = &(LUT::flatimage[0]);
    // Serial.println("B \t G \t R");
    // // Print the color data for the first 10 LEDs
    // for (int i = 0; i < 10; i++) {
    //     for (int j = 0; j < 3; j++) {
    //         Serial.print(*(p+3*i+j));
    //         Serial.print("\t");
    //     }
    //     Serial.println("");
    // }
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("Setting LED HIGH");
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("Setting LED LOW");
    delay(500);

    // for (int i = 0; i<53; i++) {
    //     for (int j = 0; j<3; j++) {
    //         Serial.print(color_array0[i*3+j]);
    //         Serial.print("\t");
    //     }
    // }
    // Serial.println();
    // deg = (deg + 1) % 333;
    // delay(100);
}
