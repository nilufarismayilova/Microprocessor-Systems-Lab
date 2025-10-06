void setup() {
  DDRB = 0b00100000; // Sets the LED (PB5) as an output
}

void loop() {
  PORTB = 0b00100000; // Turns the LED on by directly setting bit 5 of port B to 1
  delay(500);
  PORTB = 0b00000000; // Turns the LED off by rewriting the value of PORTB and setting bit 5 to 0
  delay(500);
}
