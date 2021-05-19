/*
 * Activity 2: Multitasking using millis() 
 * 
 */
 
#include <loopTimer.h>
#include <Arduino.h>
#include <U8x8lib.h>
U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);

// U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // OLEDs without Reset of the Display

int ledPin = 6; // address of external LED (connected to digital pin 6)
int buzzerPin = 5; // address of buzzer (connected to digital pin 5)

unsigned long currentMillis;                   // stores the number of milliseconds passed since the start of program
unsigned long currentMillis_buzz;              // keeps track of how long buzzer has been turned on
unsigned long previousMillis_buzz;             // stores the start time of the buzzer being turned on
const long INTERVAL_BLINK = 250;               // LED blinks every 500ms
const long INTERVAL_BUZZ = 1000;               // buzzer turns on every 1s
const long BUZZ_TIME = 100;                    // buzzer stays on for 100ms
const long INTERVAL_OLED = 2000;               // print to OLED every 2s
unsigned long counter_blink = 0;
unsigned long counter_buzz = 0;
unsigned long counter_OLED = 0;
int counter_Hello = 0;
int blink_toggle = 1;

void setup() {
  Serial.begin(9600);
  //u8x8.setBusClock(100000);  // If you breakout other modules, please enable this line
  u8x8.begin(); // to start using the screen
  u8x8.setFlipMode(1);
  pinMode(ledPin, OUTPUT);   // initialize digital pin 6 as an output
  pinMode(buzzerPin, OUTPUT);   // intitialize digital pin 5 as an output
}

void loop() {
  loopTimer.check(Serial);
  currentMillis = millis();
  if (currentMillis >= counter_blink * INTERVAL_BLINK){
    toggleLED();
    counter_blink++;
    currentMillis = millis();
    if (currentMillis >= counter_buzz * INTERVAL_BUZZ){
      buzz();
      counter_buzz++;
      currentMillis = millis();
      if (currentMillis >= counter_OLED * INTERVAL_OLED){
        printOLED();
        counter_OLED++;
      }
    }
  }
}

void toggleLED(){
  Serial.println("Task 1 Running");
  if (blink_toggle == 1){
    digitalWrite(ledPin, HIGH);   // turn the LED on (HIGH is the voltage level)
    blink_toggle = 0;
  }else{
    digitalWrite(ledPin, LOW);    // turn the LED off by making the voltage LOW
    blink_toggle = 1;
  }
}

void buzz(){
  Serial.println("Task 2 Running");
  tone(buzzerPin, 500);     // turn the buzzer on
  previousMillis_buzz = millis();
  while (true){
    currentMillis_buzz = millis();
    if (currentMillis_buzz - previousMillis_buzz >= BUZZ_TIME){
      break;
    }
  }
  noTone(buzzerPin);        // turn the buzzer off by making the voltage LOW
}

void printOLED(){
  Serial.println("Task 3 Running");
  if (counter_Hello == 4){
    counter_Hello = 0;
    u8x8.clear();
  }
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.setCursor(0, counter_Hello);   //defining the starting point for the cursor
  u8x8.print("Hello World");
  counter_Hello++;
}
