// Receives 1 byte from master - updates LED_B
// Sends 1 byte back - state of BUTTON_B

#define F_CPU 16000000UL // CPU clock frequency 
#include <avr/io.h>               
#include <util/delay.h>           

#define SLAVE_ADDR 0x10 // 7-bit I2C slave address
#define BUTTON_B   PD2  // Button pin on PORTD, bit 2
#define LED_B      PD3  // LED pin on PORTD, bit 3

void TWI_SlaveInit(uint8_t address)
{
    PRR &= ~(1 << PRTWI);  // Enables TWI module by clearing power-reduction TWI (bit 7 in PRR)

    TWAR = (address << 1);  // Loads 7-bit slave address into TWI address register

    TWCR = (1 << TWEA) | (1 << TWEN); // Enables TWI hardware and ACK
}

void io_init(void)
{
    DDRD &= ~(1 << BUTTON_B); // Sets BUTTON_B as input
    PORTD |= (1 << BUTTON_B); // Enables internal pull-up on BUTTON_B

    DDRD |= (1 << LED_B); // Sets LED_B as output
}

int main(void)
{
    io_init(); // Configures button and LED pins
    PORTC |= (1 << PC4) | (1 << PC5); // Enables internal pull-up on SDA/SCL pins, ensuring they are not driven low

    TWI_SlaveInit(SLAVE_ADDR);  // Initializes TWI in slave mode with given address

    uint8_t received = 0;  // Stores last byte received from master
    uint8_t button_b_state = 0; // Current logical state of the button

    static uint8_t led_b_state = 0; // Internal LED toggle state
    static uint8_t prev_button_b_state = 0; // Previous button state for edge detection
    uint8_t toggle_event_b = 0; // Flag indicating a button press event

    while (1)
    {
        while (!(TWCR & (1 << TWINT))); // Waits until a TWI event is complete (TWINT = 1)

        uint8_t status = TWSR & 0xF8; // Reads TWI status code (mask prescaler bits)

        // Master to slave write (SLA+W)
        if (status == 0x60 || status == 0x68) // Own address + Write received
        {
            TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN); // Clears flag, ACK next byte, keeps TWI on
        }
        // Data received from master
        else if (status == 0x80) // Data byte received, ACK returned
        {
            received = TWDR; // Reads received data from TWI data register

            if (received == 0x01) // If the command is 0x01, toggle LED
            {
                led_b_state ^= 1; // Toggles internal LED state variable

                if (led_b_state) // If LED state is ON
                    PORTD |= (1 << LED_B); // Turns LED pin high
                else
                    PORTD &= ~(1 << LED_B); // Turns LED pin low
            }

            TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN); // Prepares for the next received byte
        }
        // Master from slave read (SLA+R)
        else if (status == 0xA8 || status == 0xB0) // Master wants to read from slave
        {
            button_b_state = !(PIND & (1 << BUTTON_B)); // Reads button (active-low to active-high)

            toggle_event_b = 0; // Assumes no new toggle event by default
            if (button_b_state == 1 && prev_button_b_state == 0) // Rising edge (button just pressed)
            {
                toggle_event_b = 1; // Marks a new toggle event
            }
            prev_button_b_state = button_b_state; // Stores current button state for next time

            TWDR = toggle_event_b ? 0x01 : 0x00;  // Loads reply byte: 1 if event, else 0. Uses ternary operator: "if toggle_event_b is true (1), then assign 0x01 to TWDR, otherwise assign 0x00)"

            TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN); // Starts transmitting data to master
        }
        // Master done reading
        else if (status == 0xC0 || status == 0xC8) // Data has been sent and master ended read
        {
            TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN); // Goes back to idle ready state
        }
        else
        {
            TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN); // Default: recovers and waits for the next event
    }
}
