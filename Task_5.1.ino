
void setup() {
  // LED1: Timer1 (1Hz)
  DDRB |= (1 << PB1);    // Sets PB1 as output. Specified as OC1A in the data sheet
  TCCR1A = (1 << COM1A0);      // Toggles OC1A on compare match
  TCCR1B = (1 << WGM12) | (1 << CS12); // Turns on CTC mode and sets prescaler to 256
  OCR1A = 31249;                   // OCR1A value is dtermined by the formula given in the datasheet: (16 MHz / (2×256×1 Hz)) − 1

  // LED2: Timer0 (50Hz)
  DDRD |= (1 << PD6);       // Sets PD6 (OC0A)
  TCCR0A = (1 << COM0A0) | (1 << WGM01); // Toggles on compare and turns on CTC mode
  TCCR0B = (1 << CS02) | (1 << CS00);    // Sets prescaler to 1024
  OCR0A = 155;                     // (16MHz / (2x1024x50)) -1 
  // LED3: Timer2 (100Hz)
  DDRB |= (1 << PB3);        // Sets PB3 (OC2A)
  TCCR2A = (1 << COM2A0) | (1 << WGM21); // Toggles on compare and turns on CTC mode
  TCCR2B = (1 << CS22) | (1 << CS21) | (1 << CS20); // Sets prescaler to 1024
  OCR2A = 77;                      // (16MHz / (2x1024x100)) -1 
}

void loop() {
  // Empty
}

