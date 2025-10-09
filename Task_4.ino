
volatile uint8_t counter;  // Stores counter in SRAM. It resets to 0 every time the power turns off

void setup() {
  Serial.begin(9600);  


  while (EECR & (1 << EEPE));   // Waits if EEPROM is busy
  EEAR = 0;                     // Sets EEPROM address to 0
  EECR |= (1 << EERE);          // Triggers read operation from EEPROM
  counter = EEDR;               // Copies value from EEDR into counter

  DDRD &= ~(1 << PD2);   // Sets PD2 (button) as input 

  Serial.print("Starting counter from: ");
  Serial.println(counter);
}

void loop() {
  if (PIND & (1 << PD2)) {     // Checks if button is pressed
    delay(200);            


    asm volatile(
      "lds r24, counter    \n\t"   // Loads counter value from SRAM into register r24
      "inc r24             \n\t"   // Increments the value in r24 by 1
      "sts counter, r24    \n\t"   // Stores the updated value back into SRAM (counter)   
    );

    //lds = Loads from SRAM to register
    //sts = Stores from register to SRAM

   
    while (EECR & (1 << EEPE));   // Waits if EEPROM is busy
    EEAR = 0;                     // Sets EEPROM address to 0
    EEDR = counter;               // Places new counter value into EEDR
    EECR |= (1 << EEMPE);         // Enables master write
    EECR |= (1 << EEPE);          // Starts EEPROM write operation

    Serial.print("Counter: ");
    Serial.println(counter);

    while (PIND & (1 << PD2));    // Waits until the button is released
  }
}