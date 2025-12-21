// Sends 1 byte to slave (0x01 = pressed, 0x00 = released)
// Receives 1 byte back from slave and updates LED

#define F_CPU 16000000UL // CPU clock frequency (16 MHz)
#include <avr/io.h>                   
#include <util/delay.h>    

#define SLAVE_ADDR 0x10 // 7-bit I2C slave address
#define BUTTON_A   PD2 // Button pin on PORTD, bit 2
#define LED_A      PD3  // LED pin on PORTD, bit 3


void TWI_MasterInit(void)
{
    PRR &= ~(1 << PRTWI); // Enables TWI module by clearing power-reduction TWI (bit 7 in PRR)

    TWSR &= ~((1 << TWPS1) | (1 << TWPS0));  // Sets TWI prescaler to 1

    TWBR = 72; // Sets TWI bit rate (controls SCL clock speed)

    TWCR = (1 << TWEN); // Enables TWI hardware (master mode, no interrupts)
}


// Sends START condition
void TWI_Start(void)
{
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN); // Requests START condition and clears flag
    while (!(TWCR & (1 << TWINT)));  // Waits until START condition has been transmitted
}

void TWI_Stop(void)
{
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN); // Requests STOP condition and clears flag
}

void TWI_Write(uint8_t data)
{
    TWDR = data;                      // Loads data byte into TWI data register
    TWCR = (1 << TWINT) | (1 << TWEN); // Starts transmission of data byte
    while (!(TWCR & (1 << TWINT)));   // Waits until data has been transmitted
}

uint8_t TWI_Read_ACK(void)
{
    TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN); // Starts reception and prepares to send ACK
    while (!(TWCR & (1 << TWINT)));  // Waits until data byte has been received
    return TWDR;                     // Returns received byte
}

uint8_t TWI_Read_NACK(void)
{
    TWCR = (1 << TWINT) | (1 << TWEN); // Starts reception but does not send ACK after
    while (!(TWCR & (1 << TWINT)));    // Waits until data byte has been received
    return TWDR;                       // Returns received byte
}

void io_init(void)
{
    DDRD &= ~(1 << BUTTON_A); // Sets BUTTON_A as input
    PORTD |= (1 << BUTTON_A); // Enables internal pull-up on BUTTON_A

    DDRD |= (1 << LED_A);  // Sets LED_A as output
}

int main(void)
{
    io_init(); // Configures button and LED pins
    TWI_MasterInit(); // Initializes TWI in master mode

    PORTC |= (1 << PC4) | (1 << PC5); // Enables internal pull-up on SDA/SCL pins, ensuring they are not driven low

    uint8_t button_state = 0; // Current sampled state of button A
    uint8_t slave_reply  = 0; // Byte received from slave

    static uint8_t prev_button_state = 0; // Previous button state for edge detection
    uint8_t toggle_event = 0; // Flag indicating a button press event


    while (1)
    {
        button_state = !(PIND & (1 << BUTTON_A)); // Reads button (active-low invert to active-high)

        toggle_event = 0; // Assumes no new press by default
        if (button_state == 1 && prev_button_state == 0) // Checks for rising edge (not pressed to pressed)
        {
            toggle_event = 1; // Marks a single press event
        }
        prev_button_state = button_state; // Stores current button state for next loop


        // Send to slave
        TWI_Start(); // Sends START condition
        TWI_Write((SLAVE_ADDR << 1) | 0);  // Sends SLA+W (slave address + write bit 0)
        TWI_Write(toggle_event ? 0x01 : 0x00); // Sends 0x01 if press event, else 0x00. Uses ternary operator: "if toggle_event is true (1), then assign 0x01 to TWDR, otherwise assign 0x00)"

        TWI_Stop(); // Sends STOP condition to end write

        _delay_ms(5);  // Short delay before read phase

        // Read from slave
        TWI_Start(); // Sends START condition
        TWI_Write((SLAVE_ADDR << 1) | 1); // Sends SLA+R (slave address + read bit 1)
        slave_reply = TWI_Read_NACK();  // Reads one byte from slave and sends NACK (end read)
        TWI_Stop(); // Send STOPs condition to release bus

        static uint8_t led_a_state = 0; // Internal state of LED_A (for toggling)

        if (slave_reply == 0x01) // If slave reports a button event
        {
            led_a_state ^= 1; // Toggles LED state variable

            if (led_a_state) // If LED should be ON
                PORTD |= (1 << LED_A);  // Set LED_A pin high
            else   // If LED should be OFF
                PORTD &= ~(1 << LED_A); // Clear LED_A pin
        }

        _delay_ms(100); // Delay 
    }
}
