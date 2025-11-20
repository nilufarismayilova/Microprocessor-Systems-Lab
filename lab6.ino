
#include <avr/io.h>       
#include <avr/interrupt.h> 
#include <util/delay.h>    

volatile uint8_t digit = 0;   // Current digit 0–9
volatile uint8_t paused = 0;  // Pause state

// g f e d c b a  → Mapped to pins: PB1 PB0 PD7 PD6 PD5 PD4 PD3
const uint8_t patterns[10] = {
  0b00111111, // 0 → a b c d e f
  0b00000110, // 1 → b c
  0b01011011, // 2 → a b d e g
  0b01001111, // 3 → a b c d g
  0b01100110, // 4 → b c f g
  0b01101101, // 5 → a c d f g
  0b01111101, // 6 → a c d e f g
  0b00000111, // 7 → a b c
  0b01111111, // 8 → a b c d e f g
  0b01101111  // 9 → a b c d f g
};
      
void displayDigit(uint8_t n) {
    uint8_t val = patterns[n];

    // Clear previous segment bits, then set new bits
    PORTD = (PORTD & 0b00000111) | ((val & 0b00011111) << 3); // PD3–PD7 (a-e)
    PORTB = (PORTB & 0b11111100) | ((val & 0b01100000) >> 5); // PB0–PB1 (f,g)
}

// Timer1 Compare March A Interrupt (every 0.5 seconds)
ISR(TIMER1_COMPA_vect) {
    if (!paused) {
        digit = (digit + 1) % 10; // Counts 0 to 9
        displayDigit(digit);
    }
    PINB |= (1 << PB5); // Toggles debug LED 
}

// External Interrupt INT0: Triggered on falling edge (button pressed)
ISR(INT0_vect) {
    paused = !paused;
    PINB |= (1 << PB5); // Debugs toggle LED
}

void setup() {

    DDRD |= (1 << PD3) | (1 << PD4) | (1 << PD5) | (1 << PD6) | (1 << PD7); // Sets Segments a–e as output
    DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB5);                           // Sets Segments f,g and LED as output

    DDRD &= ~(1 << PD2);   // Set PD2 (Button) as input
    PORTD |= (1 << PD2);   // Enables internal pull-up 

    EICRA |= (1 << ISC01); // Enabls INT0 (external interrupt) on falling edge
    EIMSK |= (1 << INT0);  // Enables INT0 interrupt

    TCCR1A = 0;      
    TCCR1B = 0;
    TCCR1B |= (1 << WGM12); // CTC mode for Timer 1
    TCCR1B |= (1 << CS12); // Sets prescaler to 256

    OCR1A = 31249; // Formula: OCR1A = (16,000,000 / (2 * 256)) - 1 = 31249  → gives 0.5s toggle

    TIMSK1 |= (1 << OCIE1A); // Enables Output Compare Match A Interrupt 

    sei(); // Enables Global interrupt 

    displayDigit(0);  // Show 0 immediately at startup
}

void loop() {
    // Empty
}
