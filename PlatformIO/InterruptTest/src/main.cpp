#include <Arduino.h>
#include <RPi_Pico_TimerInterrupt.h>
#include <LUT.cpp>

#define FLASH_TIMER_FREQ    8877.841

RPI_PICO_Timer timer0(0);

// Timer ISR
bool flash_led_callback(struct repeating_timer *t) {
    // Add custom LED library calls here to change the LED data of the LED strips. 
    return true;
}

void setup() {
    Serial.begin(9600);
    pinMode(LED_BUILTIN, OUTPUT);

    timer0.attachInterrupt(FLASH_TIMER_FREQ, flash_led_callback);
}

void loop() {
    const uint8_t *p = &(LUT::flatimage[0]);
    Serial.println("B \t G \t R");
    // Print the color data for the first 10 LEDs
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 3; j++) {
            Serial.print(*(p+3*i+j));
            Serial.print("\t");
        }
        Serial.println("");
    }
    delay(1000);
}
