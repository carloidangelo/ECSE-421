/*
 * Exercise 3: Using the potentiometer 
 * 
 * Program that turns on the external LED only when the 
 * value of the potentiometer is higher than a threshold
 * 
 * Range of values of the potentiometer: [0,1023]
 */

int ledPin = 6; // address of external LED (connected to digital pin 6)
int potentiometerPin = A0; // address of rotary potentiomer (connected to analog pin 0)
int analogValue = 0;  // variable to hold the analog value coming from the potentiometer
int threshold = 500; 

void setup() {
  // Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
  pinMode(ledPin, OUTPUT); // initialize digital pin 6 as an output
  pinMode(potentiometerPin, INPUT); // intitialize analog pin 0 as an input
}

void loop() {
  analogValue = analogRead(potentiometerPin); // gets value of the potentiometer
  if (analogValue > threshold){
    digitalWrite(ledPin, HIGH);
  }else{
    digitalWrite(ledPin,LOW);
  }
  // Serial.println(analogValue); // prints real-time value of the potentiometer to the serial monitor
  // delay(10); // delay 10 milliseconds before the next reading
}
