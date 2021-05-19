/*
 * Exercise 1: Designing an alarm clock 
 * 
 */
 
#include <Arduino.h>
#include <U8x8lib.h>
U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);

// U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // OLEDs without Reset of the Display

const long INTERVAL_MIN = 60000;         // interval at which to update the minutes (milliseconds)
const int INTERVAL_SEC = 1000;           // interval at which to update the seconds (milliseconds)
const int INTERVAL_BLINK_ON = 100;       // amount of time OLED screen is ON during blinking (milliseconds)
const int INTERVAL_DOUBLE_BUTTON = 5000; // amount of time for a double button press to happen (milliseconds)
const int MAX_MIN_AND_SEC = 60;          // max number that can show up for min and sec is (MAX_MIN_AND_SEC - 1)
const int MAX_POTVALUE_CHANGE_MIN = 16;  // an increase of 16 in the potentiometer value is equivalent to an increase of 1 min

const int buzzerPin = 5;                 // address of buzzer (connected to digital pin 5)
const int pushButtonPin = 4;             // address of pushbutton (connected to digital pin 4)
const int potentiometerPin = A0;         // address of rotary potentiomer (connected to analog pin 0)
int analogValue = 0;                     // variable to hold the analog value coming from the potentiometer

int current_state = 0;                   // keeps track of the current state

// used to update digital clock
unsigned long currentMillis;             // stores number of milliseconds passed since the start of program
unsigned long previousMillis_min = 0;    // stores last time min was updated
unsigned long previousMillis_sec = 0;    // stores last time sec was updated
unsigned long count_min = 0;             // number of min that have passed in total
unsigned long count_sec = 0;             // number of sec that have passed in total
int elapsed_min, elapsed_sec;       
String current_min, current_sec;

// used to prevent chattering and satisfy guards related to time
unsigned long currentMillisG;             
unsigned long previousMillisG;
unsigned long currentMillisB;             
unsigned long previousMillisB;   
unsigned long currentMillisD;             
unsigned long previousMillisD;       

int alarm_min = 59, alarm_sec = 59;       // initial value of the alarm (59min:59sec) represents the max alarm
                                          // we can have in this implementation
String alarm_min_str, alarm_sec_str;

int buttonCount = 0;                     // number of button presses within 5 sec

void setup(void) {
  //u8x8.setBusClock(100000);  // If you breakout other modules, please enable this line
  u8x8.begin(); // to start using the screen
  u8x8.setFlipMode(1);
  pinMode(pushButtonPin, INPUT); // intitialize digital pin 4 as an input
  pinMode(potentiometerPin, INPUT); // intitialize analog pin 0 as an input
  pinMode(buzzerPin, OUTPUT);   // intitialize digital pin 5 as an output
}

void loop(void) {
  switch (current_state) {
    case 0: // normal state
      previousMillisG = millis();
      while(true){
        displayTime();
        currentMillisG = millis();
        if (digitalRead(pushButtonPin) == HIGH && (currentMillisG - previousMillisG >= INTERVAL_SEC)){
          current_state = 1;
          break;
        }else if ((count_min % MAX_MIN_AND_SEC) >= alarm_min && (count_sec % MAX_MIN_AND_SEC) >= alarm_sec){
          tone(buzzerPin, 500);
          current_state = 2;
          break;
        }
      }
      break;
    case 1: // programming state
      previousMillisG = millis();
      while(true){
        analogValue = analogRead(potentiometerPin); // gets value of the potentiometer
        updateAlarm(analogValue);
        currentMillisG = millis();
        if (digitalRead(pushButtonPin) == HIGH && (currentMillisG - previousMillisG >= INTERVAL_SEC)){
          current_state = 0;
          break;
        }
      }
      break;
    case 2: // alarm state
      previousMillisD = millis();
      while (true){
        previousMillisG = millis();
        while(true){
          previousMillisB = millis();
          while (true){
            displayTime();
            currentMillisB = millis();
            // blink OLED
            if (currentMillisB - previousMillisB >= INTERVAL_BLINK_ON){
              u8x8.clear();
              break;
            }
          }
          currentMillisG = millis();
          if (digitalRead(pushButtonPin) == HIGH && (currentMillisG - previousMillisG >= INTERVAL_SEC/2)){
            buttonCount++;
            previousMillisD = millis();
            break;
          }else if(buttonCount == 1 && currentMillisG - previousMillisD >= INTERVAL_DOUBLE_BUTTON){
            break;
          }else if (currentMillisG - previousMillisD >= 5*INTERVAL_MIN){
            break;
          }
        }
        currentMillisD = millis();
        // when button is pressed twice within 5 seconds
        if (buttonCount == 2){
          current_state = 0;
          noTone(buzzerPin);
          resetAlarm();
          buttonCount = 0;
          break;
        // when button is pressed once within 5 seconds
        }else if (buttonCount == 1 && (currentMillisD - previousMillisD >= INTERVAL_DOUBLE_BUTTON)){
          current_state = 3;
          noTone(buzzerPin);
          buttonCount = 0;
          break;
        // when button is pressed zero times and 5min go by
        }else if ((currentMillisD - previousMillisD) >= 5*INTERVAL_MIN){
          current_state = 0;
          resetAlarm();
          break;
        }
      }
      break;
    case 3: // snooze state
      previousMillisD = millis();
      while (true){
        previousMillisG = millis();
        while(true){
          previousMillisB = millis();
          while (true){
            displayTime();
            currentMillisB = millis();
            // blink OLED
            if (currentMillisB - previousMillisB >= INTERVAL_BLINK_ON){
              u8x8.clear();
              break;
            }
          }
          currentMillisG = millis();
          if (digitalRead(pushButtonPin) == HIGH && (currentMillisG - previousMillisG >= INTERVAL_SEC/2)){
            buttonCount++;
            previousMillisD = millis();
            break;
          }else if(buttonCount == 1 && currentMillisG - previousMillisD >= INTERVAL_DOUBLE_BUTTON){
            break;
          }else if (currentMillisG - previousMillisD >= INTERVAL_MIN){
            break;
          }
        }
        currentMillisD = millis();
        // when button is pressed twice within 5 seconds
        if (buttonCount == 2){
          current_state = 0;
          resetAlarm();
          buttonCount = 0;
          break;
        // when button is pressed once within 5 seconds
        }else if (buttonCount == 1 && (currentMillisD - previousMillisD >= INTERVAL_DOUBLE_BUTTON)){
          current_state = 3;
          buttonCount = 0;
          break;
        // when button is pressed zero times and 1min goes by
        }else if (currentMillisD - previousMillisD >= INTERVAL_MIN){
          tone(buzzerPin, 500);
          current_state = 2;
          break;
        }
      }
      break;
    default:
      break;
  }
}

void displayTime(void){
  currentMillis = millis();
  if (currentMillis - previousMillis_sec >= INTERVAL_SEC){
    elapsed_sec = (currentMillis - previousMillis_sec)/INTERVAL_SEC;
    count_sec += elapsed_sec;
    previousMillis_sec = currentMillis;
    if (currentMillis - previousMillis_min >= INTERVAL_MIN){
      elapsed_min = (currentMillis - previousMillis_min)/INTERVAL_MIN;
      count_min += elapsed_min;
      previousMillis_min = currentMillis;
    }
  }
  current_min = String(count_min % MAX_MIN_AND_SEC);
  current_sec = String(count_sec % MAX_MIN_AND_SEC);

  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.setCursor(0, 0);
  if (current_min.toInt() < 10){
    u8x8.print('0' + current_min + ':');
  }else{
    u8x8.print(current_min + ':');
  }
  u8x8.setCursor(3, 0);
  if (current_sec.toInt() < 10){
    u8x8.print('0' + current_sec);
  }else{
    u8x8.print(current_sec);  
  }
}

void updateAlarm(int potValue){  
  // extract min value from potentiometer value
  alarm_min = potValue/MAX_POTVALUE_CHANGE_MIN;
  if (alarm_min >= 60){
    alarm_min = 59;
  }
  
  // extract sec value from potentiometer value
  alarm_sec = round((potValue % MAX_POTVALUE_CHANGE_MIN) * (MAX_MIN_AND_SEC/float(MAX_POTVALUE_CHANGE_MIN)));

  alarm_min_str = String(alarm_min);
  alarm_sec_str = String(alarm_sec);

  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.setCursor(0, 0);
  if (alarm_min_str.toInt() < 10){
    u8x8.print('0' + alarm_min_str + ':');
  }else{
    u8x8.print(alarm_min_str + ':');
  }
  u8x8.setCursor(3, 0);
  if (alarm_sec_str.toInt() < 10){
    u8x8.print('0' + alarm_sec_str);
  }else{
    u8x8.print(alarm_sec_str);  
  }
}

void resetAlarm(void){
  alarm_min = 59;
  alarm_sec = 59;
}
