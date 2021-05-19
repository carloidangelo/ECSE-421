/*
 * Exercise 5: Displaying displacement data to an OLED Screen
 */
 
#include <Arduino.h>
#include <U8x8lib.h>
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

U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);

// U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // OLEDs without Reset of the Display

const float RAWLOW_X = -0.978000;
const float RAWHIGH_X = 1.009000;
const float RAWLOW_Y = -0.981000;
const float RAWHIGH_Y = 0.971000;
const float RAWLOW_Z = -1.003000;
const float RAWHIGH_Z = 1.027000;
const float REFERENCELOW = -1.0; 
const float REFERENCEHIGH = 1.0;
const float GRAVITY = 9.81;

float x,y,z;
float correctedX, correctedY, correctedZ;
float velocityX = 0, velocityY = 0, velocityZ = 0;
float disX, disY, disZ;
float currentTime;
float previousTime = 0.0;
float previousAccelerationX = 0.0, previousAccelerationY = 0.0, previousAccelerationZ = 0.0;
float posX = 0, posY = 0, posZ = 0;
String str_posX, str_posY, str_posZ;

void setup(void) {
  //u8x8.setBusClock(100000);  // If you breakout other modules, please enable this line
  u8x8.begin(); // to start using the screen
  u8x8.setFlipMode(1);
  LIS.begin(Wire, 0x19); // initialize device by I2C

  // Setting full-scale range
  LIS.setFullScaleRange(LIS3DHTR_RANGE_2G); // 2G
  
  // Setting operating mode
  // High-resolution Mode 
  LIS.setHighSolution(true);

  // Setting output data rate
  LIS.setOutputDataRate(LIS3DHTR_DATARATE_50HZ); // 50HZ
}

void loop(void) {
  currentTime = millis();
  LIS.getAcceleration(&x, &y, &z); // getting acceleration values in each axis
  // Two Point Calibration
  correctedX = (((x-RAWLOW_X) * (REFERENCEHIGH - REFERENCELOW))/ (RAWHIGH_X - RAWLOW_X)) + REFERENCELOW;
  correctedY = (((y-RAWLOW_Y) * (REFERENCEHIGH - REFERENCELOW))/ (RAWHIGH_Y - RAWLOW_Y)) + REFERENCELOW;
  correctedZ = (((z-RAWLOW_Z) * (REFERENCEHIGH - REFERENCELOW))/ (RAWHIGH_Z - RAWLOW_Z)) + REFERENCELOW;
  // Displacement Calculation (in meters)
  disX = (velocityX*((currentTime - previousTime)/1000)) + ((0.5)*previousAccelerationX* pow(((currentTime - previousTime)/1000), 2));
  disY = (velocityY*((currentTime - previousTime)/1000)) + ((0.5)*previousAccelerationY* pow(((currentTime - previousTime)/1000), 2));
  disZ = (velocityZ*((currentTime - previousTime)/1000)) + ((0.5)*previousAccelerationZ* pow(((currentTime - previousTime)/1000), 2));
  // Initial velocity calculation (in m/s) for the next iteration of the displacement calculation
  velocityX = previousAccelerationX*((currentTime - previousTime)/1000) + velocityX;
  velocityY = previousAccelerationY*((currentTime - previousTime)/1000) + velocityY;
  velocityZ = previousAccelerationZ*((currentTime - previousTime)/1000) + velocityZ;
  // Position calculation (in meters)
  posX = posX += disX;
  posY = posY += disY;
  posZ = posZ += disZ;

  str_posX = String(posX, 2); 
  str_posY = String(posY, 2); 
  str_posZ = String(posZ, 2); 

  // print values to the OLED Screen
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.setCursor(0, 0); //defining the starting point for the cursor
  u8x8.print("x:" + str_posX + "m");
  u8x8.setCursor(0, 1); //defining the starting point for the cursor
  u8x8.print("y:" + str_posY + "m");
  u8x8.setCursor(0, 2); //defining the starting point for the cursor
  u8x8.print("z:" + str_posZ + "m");

  // Calculating the accelerations for the next iteration of the displacement calculation
  previousAccelerationX = correctedX * GRAVITY;
  previousAccelerationY = correctedY * GRAVITY;
  previousAccelerationZ = (correctedZ - 1) * GRAVITY; // doing the "correctedZ - 1" only works assuming the accelerometer doesn't turn upside down
  previousTime = currentTime;
  delay(10); // delay 10 milliseconds before the next reading
}
