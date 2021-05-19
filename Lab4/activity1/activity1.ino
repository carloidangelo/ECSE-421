``/*
 * Activity 1: Task Generation
 * 
 */
 
#include <loopTimer.h>

int ledPin = 6; // address of external LED (connected to digital pin 6)
int buzzerPin = 5; // address of buzzer (connected to digital pin 5)

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);   // initialize digital pin 6 as an output
  pinMode(buzzerPin, OUTPUT);   // intitialize digital pin 5 as an output
}

void loop() {
  loopTimer.check(Serial);
  blinkLED();
  //buzz();
}

void blinkLED(){
  digitalWrite(ledPin, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for one second
  digitalWrite(ledPin, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for one seconds
}

void buzz(){
  tone(buzzerPin, 500);         // turn the buzzer on
  delay(100);                       // wait for one second
  noTone(buzzerPin);            // turn the buzzer off by making the voltage LOW
  delay(1000);                       // wait for one seconds
}
