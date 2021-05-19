/*
 * Exercise 4: Recording displacement data from an accelerometer
 * 
 * Task 1: Determining the resolution for each full-scale setting of the 
 * accelerometer through empirical measurements
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

void setup() {
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
  LIS.begin(Wire, 0x19); // initialize device by I2C

   // Setting full-scale range
  LIS.setFullScaleRange(LIS3DHTR_RANGE_2G); // 2G
  //LIS.setFullScaleRange(LIS3DHTR_RANGE_4G); // 4G
  //LIS.setFullScaleRange(LIS3DHTR_RANGE_8G); // 8G
  //LIS.setFullScaleRange(LIS3DHTR_RANGE_16G); // 16G
  
  // Setting operating mode
  // Low-power Mode
  LIS.setPowerMode(POWER_MODE_LOW);
  LIS.setHighSolution(false);
  // Normal Mode
  //LIS.setPowerMode(POWER_MODE_NORMAL);
  //LIS.setHighSolution(false);
  // High-resolution Mode
  //LIS.setHighSolution(true);

  // Setting output data rate
  LIS.setOutputDataRate(LIS3DHTR_DATARATE_50HZ); // 50HZ
}

void loop() {
  // printing raw values of the acceleration in each axis to the serial monitor
  // rounding off values to 6 decimal places
  Serial.print("x:"); Serial.print(LIS.getAccelerationX(), 6); Serial.print("  ");
  Serial.print("y:"); Serial.print(LIS.getAccelerationY(), 6); Serial.print("  ");
  Serial.print("z:"); Serial.println(LIS.getAccelerationZ(), 6);
  delay(10); // delay 10 milliseconds before the next reading
}
