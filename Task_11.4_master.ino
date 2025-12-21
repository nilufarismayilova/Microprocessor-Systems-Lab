#define F_CPU 16000000UL // Defines CPU frequency for _delay_ms()

#include <avr/io.h>
#include <util/delay.h>

void SPI_MasterInit(void)
{
    
    DDRB |= (1 << PB2) | (1 << PB3) | (1 << PB5); // Sets PB2 (SS), PB3 (MOSI), PB5 (SCK) as output
    DDRB &= ~(1 << PB4);  // Sets PB4 (MISO) as input (Master must read from slave on MISO even if it is not used)

    PORTB |= (1 << PB2); // Sets SS high initially: slave is not selected

    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0); // Configures SPI Control Register (SPCR): SPE  = 1 (enable SPI); MSTR = 1 (master mode); CPOL = 0, CPHA = 0 (SPI mode 0); SPR0 = 1, SPR1 = 0, SPI2X = 0 (SCK = F_CPU/16) 
}

void SPI_MasterTransmit(uint8_t data)
{
    PORTB &= ~(1 << PB2); // Pulls SS low, which activates the slave

    SPDR = data; // Loads byte into SPI Data Register and begins transmission

    while (!(SPSR & (1 << SPIF))) // Waits until transfer is complete (SPIF = 1)
        ;
    PORTB |= (1 << PB2); // Pulls SS high: end of SPI frame
}

int main(void)
{
    SPI_MasterInit(); // Initializes SPI Master

    uint8_t values[3] = {85, 170, 255}; // Values to send in sequence
    uint8_t i = 0;

    while (1)
    {
        SPI_MasterTransmit(values[i]); // Sends one byte from the sequence

        // Moves to the next value
        i++;
        if (i >= 3)
            i = 0;
        _delay_ms(1000); // Waits 1 second before sending the next value
    }
}
