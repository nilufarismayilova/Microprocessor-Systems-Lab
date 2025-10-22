
// LED1: OC0A = PD6 (~30%)
// LED2: OC0B = PD5 (~50%)
// LED3: OC2A = PB3 (~80%)

void setup() {
 // Sets PD5 (OC0A), PD5 (OC0B) and  D11 (OC2A) ad outputs. These were taken from tables 13-9 and 13-3 in the datasheet
  DDRD |= (1 << PD6) | (1 << PD5);   
  DDRB |= (1 << PB3);      

// Timer0
  TCCR0A = (1 << COM0A1) | (1 << COM0B1) | (1 << WGM01) | (1 << WGM00); // Turns on Fast PWM mode for OC0A and OC0B, and sets the output to be non-inverting
  TCCR0B = (1 << CS01) | (1 << CS00);   // Sets prescaler to 64 to achieve ~976Hz

  
  OCR0A = 77;    // Determines duty cycle on PD6 with (≈ 0.30 * 255)
  OCR0B = 128;   // Determines duty cycle on PD5 with (≈ 0.50 * 255)

// Timer2
  TCCR2A = (1 << COM2A1) | (1 << WGM21) | (1 << WGM20); // Turns on Fast PWM mode for OC2A and sets the output to be non-inverting
  TCCR2B = (1 << CS22);      // Sets prescaler to 64 to achieve ~976Hz
  OCR2A = 204;  // Determines duty cycle on PB3 with (≈ 0.80 * 255)
}

void loop() {
  // Empty
}
