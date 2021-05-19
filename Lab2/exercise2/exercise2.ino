/*
 * Exercise 2: Train Crossing
 * 
 */
#include <Arduino.h>
#include <U8x8lib.h>
U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);

// U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // OLEDs without Reset of the Display

const int soundPin = A2;                // address of sound sensor (connected to analog pin 2)
const int lightPin = A3;                // address of light sensor (connected to analog pin 3)
int soundValue = 0;                     // variable to hold the analog value coming from the sound sensor
int lightValue = 0;                     // variable to hold the analog value coming from the light sensor
const int soundThreshold = 550;         // found this value by looking at the serial plotter
const int lightThreshold = 600;         // found this value by looking at the serial plotter

int current_state = 0;                   // keeps track of the current state

void setup(void) {
  //u8x8.setBusClock(100000);  // If you breakout other modules, please enable this line
  u8x8.begin(); // to start using the screen
  u8x8.setFlipMode(1);
  
  pinMode(soundPin, INPUT); // initialize digital pin 2 as an input
  pinMode(lightPin, INPUT); // intitialize analog pin 3 as an input

  // code used to find thresholds
  //Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
}

void loop(void) {
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  switch (current_state) {
  case 0: // wait state
    u8x8.clearLine(1);
    u8x8.setCursor(0, 0); 
    u8x8.print("state: wait");
    soundValue = analogRead(soundPin); // reads the input on analog pin A2 (value between 0 and 1023)
    lightValue = analogRead(lightPin); // reads the input on analog pin A3 (value between 0 and 1023)
    if (lightValue > lightThreshold){ // train is approaching
      u8x8.clearLine(1);
      u8x8.setCursor(0, 1);
      u8x8.print("output: lowerGate"); 
      current_state = 0;
    }else if (soundValue > soundThreshold){ // train is leaving
      u8x8.clearLine(1);
      u8x8.setCursor(0, 1);
      u8x8.print("output: raiseGate"); 
      current_state = 0;
    }
    break;
  default:
    break;
  }
  // code used to find thresholds
  //soundValue = analogRead(soundPin); // reads the input on analog pin A2 (value between 0 and 1023)
  //lightValue = analogRead(lightPin); // reads the input on analog pin A3 (value between 0 and 1023)
  //Serial.print("x:"); Serial.print(soundValue); Serial.print("  ");
  //Serial.print("y:"); Serial.println(lightValue);
  //delay(10); // delay 10 milliseconds before the next reading
}
