
// Duty steps (~25/50/75/100%)
const uint8_t STEPS[4] = { 64, 128, 192, 255 };

// Coarse wait using Timer0 overflow flag 
static void wait_ms_coarse(uint16_t ms) {
  TIFR0 |= (1 << TOV0);
  while (ms--) {
    while ((TIFR0 & (1 << TOV0)) == 0) 
    TIFR0 |= (1 << TOV0);
  }
}

void setup() {

  DDRD |= (1 << PD6) | (1 << PD3); // Sets PD6 (OC0A), PD3 (OC2B)and PB3 (OC2A) as outputs.
  DDRB |= (1 << PB3);            

  DDRD &= ~(1 << PD5);   // Sets Button as input (use PD5 (T1) as external counter input)

  // Timer0: OC0A (PD6) 
  TCCR0A = (1 << COM0A1) | (1 << WGM01) | (1 << WGM00); // Turns on fast PWM mode and sets the output to be non-inverting
  TCCR0B = (1 << CS01) | (1 << CS00);   // Sets Prescaler to 64, which gives us a frequency of around 976Hz
  OCR0A  = 0;

  // Timer2: OC2A (PB3), OC2B (PD3) 
  TCCR2A = (1 << COM2A1) | (1 << COM2B1) | (1 << WGM21) | (1 << WGM20); // Turns on fast PWM mode and sets the output to be non-inverting
  TCCR2B = (1 << CS22);       // Sets Prescaler to 64, which gives us a frequency of around 976Hz
  OCR2A  = 0;
  OCR2B  = 0;

  // Timer1: used as external counter on T1 (PD5) 
  TCCR1A = 0;
  TCCR1B = (1 << CS12) | (1 << CS11) | (1 << CS10); // Sets external clock source (T1) on rising edge
  TCNT1  = 0;

  // Clears any pending Timer0 overflow flag (writing '1' resets it) to avoid false detection in wait_ms_coarse()
  TIFR0 |= (1 << TOV0);
}

void loop() {
  static uint8_t state = 0;  // Shows Current LED state; 0 = all OFF, 1–12 = brightness steps

  // Checks if the button registered a pulse. If any edges arrived since last check it is treated as one press
  if (TCNT1 != 0) {
    TCNT1 = 0;                            // Resets counter to consume bounce/pulse burst
    state = (state + 1) % 13;             // Moves on to next state, wraps to 0 after 12

    // debounce: waits for release of button
    wait_ms_coarse(50);
    while ((PIND & (1 << PD5)) == 0) 
    wait_ms_coarse(10);
  }

  // Determines PWM duty cycles based on current state
  uint8_t duty1 = 0, duty2 = 0, duty3 = 0;

  if (state == 0) {
    // all off
    duty1 = duty2 = duty3 = 0; // All LEDs are OFF
  } else {
    // states 1..12 → map to 0..11 for math
    uint8_t s = state - 1;
    uint8_t block    = s / 4;        // Determines LED group: 0 = LED1, 1 = LED2, 2 = LED3
    uint8_t step_idx = s % 4;        // Sets brightness step: 25/50/75/100%
    uint8_t duty     = STEPS[step_idx];

    if (block == 0) {          // Steps through LED1
      duty1 = duty;
    } else if (block == 1) {    // Steps through LED2; keeps LED1 at 100%
      duty1 = STEPS[3];
      duty2 = duty;
    } else {                // Steps through LED3; keeps LED1, LED2 at 100%
      duty1 = STEPS[3];
      duty2 = STEPS[3];
      duty3 = duty;
    }
  }

  // Writes PWM duties
  OCR0A = duty1;  // D6 (OC0A)  LED1
  OCR2A = duty2;  // D11 (OC2A) LED2
  OCR2B = duty3;  // D3 (OC2B)  LED3
}



