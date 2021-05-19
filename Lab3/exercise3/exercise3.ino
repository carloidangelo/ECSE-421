/*
 * Exercise 3: Generating a PWM signal
 * 
 */

int ledPin = 6;      // LED connected to digital pin 6
int analogPin = A0;  // potentiometer connected to analog pin 0
int val = 0;         // variable to store the read value

void setup() {
  pinMode(ledPin, OUTPUT);  // sets the pin as output
  pinMode(analogPin, INPUT);  // sets the pin as output
  
  cli();  //disable interrupts

  //set timer0 interrupt
  TCCR0A = 0;                // set entire TCCR0A register to 0
  TCCR0B = 0;                // same for TCCR0B
  
  TCNT0  = 0;                // initialize counter value to 0
  OCR0A = 255;               // set compare match register
  TIMSK0 |= (1 << OCIE0A);   // enable timer compare interrupt

  // turn on Fast PWM, 8-bit mode
  TCCR0A |= (1 << WGM00);    
  TCCR0A |= (1 << WGM01); 
  TCCR0A |= (1 << COM0A1);
  TCCR0B |= (1 << CS00);     // Set CS00 bit for prescaler of 1
  
  sei();  //enable interrupts
}

void loop() {
}

ISR(TIMER0_COMPA_vect){  //timer1 interrupt
  val = analogRead(analogPin);  // read the input pin
  OCR0A = val/4;
}
