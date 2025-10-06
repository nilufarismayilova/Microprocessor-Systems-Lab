void setup() {
  DDRB |= (1 << DDB5);   // Sets the LED (PB5) as an output
}

 PORTB |= 0b00100000;  // Turns the LED on by using OR-assign operation (0 OR 1 = 1 / 1 OR 1 = 1)
  delay(500);
  PORTB &= 0b11011111; // Turns the LED off (0 & 0 = 0 / 1 & 0 = 0)
  delay(500);
}