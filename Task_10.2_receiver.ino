#include <avr/io.h>

void uart_init(void) {
     // From the datasheet: UBRR0 = FCPU / (16×BAUD) – 1, For 16 MHz and 9600 baud = 103 
    uint16_t ubrr = 103;              
    UBRR0H = (uint8_t)(ubrr >> 8); // Sets high byte of UBRR (page 149 datasheet)
    UBRR0L = (uint8_t)ubrr; // Sets low byte of UBRR


    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // Sets frame format: 8 data bits
    UCSR0B = (1 << RXEN0); // Enables receiver 
}


uint8_t uart_receive(void) {
    while (!(UCSR0A & (1 << RXC0)));   // Waits until a byte is received (RXC0 = 1)
    return UDR0;                       // Reads received byte
}


void leds_init(void) {
    DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2);   // Sets PB0–PB2 as output pins
}

void leds_show(uint8_t val) {
    PORTB &= ~((1 << PB0) | (1 << PB1) | (1 << PB2)); // Turns off all LEDs first

    if (val == 1) PORTB |= (1 << PB0); // If '1' was sent, turn on LED1 on PB0
    else if (val == 2) PORTB |= (1 << PB1);  // If '2' was sent, turn on LED2 on PB1
    else if (val == 3) PORTB |= (1 << PB2); // If '3' was sent, turn on LED3 on PB2
}


int main(void) {
    uart_init(); // Sets up UART
    leds_init(); // Configures LED pins

    while (1) {
        uint8_t data = uart_receive(); // Blocks until a byte arrives
        leds_show(data);  // Shows 1–3 on LEDs
    }
}
