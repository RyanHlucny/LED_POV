#include <Arduino.h>
#include <RPi_Pico_TimerInterrupt.h>
#include <LUT.cpp>
#include "SPI_LED.h"
#include <SPI.h>
#include <math.h>
#include <Adafruit_ICM20948.h>
#include <Wire.h>

#define VERBOSE false

#define SPI_FREQ 33000000
// 50 hz -> 1/50 (20 ms, or 20000 us)
#define IMU_MEASURE_PERIOD_US 20000
#define NUM_LEDS 53

#define FLASH_TIMER_FREQ    2000

// LUT Constants
#define NUM_ANGLE_DIVISIONS 333

// Creating LED Strip objects
SPISettings spi_settings(SPI_FREQ, MSBFIRST, SPI_MODE0);

// Creating ICM sensor
Adafruit_ICM20948 icm;

// Creating timer for running LED interrupts
RPI_PICO_Timer timer0(0);
RPI_PICO_Timer timer1(1);

uint8_t color_array0[NUM_LEDS*3];
uint8_t color_array1[NUM_LEDS*3];
const uint8_t* p_short = &(LUT::LUT_short[0]);
const uint8_t* p_long = &(LUT::LUT_long[0]);
uint16_t angle_idx = 0;

unsigned long prev_time = micros();
unsigned long curr_time = micros();
float prev_output = 0;
float curr_output = prev_output;
double angle_trim = 0;//M_PI / 2;

unsigned long now;
double m;
double sample;
double angle_mod;
unsigned long dt;

int dma_chan0;
int dma_chan1;

// timer0 ISR
bool flash_led_callback(struct repeating_timer *t) {
    static bool first_run = true;

    // Handle linear interpolation
    dt = curr_time - prev_time;
    if (dt > 0) {
        now = micros();
        m = (curr_output - prev_output) / (dt);
        sample = m*(now - curr_time) + curr_output;

        // calculate the angle_idx for the image here
        angle_mod = fmod(sample + angle_trim, M_TWOPI);
        if (angle_mod < 0) 
            angle_mod += M_TWOPI;

        angle_idx = (int) (angle_mod * NUM_ANGLE_DIVISIONS / M_TWOPI);
    }

    // Pointers for regular LUTs
    // uint8_t* ptr0 = (uint8_t*)(p_short + NUM_LEDS*3*(NUM_ANGLE_DIVISIONS-angle_idx-1));
    // uint8_t* ptr1 = (uint8_t*)(p_long + NUM_LEDS*3*(NUM_ANGLE_DIVISIONS-angle_idx-1));

    // Pointers for async LUTs
    uint8_t* ptr0 = (uint8_t*)(p_short + (NUM_LEDS*4 + 8)*(NUM_ANGLE_DIVISIONS-angle_idx-1));
    uint8_t* ptr1 = (uint8_t*)(p_long + (NUM_LEDS*4 + 8)*(NUM_ANGLE_DIVISIONS-angle_idx-1));

    if (first_run) {
        SPI.beginTransaction(spi_settings);
        SPI1.beginTransaction(spi_settings);
        first_run = false;
    }

    dma_channel_set_read_addr(dma_chan0, ptr0, true);
    dma_channel_set_read_addr(dma_chan1, ptr1, true);

    return true;
}

bool dma_setup() {
    // DMA channel 0 setup
    dma_chan0 = dma_claim_unused_channel(true);
    uint transfer_count = 220*2;//NUM_LEDS*4 + 8;
    if (dma_chan0 == -1)
        return false;

    hw_write_masked(&spi0_hw->cr0, (8-1) << SPI_SSPCR0_DSS_LSB, SPI_SSPCR0_DSS_BITS);

    dma_channel_config c = dma_channel_get_default_config(dma_chan0);
    channel_config_set_transfer_data_size(&c, DMA_SIZE_8);
    channel_config_set_read_increment(&c, true);
    channel_config_set_write_increment(&c, false);
    channel_config_set_dreq(&c, spi_get_dreq(spi0, true));
    // channel_config_set_chain_to(&c, dma_chan0);
    channel_config_set_irq_quiet(&c, true);
    
    dma_channel_configure(
        dma_chan0, 
        &c, 
        &spi0_hw->dr,   // Write address
        NULL,           // Read address
        transfer_count, // transfer count (220 bytes)
        false
    );

    spi0_hw->dmacr = 1 | (1 << 1);

    // DMA channel 1 setup
    dma_chan1 = dma_claim_unused_channel(true);
    if (dma_chan1 == -1)
        return false;

    hw_write_masked(&spi1_hw->cr0, (8-1) << SPI_SSPCR0_DSS_LSB, SPI_SSPCR0_DSS_BITS);

    c = dma_channel_get_default_config(dma_chan1);
    channel_config_set_transfer_data_size(&c, DMA_SIZE_8);
    channel_config_set_read_increment(&c, true);
    channel_config_set_write_increment(&c, false);
    channel_config_set_dreq(&c, spi_get_dreq(spi1, true));
    // channel_config_set_chain_to(&c, dma_chan1);
    channel_config_set_irq_quiet(&c, true);
    
    dma_channel_configure(
        dma_chan1, 
        &c, 
        &spi1_hw->dr,   // Write address
        NULL,         // Read address
        transfer_count, // transfer count (220 bytes)
        false
    );

    spi1_hw->dmacr = 1 | (1 << 1);

    return true;
}

bool led_strip_setup() {
    // Setting up pins 
    
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

    sleep_ms(10);

    SPI.begin();
    SPI1.begin();
    sleep_ms(20);

    return true;
}

void setup() {
    Serial.begin(9600);
        
    Serial.println("Starting setup");
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.println("Setting up DMA channels");
    if (!dma_setup()) {
        while (1) {
            Serial.println("Failed to setup DMA channels");
            delay(1000);
        }
    }

    Serial.println("Setting up LED strips");
    if (!led_strip_setup()) {
        while (1) {
            Serial.println("Failed to setup LED strips");
            delay(1000);
        }
    }
    delay(100);
    Serial.println("LED strip setup complete");
    if (!icm.begin_I2C()) {
        while (1) {
            Serial.println("Failed to find ICM20948 chip");
            delay(1000);
        }
    }
    delay(100);
    Serial.println("ICM20948 Found!");

    timer0.attachInterrupt(FLASH_TIMER_FREQ, flash_led_callback);
    delay(100);
    Serial.println("LED Interrupt attached");
}

sensors_event_t accel;
sensors_event_t gyro;
sensors_event_t mag;
sensors_event_t temp;

float prev_angle = 0;
// Filter Parameters
float a = 0.8;
float b = 1 - a;

unsigned long prev_imu_time = micros();

void loop() {

    // ICM code
    if (micros() - prev_imu_time > IMU_MEASURE_PERIOD_US) {

        icm.getEvent(&accel, &gyro, &temp, &mag);
        float angle = atan2(accel.acceleration.x, accel.acceleration.y);
        float dq = angle - prev_angle;

        // Unwrap sensor readings
        if (dq >= M_PI) {
            while (!(dq < M_PI)) {
                angle -= M_TWOPI;
                dq = angle - prev_angle;
            }
        }
        else if (dq <= - M_PI) {
            while (!(dq > -M_PI)) {
                angle += M_TWOPI;
                dq = angle - prev_angle;
            }
        }
        
        // Apply IIR low-pass filter
        prev_output = curr_output;
        curr_output = a * prev_output + b * angle;
        prev_time = curr_time;
        curr_time = micros();
        
        if (VERBOSE) {
            Serial.print("a_x: ");
            Serial.print(accel.acceleration.x);
            Serial.print(" \ta_y: ");
            Serial.print(accel.acceleration.y);
            Serial.print(" \tangle: ");
            Serial.print(angle);
            Serial.print(" \ty: ");
            Serial.print(curr_output);
        }

        prev_angle = angle;
        prev_imu_time = micros();
    }
}
