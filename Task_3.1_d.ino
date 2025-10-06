void setup() {
  DDRB = 0b00100000 ;   // Sets the LED (PB5) as an output
}

  PORTB = (1 << 5);    // Turns LED on by shifting 1 left by 5 places (0b00100000) 
  delay(500);
  PORTB = (1 << 0);     // Turns the LED off by shifting 1 by 0 places (0b00000001), meaning bit 5 is 0.
  delay(500);
}
