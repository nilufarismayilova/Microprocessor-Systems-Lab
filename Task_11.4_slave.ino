#include <avr/io.h>

void uart_init(void)
{
// From the datasheet: UBRR0 = FCPU / (16×BAUD) – 1, For 16 MHz and 9600 baud = 103 
uint16_t ubrr = 103; 
UBRR0H = (uint8_t)(ubrr >> 8); // Sets high byte of UBRR (page 149 datasheet)
UBRR0L = (uint8_t)ubrr; // Sets low byte of UBRR


    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // Sets frame format: 8 data bits
    UCSR0B = (1 << TXEN0);  // Enables transmitter (TX)
}

void uart_send(char c)
{
    while (!(UCSR0A & (1 << UDRE0))); // Waits until UART Data Register Empty (UDRE0 = 1)
    UDR0 = c;  // Loads character into the data register to transmit it
}

// Converts the 3 digits of a number into ASCII characters
void uart_send_number(uint8_t v)
{
    uart_send( (v/100) + '0' ); // Hundreds digit
    uart_send( ((v/10)%10) + '0' ); // Tens digit
    uart_send( (v%10) + '0' ); // Ones digit
    uart_send('\n'); // New Line
}

// -------- SPI SLAVE --------
void SPI_SlaveInit(void)
{
  // MISO (PB4) must be the output in slave mode
    DDRB |= (1 << PB4);  // MISO output
    SPCR = (1 << SPE);   // Enables SPI (SPE = 1) and MSTR = 0 by default (Slave mode)
}

// Receives one SPI byte from the master
uint8_t SPI_SlaveReceive(void)
{
    while (!(SPSR & (1 << SPIF))); // Waits until transfer is complete (SPIF= = 1)
    return SPDR; // Returns received byte from SPI Data Register
}

int main(void)
{
    SPI_SlaveInit(); // Configures SPI slave hardware
    uart_init(); // Prepares UART for serial printing

    while (1)
    {
        uint8_t v = SPI_SlaveReceive(); // Waits for master byte
        uart_send_number(v);    // prints 085, 170, 255, ...
    }
}
