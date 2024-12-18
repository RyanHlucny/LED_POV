#include <Arduino.h>
#include <math.h>
#include <Adafruit_ICM20948.h>
#include <Wire.h>

#define A 0.0776509570923569
#define B -0.287434475393028
#define C (M_PI_4 - A - B)

double Fast2ArcTan2(double x, double y) {
  double xy = x / y;
  double xy2 = xy * xy;
  return ((A*xy2 + B)*xy2 + C)* xy;
}

Adafruit_ICM20948 icm;
uint16_t measurement_delay_us = 65535;

void setup() {
  Serial.begin(115200);
  while (!Serial)
    delay(10);
  
  Serial.println("Adafruit ICM20948 test!");

  // Try to initialize sensor
  if (!icm.begin_I2C()) {
    while (1) {
      Serial.println("Failed to find ICM20948 chip");
      delay(1000);
    }
  }

  Serial.println("ICM20948 Found!");
  // icm.setAccelRange(ICM20948_ACCEL_RANGE_16_G);
  Serial.print("Accelerometer range set to: ");

  switch (icm.getAccelRange()) {
    case ICM20948_ACCEL_RANGE_2_G:
      Serial.println("+-2G");
      break;
    case ICM20948_ACCEL_RANGE_4_G:
      Serial.println("+-4G");
      break;
    case ICM20948_ACCEL_RANGE_8_G:
      Serial.println("+-8G");
      break;
    case ICM20948_ACCEL_RANGE_16_G:
      Serial.println("+-16G");
      break;
  }
  Serial.println("OK");

  // icm.setGyroRange(ICM20948_GYRO_RANGE_2000_DPS);
  Serial.print("Gyro range set to: ");
  switch (icm.getGyroRange()) {
    case ICM20948_GYRO_RANGE_250_DPS:
      Serial.println("250 degrees/s");
      break;
    case ICM20948_GYRO_RANGE_500_DPS:
      Serial.println("500 degrees/s");
      break;
    case ICM20948_GYRO_RANGE_1000_DPS:
      Serial.println("1000 degrees/s");
      break;
    case ICM20948_GYRO_RANGE_2000_DPS:
      Serial.println("2000 degrees/s");
      break;
  }

  //  icm.setAccelRateDivisor(4095);
  uint16_t accel_divisor = icm.getAccelRateDivisor();
  float accel_rate = 1125 / (1.0 + accel_divisor);

  Serial.print("Accelerometer data rate divisor set to: ");
  Serial.println(accel_divisor);
  Serial.print("Accelerometer data rate (Hz) is approximately: ");
  Serial.println(accel_rate);

  //  icm.setGyroRateDivisor(255);
  uint8_t gyro_divisor = icm.getGyroRateDivisor();
  float gyro_rate = 1100 / (1.0 + gyro_divisor);

  Serial.print("Gyro data rate divisor set to: ");
  Serial.println(gyro_divisor);
  Serial.print("Gyro data rate (Hz) is approximately: ");
  Serial.println(gyro_rate);

  // icm.setMagDataRate(AK09916_MAG_DATARATE_10_HZ);
  Serial.print("Magnetometer data rate set to: ");
  switch (icm.getMagDataRate()) {
    case AK09916_MAG_DATARATE_SHUTDOWN:
      Serial.println("Shutdown");
      break;
    case AK09916_MAG_DATARATE_SINGLE:
      Serial.println("Single/One shot");
      break;
    case AK09916_MAG_DATARATE_10_HZ:
      Serial.println("10 Hz");
      break;
    case AK09916_MAG_DATARATE_20_HZ:
      Serial.println("20 Hz");
      break;
    case AK09916_MAG_DATARATE_50_HZ:
      Serial.println("50 Hz");
      break;
    case AK09916_MAG_DATARATE_100_HZ:
      Serial.println("100 Hz");
      break;
  }
  Serial.println();
}
unsigned long N = 0;
unsigned long start_time;
unsigned long finish_time;
void loop() {
  start_time = micros();
  while(N < 1000) {
    N++;
    sensors_event_t accel;
    sensors_event_t gyro;
    sensors_event_t mag;
    sensors_event_t temp;
    icm.getEvent(&accel, &gyro, &temp, &mag);

    // Serial.print("\t\tTemperature ");
    // Serial.print(temp.temperature);
    // Serial.println(" deg C");

    /* Display the results (acceleration is measured in m/s^2) */
    // Serial.print("\t\tAccel X: ");
    // Serial.print(accel.acceleration.x);
    // Serial.print(" \tY: ");
    // Serial.print(accel.acceleration.y);
    float deg;
    int idx;
    deg = atan2(accel.acceleration.x, accel.acceleration.y) * 180 / (M_PI) + 180;
    Serial.print(" \tDeg: ");
    Serial.println(deg);  
    idx = deg * 333 / 360;
    Serial.print(" \tIdx: ");
    Serial.print(idx);
    
    // Serial.print(" \tZ: ");
    // Serial.print(accel.acceleration.z);
    // Serial.println(" m/s^2 ");

    // Serial.print("\t\tMag X: ");
    // Serial.print(mag.magnetic.x);
    // Serial.print(" \tY: ");
    // Serial.print(mag.magnetic.y);
    // Serial.print(" \tZ: ");
    // Serial.print(mag.magnetic.z);
    // Serial.println(" uT");

    /* Display the results (acceleration is measured in m/s^2) */
    // Serial.print("\t\tGyro X: ");
    // Serial.print(gyro.gyro.x);
    // Serial.print(" \tY: ");
    // Serial.print(gyro.gyro.y);
    // Serial.print(" \tZ: ");
    // Serial.print(gyro.gyro.z);
    // Serial.println(" radians/s ");
    // Serial.println();

    // delayMicroseconds(measurement_delay_us/10);
  }
  finish_time = micros();

  while(1) {
    Serial.print(N);
    Serial.print("Time to finish: ");
    Serial.println(finish_time - start_time);
    delay(1000);
  }
}
