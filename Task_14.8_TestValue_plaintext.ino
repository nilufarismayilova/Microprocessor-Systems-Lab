// Modified version of the transmitter code to capture a plaintext test value (42) for the oscilloscope waveform

#include <stdint.h>

const uint8_t SECRET_KEY = 0x54;  //8-bit symmetric decryption key 

// 4-bit substitution box (S-box). Each input nibble (0–15) is mapped to a different output value to introduce non-linearity (confusion) into the cipher
const uint8_t SBOX[16] = {
  6, 4,12, 5,
  0, 7, 2,14,
  1,15, 3,13,
  8,10, 9,11
};

// Bit permutation used after S-box layer. It rearranges the bit positions of the input byte to provide diffusion
uint8_t permute(uint8_t x) {
  uint8_t y = 0;

  // Each output bit is assigned from a specific input bit
  y |= ((x >> 3) & 0x01) << 7; // C7 (output bit) = S3 (inout bit)
  y |= ((x >> 5) & 0x01) << 6; // C6 (output bit) = S5 (input bit)
  y |= ((x >> 1) & 0x01) << 5; // C5 (output bit) = S1 (input bit)
  y |= ((x >> 0) & 0x01) << 4; // C4 (output bit) = S0 (input bit)
  y |= ((x >> 7) & 0x01) << 3; // C3 (output bit) = S7 (input bit)
  y |= ((x >> 2) & 0x01) << 2; // C2 (output bit) = S2 (input bit)
  y |= ((x >> 4) & 0x01) << 1; // C1 (output bit) = S4 (input bit)
  y |= ((x >> 6) & 0x01) << 0; // C0 (output bit) = S6 (input bit)

  return y;
}

uint8_t encryptByte(uint8_t p) {

  uint8_t x = p ^ SECRET_KEY; // Key mixing: XOR the plaintext with the secret key

  // S-box substitution of nibbles
  uint8_t hi = (x >> 4) & 0x0F; // Splits the value into high and low 4-bit nibbles
  uint8_t lo =  x       & 0x0F;

  uint8_t shi = SBOX[hi]; // Applies the S-box to each nibble 
  uint8_t slo = SBOX[lo];

  uint8_t s = (shi << 4) | slo; // Recombins the nibbles into one byte

  return permute(s); // Permutation: Applies bit-level permutation to produce the ciphertext
}
}

void setup() {
  Serial.begin(9600);
}

const uint8_t TEST_VALUE = 42; // Sends 42 as test value

void loop() {
  Serial.write(TEST_VALUE);      // Sends the plaintext byte without encryption
  delay(100);
}
