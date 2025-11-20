#include <avr/io.h>
#include <util/delay.h>


void uart_init(void) {
    // From the datasheet: UBRR0 = FCPU / (16×BAUD) – 1, For 16 MHz and 9600 baud = 103 
    uint16_t ubrr = 103;
    UBRR0H = (uint8_t)(ubrr >> 8); // Sets high byte of UBRR (page 149 datasheet)
    UBRR0L = (uint8_t)ubrr; // Sets low byte of UBRR


    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // Sets frame format: 8 data bits
    UCSR0B = (1 << TXEN0) | (1 << RXEN0); // Enables transmitter and receiver 
}


uint8_t uart_receive(void) {
    while (!(UCSR0A & (1 << RXC0))); // Waits until a byte is received (RXC0 = 1)
    return UDR0; // Reads received byte
}


void uart_transmit(uint8_t data) {
    while (!(UCSR0A & (1 << UDRE0))); // Waits until the Data Register Empty flag (UDRE0) = 1
    UDR0 = data;                 // UDR = Data Buffer, write data shifts out via PD1 (TX)
}


int main(void) {
    uart_init(); // Initializes UART
    
    uint8_t c;
    uint8_t send_val;
    
    while (1) {
        c = uart_receive(); // Block until a character arrives
        
        if (c == '1') { // If character '1' was sent
            send_val = 1;
            uart_transmit(send_val); // Sends back numeric value 1 
        }
        else if (c == '2') { // If character '2' was sent
            send_val = 2;
            uart_transmit(send_val); // Sends back numeric value 2
        }
        else if (c == '3') { // If character '3' was sent
            send_val = 3;
            uart_transmit(send_val); // Sends back numeric value 3
        }
        else if (c == '4') { // If character '4' was sent
            send_val = 4;
            uart_transmit(send_val); // Sends back numeric value 4
        }
        else if (c == '0') { // If character '0' was sent
            send_val = 0;
            uart_transmit(send_val); // Sends back numeric value 0
        }
    }
}
