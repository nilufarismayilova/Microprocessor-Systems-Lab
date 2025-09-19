void setup() {
  Serial.begin(9600); // Initialize Serial Communication

  uint8_t result;  // Will store the subtraction output
  uint8_t sreg;   // Will hold SREG after subtracting the integers

  asm volatile (
    "ldi r16, 50\n\t"    // Loads integer 50 into r16
    "ldi r17, 200\n\t"   // Loads integer 200 into r17
    "sub r16, r17\n\t"   // r16 = 50 - 200. Since the result is negative, it wraps to 106: 50-200+2^8. This also updates SREG flags. Z will be 0, because the result is nonzero and C will 1, because we used a borrow since the resulting value was negative.
    "in __tmp_reg__, __SREG__\n\t"  // Copies I/O register value into a CPU register. __tmp_reg__ is another name for r0, to which SREG is moved.
    "mov %1, __tmp_reg__\n\t"     // Copies r0, which contains the contents of SREG into the ouput operand 1%, which will later be mapped to the C variable sreg.
    "mov %0, r16\n\t"             // Copies r16 into %0, which will be mapped to the C variable result
    : "=r"(result), "=r"(sreg)    // The Output operands
    :                              // No input operands
    : "r16", "r17"                 // Clobbered registers list
  );


  bool Z = (sreg & (1 << 1));   // (1 << 1) masks for bit 1 of SREG (Z flag): 0b00000010. The AND operand gives us a 0 result, since bit 1 in SREG is 0.
  bool C = (sreg & (1 << 0));   // (1 << 0) masks for bit 0 of SREG (C flag): 0b00000001. The AND operand gives us a 1 result, since bit 1 in SREG is 1.

 
  Serial.print("Result = ");
  Serial.println(result);        // Prints the wrapped result of subtracting the two integers (106)

  Serial.print("SREG (bin) = "); 
  Serial.println(sreg, BIN);    // Prints SREQ value in binary

  Serial.print("Z (zero) = ");
  Serial.println(Z);          // Prints indvidual Z flag (0)

  Serial.print("C (carry/borrow) = ");
  Serial.println(C);            // Prints indvidual C flag (1)
}

void loop() {
  // Loop is empty
}
