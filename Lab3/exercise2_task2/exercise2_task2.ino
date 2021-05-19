/*
 * Exercise 2: An LED blink with timer interrupts
 * 
 * Task: Using direct register accessing repeat the above task 
 * (reworking the blink example using timer1) without using pinMode() or digitalWrite() 
 */

int toggle = 1;

void setup() {
  DDRD = DDRD | B01000000;  // sets pin 6 as an output

  cli();  //disable interrupts

  //set timer1 interrupt at 62500Hz
  TCCR1A = 0;                // set entire TCCR1A register to 0
  TCCR1B = 0;                // same for TCCR1B
  
  TCNT1  = 0;                // initialize counter value to 0
  OCR1A = 31249;             // set compare match register
  TIMSK1 |= (1 << OCIE1A);   // enable timer compare interrupt
  
  TCCR1B |= (1 << WGM12);    // turn on CTC mode
  TCCR1B |= (1 << CS12);     // Set CS12 bit for 256 prescaler
  
  sei();  //enable interrupts
}

void loop() {
}

ISR(TIMER1_COMPA_vect){  //timer1 interrupt
  if (toggle){
    PORTD = B01000000; // sets digital pin 6 HIGH
    toggle = 0;
  }
  else{
    PORTD = B00000000; // sets digital pin 6 LOW
    toggle = 1;
  }
}
