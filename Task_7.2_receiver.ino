#include <avr/io.h>
#include <util/delay.h>


void uart_init(void) {
 // From the datasheet: UBRR0 = FCPU / (16×BAUD) – 1, For 16 MHz and 9600 baud = 103 
    uint16_t ubrr = 103;
    UBRR0H = (uint8_t)(ubrr >> 8);   // Sets high byte of UBRR (page 149 datasheet)
    UBRR0L = (uint8_t)ubrr;   // Sets low byte of UBRR


    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // Sets frame format: 8 data bits
    UCSR0B = (1 << RXEN0); // Enables receiver 
}


uint8_t uart_receive(void) {
    while (!(UCSR0A & (1 << RXC0))); // Waits until a byte is received (RXC0 = 1)
    return UDR0;  // Reads received byte
}


void leds_init(void) {
    DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2);  // Sets PB0–PB2 as output pins
}


void leds_show(uint8_t v) {
    PORTB &= ~((1 << PB0) | (1 << PB1) | (1 << PB2)); // Turns off all LEDs first

    if (v == 1) { // If '1' was sent, turn on LED1 on PB0
        PORTB |= (1 << PB0);
    } else if (v == 2) { // If '2' was sent, turn on LED2 on PB1
        PORTB |= (1 << PB1);
    } else if (v == 3) { // If '3' was sent, turn on LED3 on PB2
        PORTB |= (1 << PB2);
    }
}


void eeprom_write(uint16_t addr, uint8_t data) {
    while (EECR & (1 << EEPE)); // Waits until previous write operation is complete

    EEAR = addr;     // Sets EEPROM address register
    EEDR = data;     // Loads data register

    EECR |= (1 << EEMPE);  // Master Write Enable (write allowed for 4 CPU cycles)
    EECR |= (1 << EEPE);   // Starts EEPROM write
}


uint8_t eeprom_read(uint16_t addr) {
    while (EECR & (1 << EEPE)); // Wait until previous write is complete

    EEAR = addr;              // Sets address
    EECR |= (1 << EERE);      // Triggers EEPROM read
    return EEDR;              // Returns data read from EEPROM
}


int main(void) {
    uart_init();  // Initialize UART 
    leds_init(); // Configures LED pins

    uint16_t index = 0;    // EEPROM write address
    uint8_t c;
    uint8_t ready_for_new_sequence = 1; // Reset index on the first element

    while (1) {
        c = uart_receive();     // Blocks until a byte arrives

        if (c == 1 || c == 2 || c == 3) { // If '1', '2' or '3' was sent

            if (ready_for_new_sequence == 1) {
                index = 0;     // Start new sequence at EEPROM[0]
                ready_for_new_sequence = 0;
            }

            eeprom_write(index, c);     // Writes received value to EEPROM
            index++;                    // Moves to next address
        }

        else if (c == 4) {
            for (uint16_t i = 0; i < index; i++) {
                uint8_t val = eeprom_read(i); // Reads EEPROM[i]
                leds_show(val);     // Displays LED pattern
                _delay_ms(500); // Small delay
            }
        }

        else if (c == 0) {
            index = 0;     // Resets index pointer
            ready_for_new_sequence = 1;    // Next 1–3 starts a new sequence
        }

        else {
            // Ignores unknown commands
        }
    }
}
