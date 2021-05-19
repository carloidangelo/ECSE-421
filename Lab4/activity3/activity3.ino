/*
 * Activity 3: Multitasking using FreeRTOS 
 * 
 */
 
#include <loopTimer.h>
#include <Arduino_FreeRTOS.h>
#include <Arduino.h>
#include <U8x8lib.h>
U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);

// U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // OLEDs without Reset of the Display

// define the three tasks
void toggleLEDTask( void *pvParameters );
void buzzTask( void *pvParameters );
void printOLEDTask( void *pvParameters );                            

void setup() {
  Serial.begin(9600);

  xTaskCreate(
    toggleLEDTask
    ,  "Blink task"   // A name just for humans
    ,  80  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  0  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );
    
  xTaskCreate(
    buzzTask
    ,  "Buzzer task"  
    ,  45 
    ,  NULL
    ,  0  
    ,  NULL );

  xTaskCreate(
    printOLEDTask
    ,  "OLED task"
    ,  145
    ,  NULL
    ,  0  
    ,  NULL );
}

void loop() {
}

void toggleLEDTask(void *pvParameters){
  (void) pvParameters;
  uint8_t ledPin = 6; // address of external LED (connected to digital pin 6)
  pinMode(ledPin, OUTPUT);

  unsigned long currentMillis;
  const int INTERVAL_BLINK = 250;                 // LED blinks every 500ms
  unsigned long counter_blink = 0;
  uint8_t blink_toggle = 1;
  
  for (;;) // A Task shall never return or exit.
  {
    loopTimer.check(Serial);
    currentMillis = millis();
    if (currentMillis >= counter_blink * INTERVAL_BLINK){
      //Serial.println("Task 1 Running");
      if (blink_toggle == 1){
        digitalWrite(ledPin, HIGH);   // turn the LED on (HIGH is the voltage level)
        blink_toggle = 0;
      }else{
        digitalWrite(ledPin, LOW);    // turn the LED off by making the voltage LOW
        blink_toggle = 1;
      }
      counter_blink++;
    }
  }
}

void buzzTask(void *pvParameters){
  (void) pvParameters;
  uint8_t buzzerPin = 5; // address of buzzer (connected to digital pin 5)
  pinMode(buzzerPin, OUTPUT);   // intitialize digital pin 5 as an output

  unsigned long currentMillis;
  unsigned long currentMillis_buzz;             
  unsigned long previousMillis_buzz;  
  const int INTERVAL_BUZZ = 1000;                 // buzzer turns on every 1s
  const int BUZZ_TIME = 100;                      // buzzer stays on for 100ms  
  unsigned long counter_buzz = 0;
  for (;;) // A Task shall never return or exit.
  {
    currentMillis = millis();
    if (currentMillis >= counter_buzz * INTERVAL_BUZZ){
      //Serial.println("Task 2 Running");
      tone(buzzerPin, 500);     // turn the buzzer on
      previousMillis_buzz = millis();
      while (true){
        currentMillis_buzz = millis();
        if (currentMillis_buzz - previousMillis_buzz >= BUZZ_TIME){
          break;
        }
      }
      noTone(buzzerPin);        // turn the buzzer off by making the voltage LOW
      counter_buzz++;
    }
  }
}

void printOLEDTask(void *pvParameters){
  (void) pvParameters;
  
  //u8x8.setBusClock(100000);  // If you breakout other modules, please enable this line
  u8x8.begin(); // to start using the screen
  u8x8.setFlipMode(1);

  unsigned long currentMillis;
  const int INTERVAL_OLED = 2000;                 // print to OLED every 2s
  unsigned long counter_OLED = 0;
  uint8_t counter_Hello = 0;
  
  for (;;) // A Task shall never return or exit.
  {
    currentMillis = millis();
    if (currentMillis >= counter_OLED * INTERVAL_OLED){
      //Serial.println("Task 3 Running");
      if (counter_Hello == 4){
        counter_Hello = 0;
        u8x8.clear();
      }
      u8x8.setFont(u8x8_font_chroma48medium8_r);
      u8x8.setCursor(0, counter_Hello);   //defining the starting point for the cursor
      u8x8.print("Hello World");
      counter_Hello++;
      counter_OLED++;
    }
  }
}
