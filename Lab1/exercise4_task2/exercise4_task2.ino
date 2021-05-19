/*
 * Exercise 4: Recording displacement data from an accelerometer
 * 
 * Task 2: Calibrate the accelerometer + record and save new accelerometer values to a .csv file
 */

#include "LIS3DHTR.h"
#ifdef SOFTWAREWIRE
    #include <SoftwareWire.h>
    SoftwareWire myWire(3, 2);
    LIS3DHTR<SoftwareWire> LIS;       //Software I2C
    #define WIRE myWire
#else
    #include <Wire.h>
    LIS3DHTR<TwoWire> LIS;           //Hardware I2C
    #define WIRE Wire
#endif

const float RAWLOW_X = -0.978000;
const float RAWHIGH_X = 1.009000;
const float RAWLOW_Y = -0.981000;
const float RAWHIGH_Y = 0.971000;
const float RAWLOW_Z = -1.003000;
const float RAWHIGH_Z = 1.027000;
const float REFERENCELOW = -1.0; 
const float REFERENCEHIGH = 1.0;

float x,y,z;
float correctedX, correctedY, correctedZ;
float currentTime;

void setup() {
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
  LIS.begin(Wire, 0x19); // initialize device by I2C

  // Setting full-scale range
  LIS.setFullScaleRange(LIS3DHTR_RANGE_2G); // 2G
  
  // Setting operating mode
  // High-resolution Mode 
  LIS.setHighSolution(true);

  // Setting output data rate
  LIS.setOutputDataRate(LIS3DHTR_DATARATE_50HZ); // 50HZ
}

void loop() {
  currentTime = millis();
  LIS.getAcceleration(&x, &y, &z); // getting acceleration values in each axis
  // Two Point Calibration
  correctedX = (((x-RAWLOW_X) * (REFERENCEHIGH - REFERENCELOW))/ (RAWHIGH_X - RAWLOW_X)) + REFERENCELOW;
  correctedY = (((y-RAWLOW_Y) * (REFERENCEHIGH - REFERENCELOW))/ (RAWHIGH_Y - RAWLOW_Y)) + REFERENCELOW;
  correctedZ = (((z-RAWLOW_Z) * (REFERENCEHIGH - REFERENCELOW))/ (RAWHIGH_Z - RAWLOW_Z)) + REFERENCELOW;
  // printing raw values of the acceleration in each axis and timestamps to the serial monitor
  // rounding off values to 6 decimal places
  Serial.print(currentTime); Serial.print(" "); // timestamp in milliseconds
  Serial.print(correctedX, 6); Serial.print(" ");
  Serial.print(correctedY, 6); Serial.print(" ");
  Serial.println(correctedZ, 6);
  delay(10); // delay 10 milliseconds before the next reading
}
