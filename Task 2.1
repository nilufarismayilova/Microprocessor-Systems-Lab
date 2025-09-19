void setup() {
    DDRB |= (1 << DDB5);   // DDRB is the data direction register for port B. |= is the bitwise OR-assign operation. DDB5 is the bit index for pin PB5. Here, (1 << DDB5) shifts 1 left by 5 positions, producing 0b00100000. This sets bit 5 of DDRP to 1, which makes PB5 (onboard LED) an OUTPUT.
    DDRD &= ~(1 << DDD2);  // DDRD is the data direction register for port D. &= is the bitwise AND-assign operation. ~(1 << DDD2) shifts 1 left by 2 positions and inverts all bits, producing 0b11111011. DDRD &= ~(1 << DDD2) clears bit 2 of DDRD (sets it to 0), which makes PD2 (connected to the button) an INPUT.

  }

  void loop() {
    if (PIND & (1 << PIND2)) {       // PIND is the input register for port D, which shows the current logic levels on the pins. (1 << PIND2) creates 0b00000100. PIND & (1 << PIND2) masks out all bits except bit 2, since the AND operator only gives a nonzero result if both are nonzero. So, this line checks if the button is pressed: if PIND2 = HIGH, the result is nonzero (true), if PIND2 = LOW, the result is 0 (false).
      PORTB ^= (1 << PORTB5);        // PORTB is the output register for port B. (1 << PORTB5) is 0b00100000. the XOR-assign operator, ^=, turns the LED on or off each time the button is pressed: if PB5 was 0, it becomes 1, if it was 1, it becomes 0.
      delay(200);                    // delays the program by 200ms to avoid multiple fast triggers when the button is pressed (bouncing).
    }
  }
