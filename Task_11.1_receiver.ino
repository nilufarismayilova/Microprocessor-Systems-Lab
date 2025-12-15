#include <stdint.h>

const uint8_t SECRET_KEY = 0x54; //8-bit symmetric decryption key (identical to the encryption key)

// Inverse S-box (reverses the substitutions performed by the S-box used during encryption)
const uint8_t INV_SBOX[16] = {
  4, 8, 6,10,
  1, 3, 0, 5,
 12,14,13,15,
   2,11, 7, 9
};

// Inverse bit permutation (reverses the bit rearrangement applied during the encryption permutation)
uint8_t invPermute(uint8_t x) {
  uint8_t y = 0;

  // Original bit positions are restored from the ciphertext
  y |= ((x >> 7) & 0x01) << 3; // S3 (input bit) = C7 (output bit)
  y |= ((x >> 6) & 0x01) << 5; // S5 (input bit) = C6 (output bit)
  y |= ((x >> 5) & 0x01) << 1; // S1 (input bit) = C5 (output bit)
  y |= ((x >> 4) & 0x01) << 0; // S0 (input bit) = C4 (output bit)
  y |= ((x >> 3) & 0x01) << 7; // S7 (input bit) = C3 (output bit)
  y |= ((x >> 2) & 0x01) << 2; // S2 (input bit) = C2 (output bit)
  y |= ((x >> 1) & 0x01) << 4; // S4 (input bit) = C1 (output bit)
  y |= ((x >> 0) & 0x01) << 6; // S6 (input bit) = C0 (output bit)

  return y;
}

uint8_t decryptByte(uint8_t c) {

  uint8_t s = invPermute(c); // Inverse permutation: undo the permutation applied during encryption

  // Inverse S-box substitution
  uint8_t shi = (s >> 4) & 0x0F; // Undo the S-box substitution by spliting the value into high and low nibbles
  uint8_t slo =  s       & 0x0F;

  uint8_t hi = INV_SBOX[shi]; // Applies inverse S-box to each nibble
  uint8_t lo = INV_SBOX[slo];

  uint8_t x = (hi << 4) | lo; // Recombines the nibbles to reconstruct the previous value


  return x ^ SECRET_KEY; // Inverse key mixing: undo key mixing by XORing with the secret key
}

void setup() {
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) { // Checks if at least one byte has been received over UART
    uint8_t cipher = Serial.read(); // Reads one encrypted byte from the serial buffer
    uint8_t plain  = decryptByte(cipher); // Decrypts the received ciphertext byte

    // Prints the ciphertext and recovered plaintext to Serial Monitor
    Serial.print("Cipher: ");
    Serial.print(cipher);
    Serial.print("  Plain: ");
    Serial.println(plain);
  }
}
