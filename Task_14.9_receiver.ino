#include <Arduino.h>
#include <stdint.h>

// XTEA parameters:
// 128-bit key stored as four 32-bit words (must be identical for sender and receiver)
static const uint32_t XTEA_KEY[4] = {
  0xA56BABCDUL, 0x000FF111UL, 0xDEADBEEFUL, 0x12345678UL
};

// 64-bit nonce (shared value that helps prevent reuse of the same keystream during encryption). In a real system this should be random and unique, but for this lab it is fixed so both boards start in sync
static const uint32_t NONCE0 = 0xCAFEBABEUL;
static const uint32_t NONCE1 = 0x0BADF00DUL;

// XTEA constants (new-variant XTEA uses delta = 0x9E3779B9, 32 cycles)
static const uint32_t DELTA = 0x9E3779B9UL;

// XTEA block encryption function
// Input: v[0], v[1] (two 32-bit halves)
// Output: v[0], v[1] encrypted in-place
static void xtea_encipher(uint32_t v[2], const uint32_t k[4]) {

  uint32_t y = v[0], z = v[1]; // Copies input block halves into working variables
  uint32_t sum = 0;

  // Performs 32 encryption cycles (standard XTEA)
  for (uint8_t n = 0; n < 32; n++) {
    y += ((z << 4) ^ (z >> 5)) + z ^ sum + k[sum & 3]; // Updates first half using shifts, XORs, addition, and key mixing
    sum += DELTA; // Increment sum by the delta constant
    z += ((y << 4) ^ (y >> 5)) + y ^ sum + k[(sum >> 11) & 3]; // Updates second half using the updated y value
  }
  // Stores encrypted result back into the block
  v[0] = y;
  v[1] = z;
}

// CTR keystream generator (byte-wise). Each encrypted 64-bit counter block produces 8 bytes
static uint32_t blockCounter = 0; // Counts how many 64-bit keystream blocks have been generated
static uint8_t  ksIndex = 8; // (0-7), is set to 8 so that a block is generated on first use
static uint8_t  ksBuf[8]; // Holds the current 8-byte keystream block

// Generates a new 64-bit keystream block
static void refillKeystreamBlock() {
  uint32_t v[2]; // Counter block to be encrypted

  // Builds 64-bit counter block using nonce and block counter
  v[0] = NONCE0 ^ blockCounter;
  v[1] = NONCE1;

  xtea_encipher(v, XTEA_KEY);  // Encrypts counter block using XTEA

  // Stores encrypted 64-bit output as 8 keystream bytes (little-endian: LSB is stored first)
  ksBuf[0] = (uint8_t)(v[0] >> 0);
  ksBuf[1] = (uint8_t)(v[0] >> 8);
  ksBuf[2] = (uint8_t)(v[0] >> 16);
  ksBuf[3] = (uint8_t)(v[0] >> 24);
  ksBuf[4] = (uint8_t)(v[1] >> 0);
  ksBuf[5] = (uint8_t)(v[1] >> 8);
  ksBuf[6] = (uint8_t)(v[1] >> 16);
  ksBuf[7] = (uint8_t)(v[1] >> 24);

  ksIndex = 0; // Resets keystream index
  blockCounter++; // Increments counter
}

// Returns the next keystream byte for CTR mode
static uint8_t nextKeystreamByte() {

  // Generates a new keystream block if all bytes are used
  if (ksIndex >= 8) {
    refillKeystreamBlock();
  }
  return ksBuf[ksIndex++]; // Returns current keystream byte and increments index
}

void setup() {
  Serial.begin(9600); // Initializes UART at 9600 baud
}

void loop() {

  // Means at least one byte is waiting to be read
  if (Serial.available() > 0) {
    uint8_t ct = (uint8_t)Serial.read(); // Reads one encrypted byte from UART

    // Gets matching keystream byte and decrypts (CTR uses XOR both ways)
    uint8_t ks = nextKeystreamByte();
    uint8_t pt = ct ^ ks;               

    // Prints values on Serial Monitor
    Serial.print("Cipher: ");
    Serial.print(ct);
    Serial.print("  Plain: ");
    Serial.println(pt);
  }
}
