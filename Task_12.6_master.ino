#include <Wire.h>
#define SLAVE_ADDR 0x10 // I2C address of the slave device

// Configures the I2C clock speed by configuring the TWI prescaler bits (bit 0 and 1 in TWSR) and the bit rate register (TWBR). They are calculated using the formula from the ATmega328P datasheet (21.5.2) (SCL = F_CPU / (16 + 2 * TWBR * prescaler))
void configureI2CSpeed(uint32_t freq)
{
    // 25 kHz (slow mode)
    if (freq == 25000)
    {
        // For 25 kHz, TWBR would be >255 if prescaler = 1, so we use prescaler = 4 (TWPS0 = 1, TWPS1 = 0) and TWBR = 78
        TWSR = (TWSR & ~((1 << TWPS1) | (1 << TWPS0))) | (1 << TWPS0);
        TWBR = 78;

        Wire.begin(); // Initializes I2C in master mode (Wire.begin() resets TWBR and TWSR internally)

        // Reapplies prescaler and TWBR after Wire.begin(), as 'Wire.begin()' reset TWBR and TWSR
        TWSR = (TWSR & ~((1 << TWPS1) | (1 << TWPS0))) | (1 << TWPS0);
        TWBR = 78;

        Serial.println("Configured freq = 25000 Hz, Prescaler=4, TWBR=78");
        return;  
    }

    // Rearranged formula from the datasheet: TWBR = (F_CPU / SCL - 16) / (2 * prescaler), here the prescaler = 1, so: TWBR = (F_CPU / SCL - 16) / 2
    uint32_t twbr_val = (F_CPU / freq - 16) / 2;

    // Makes sure TWBR fits in 8 bits
    if (twbr_val > 255)
        twbr_val = 255;

    TWSR &= ~((1 << TWPS1) | (1 << TWPS0)); // Sets prescaler to 1

    TWBR = (uint8_t)twbr_val; // Sets TWBR value (72 for 100 kHz, 12 for 400 kHz)


    // Initializes I2C. This will again overwrite TWBR/TWSR, so we call 'Wire.begin()' first and then reapply our settings.
    Wire.begin();

    // Reapplies prescaler and TWBR after Wire.begin()
    TWSR &= ~((1 << TWPS1) | (1 << TWPS0));
    TWBR = (uint8_t)twbr_val;

    // Prints the configured frequency and TWBR value
    Serial.print("Configured freq = ");
    Serial.print(freq);
    Serial.print(" Hz, TWBR = ");
    Serial.println((uint8_t)twbr_val);
}

// Performs one full I2C exchange: Master writes 1 byte to the slave and then master requests 1 byte back from the slave
void exchange_once()
{
    // Master to Slave (write phase)
    Wire.beginTransmission(SLAVE_ADDR); // Sends START + SLA+W
    Wire.write(0x55); // Sends a single test byte
    Wire.endTransmission(); // Sends STOP and finishes the write

    // Slave to Master (read phase)
    Wire.requestFrom(SLAVE_ADDR, 1); // Requests 1 byte from the slave
    while (Wire.available() == 0); // Waits until a byte is received   
    uint8_t response = Wire.read(); // Reads the byte from the slave
}

//Measures the total time (in microseconds) for 50 write+read exchanges at the current I2C speed.
uint32_t run_test()
{
    uint32_t t0 = micros(); // Time at start
    for (int i = 0; i < 50; i++) // Performs 50 exchanges
        exchange_once();
    uint32_t t1 = micros(); // Time at end

    return (t1 - t0);  // Total time in microseconds
}

void setup()
{
    Serial.begin(9600); // Initializes serial monitor for results
    delay(500); 

    // Test 1: Default Wire speed (~100 kHz)
    Wire.begin(); // Initializes I2C with default settings
    uint32_t t_default = run_test(); // Time 50 exchanges
    Serial.print("Default (~100 kHz) time (us): ");
    Serial.println(t_default);

    delay(1000);

    // Test 2: 25 kHz (slow mode, prescaler = 4, TWBR = 78)
    configureI2CSpeed(25000);
    uint32_t t25 = run_test();
    Serial.print("25 kHz time (us): ");
    Serial.println(t25);

    delay(1000);

    // Test 3: 100 kHz (standard mode, prescaler = 1, TWBR = 72)
    configureI2CSpeed(100000);
    uint32_t t100 = run_test();
    Serial.print("100 kHz time (us): ");
    Serial.println(t100);

    delay(1000);

    // Test 4: 400 kHz (fast mode, prescaler = 1, TWBR = 12). On Arduino Uno, for fast-mode timing the real clock is usually a bit lower (~270–320 kHz), due to bus capacitance and the weak internal pull-ups which slow the rising edges
    configureI2CSpeed(400000);
    uint32_t t400 = run_test();
    Serial.print("400 kHz time (us): ");
    Serial.println(t400);

    Serial.println("TEST COMPLETE");
}

void loop()
{
    // Nothing
}
