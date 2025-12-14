#include <stdint.h>

// Encrypted ciphertext bytes given in Task description
const uint8_t cipher[] = {
  87, 26, 72, 13, 67,
  95, 89, 23, 72, 22,
  73, 11, 87, 30, 73
};
const uint8_t cipherLen = sizeof(cipher); // Length of the ciphertext array

char plaintext[cipherLen + 1]; // Buffer to store a candidate decrypted plaintext. One extra byte is reserved for the null terminator ('\0'), so the plaintext can be treated as a C string

// Function to determine whether the decrypted plaintext candidate consists of: exactly two lowercase words, exactly one space separating the two words. We use this function is to filter out incorrect key guesses during the brute-force search
bool isTwoLowercaseWords(const char *s) {
  int spaces = 0;

// Scans each character of the candidate plaintext
  for (uint8_t i = 0; i < cipherLen; i++) {
    char c = s[i];

    if (c == ' ') {
      spaces++; // Counts spaces
    }
    else if (c < 'a' || c > 'z') {
      return false; // Rejects non-lowercase characters
    }
  }
  
  if (spaces != 1) return false; // Rejects if there is not exactly one space
  if (s[0] == ' ' || s[cipherLen - 1] == ' ') return false; // Rejects leading or trailing spaces

  return true; // Plaintext matches expected pattern
}


void setup() {
  Serial.begin(9600); // Initializes serial communication
  while (!Serial) { ; }

  Serial.println(F("Starting brute-force of two XOR keys...\n"));

  unsigned long startTotal = millis();   // Records the starting time for the brute-force process.

  // Tries all possible 256 × 256 = 65536 key combinations
  for (uint16_t key0 = 0; key0 < 256; key0++) {
    for (uint16_t key1 = 0; key1 < 256; key1++) {

      // Decrypts the ciphertext using the current key pair: Even-indexed bytes are XORed with key0, and odd-indexed bytes are XORed with key1.
      for (uint8_t i = 0; i < cipherLen; i++) {
        uint8_t useKey = (i & 1) ? key1 : key0;   // Faster than i % 2
        plaintext[i] = (char)(cipher[i] ^ useKey);
      }
      plaintext[cipherLen] = '\0'; // Null-terminates the plaintext string

      if (isTwoLowercaseWords(plaintext)) {
        unsigned long elapsed = millis() - startTotal; // Calculates elapsed time until this match was found

        Serial.println(F("POSSIBLE MATCH FOUND"));
        Serial.print(F("key0 = "));
        Serial.print(key0);
        Serial.print(F(" (0x"));
        Serial.print(key0, HEX);
        Serial.println(F(")"));

        Serial.print(F("key1 = "));
        Serial.print(key1);
        Serial.print(F(" (0x"));
        Serial.print(key1, HEX);
        Serial.println(F(")"));

        Serial.print(F("Plaintext: \""));
        Serial.print(plaintext);
        Serial.println(F("\""));

        Serial.print(F("Time taken until match (ms): "));
        Serial.println(elapsed);
        Serial.println(F("\n"));
      }
    }
  }

  unsigned long totalElapsed = millis() - startTotal;   // Measures the total execution time for the complete brute-force attack

  Serial.println(F("Brute force finished."));
  Serial.print(F("TOTAL EXECUTION TIME (ms): "));
  Serial.println(totalElapsed);
}

void loop() {
// Empty
}
