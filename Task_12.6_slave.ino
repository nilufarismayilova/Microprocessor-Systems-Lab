#include <Wire.h>

#define SLAVE_ADDR 0x10 // 7-bit I2C address of the slave

volatile uint8_t last_received = 0; // Global variable to store the last byte received from the master. It is marked as 'volatile' because it can be changed at any moment by the I2C event functions (receiveEvent or requestEvent)

 // Runs automatically whenever the master sends data to this slave. 'howMany' tells us how many bytes are available in the receive buffer
void receiveEvent(int howMany)
{
    if (howMany >= 1) // We only care about the first byte of the message, because the master only sends one byte
        last_received = Wire.read();   // Reads one byte from I2C and stores it
}

// Runs automatically whenever the master requests data from this slave (using Wire.requestFrom on the master side)
void requestEvent()
{
    Wire.write(last_received); // Sends back the last byte we received, which lets the master measure round-trip time for messages
}

void setup()
{
    Wire.begin(SLAVE_ADDR); // Initializes this Arduino as an I2C slave with address SLAVE_ADDR
    Wire.onReceive(receiveEvent); // Registers the function that will handle incoming data from the master
    Wire.onRequest(requestEvent); // Register the function that will handle data requests from the master
}

void loop()
{
    // Nothing
}
