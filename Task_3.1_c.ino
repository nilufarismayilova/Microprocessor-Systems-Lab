void setup() {
  DDRB = 0b00100000 ;   // Sets the LED (PB5) as an output
}

void loop() {
 // Toggles LED on and off with XOR
  PORTB ^= 0b00100000;  
  delay(500);
  PORTB ^= 0b00100000;  
  delay(500);

}
