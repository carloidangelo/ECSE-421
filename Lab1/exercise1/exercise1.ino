/*
 * Exercise 1: Working with the LED
 * 
 * Program that turns an LED on for one second, then off for two seconds, repeatedly.
 */
 
int ledPin = 6; // address of external LED (connected to digital pin 6)

void setup() {
  // initialize digital pin 6 as an output
  pinMode(ledPin, OUTPUT);
}

void loop() {
  digitalWrite(ledPin, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for one second
  digitalWrite(ledPin, LOW);    // turn the LED off by making the voltage LOW
  delay(2000);                       // wait for two seconds
}
