#include <Wire.h>  // Includes the Arduino I2C (TWI) library
#define SLAVE_ADDR 0x10 // I2C address of the slave device

// Configures the I2C clock speed by manually setting the TWI prescaler bits (bit 0 and 1 in TWSR) and the bit rate register (TWBR). They are calculated using the formula from the ATmega328P datasheet (21.5.2) (SCL = F_CPU / (16 + 2 * TWBR * prescaler)) Each 'mode' selects one of the three required speeds: 25 kHz, 100 kHz, or 400 kHz
void setI2CSpeedPreset(uint8_t mode)
{
    if (mode == 0)
    {
        // 25 kHz mode: Prescaler = 4 (TWPS0 = 1), TWBR = 78. Prescaler is 4 because TWBR would be >255 if prescaler = 1
        TWSR = (TWSR & ~((1 << TWPS1) | (1 << TWPS0))) | (1 << TWPS0);
        TWBR = 78;
    }
    else if (mode == 1)
    {
        // 100 kHz (standard mode): Prescaler = 1, TWBR = 72
        TWSR &= ~((1 << TWPS1) | (1 << TWPS0));
        TWBR = 72;
    }
    else if (mode == 2)
    {
        // 400 kHz (fast mode): Prescaler = 1, TWBR = 12. TWBR is much smaller because SCL is faster
        TWBR = 12;
        TWSR &= ~((1 << TWPS1) | (1 << TWPS0));  
    }
}

void setup()
{
    Wire.begin(); // Initializes I2C in master mode
    setI2CSpeedPreset(1);  // Applies chosen speed (0=25kHz, 1=100kHz, 2=400kHz)
}

void loop()
{
    // Begins a transmission to the slave (sends START + address with write bit)
    Wire.beginTransmission(SLAVE_ADDR);  // Begins a transmission to the slave (sends START + address with write bit (SLA+W))
    Wire.write(0x55); // Writes one byte to slave
    Wire.endTransmission(); // Sends STOP condition and finishes the transaction
}
