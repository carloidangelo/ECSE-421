/*
 * Exercise 2: Using the pushbutton  
 * 
 * Program that blinks an LED when the pushbutton is pressed
 */

int ledPin = 6; // address of external LED (connected to digital pin 6)
int pushButtonPin = 4; // address of pushbutton (connected to digital pin 4)

void setup() {
  pinMode(ledPin, OUTPUT); // initialize digital pin 6 as an output
  pinMode(pushButtonPin, INPUT); // intitialize digitl pin 4 as an input
}

void loop() {
  if (digitalRead(pushButtonPin) == HIGH){
    while (true){
      digitalWrite(ledPin, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(500);                       // wait for half a second
      digitalWrite(ledPin, LOW);    // turn the LED off by making the voltage LOW
      delay(500);                       // wait for half a second
    }
  }
}
