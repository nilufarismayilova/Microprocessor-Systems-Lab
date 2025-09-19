void setup() {
  Serial.begin(9600); // Initializes Serial Communication at 9600 baud, which lets the arduino send data to the Serial Monitor, so we can see the results.


uint8_t R18;  // Declares an 8-bit unsigned integer variable, called R18. Here, the result will be stored after adding the two integers. We do this because the Serial.print() function can't print registers directly.

asm volatile (
    "ldi r17, 5\n\t"      // The LDI function loads the integer 5 into register r17. r17 = 0b00000101
    "ldi r18, 8\n\t"      // Loads constant value 8 into register r18.
    "add r18, r17\n\t"    // Adds the two integer values and stores the result back in r18: r18 = r18 + r17.
    "mov %0, r18\n\t"     // MOV function copies the register content of r18 into %0, which in this code corresponds to the variable R18
    : "=r" (R18)          // This is the Output operand section. It maps %0 to R18. =r puts the result into a general purpose register and then copies it into R18.
    :                     // This is the Input operand section. It is empty, but still has to be included, otherwise the compiler won't know which section is which.
    : "r17", "r18"        // This is the Clobber list, which lets the compiler know that r17 and r18 were modified and are no longer reliable.
);

  Serial.print("Result = "); // Prints the text "Result = " to the Serial Monitor.
  Serial.println(R18);       // Prints the result of adding the two integers (13), which is stored in R18, to the Serial Monitor
}

void loop() {
  // The loop is empty, so the code only runs once
}
