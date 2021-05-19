/*
 * Exercise 1: A push-button hardware interrupt 
 * 
 * Program that controls an LED with a push button and interrupts.
 */
 
int ledPin = 6; // address of external LED (connected to digital pin 6)
int buttonPin = 3; // address of pushbutton (connected to digital pin 3)
int buttonState = LOW;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(buttonPin), controlLED, CHANGE);
}

void loop() {
  delay(10);
}

void controlLED() {
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  if (buttonState == HIGH) {
    // turn LED on:
    digitalWrite(ledPin, HIGH);
  } else {
    // turn LED off:
    digitalWrite(ledPin, LOW);
  }
}
