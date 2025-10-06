
void setup() {
  Serial.begin(9600);

  uint8_t sum;    // Declares a C variable for the sum, which will live in a CPU register, randomly chosen by the compiler inside the inline assembly block 
  uint8_t cnt;    // Declares a C variable for the counter
  uint8_t limit;  // Declares limit to compare the counter with

  asm volatile(
    "clr %[sum]        \n\t"   // Clears sum (sets it to 0)
    "ldi %[cnt], 1     \n\t"   // Loads integer 1 inside the register chosen for the counter
    "ldi %[limit], 11  \n\t"   // Loads integer 11 to the limit ?

    "loop%=:           \n\t" // Defines loop as a local label, so it can be used later with the BRNE function
    "add %[sum], %[cnt]\n\t"   // Adds the value of the counter to the sum and stores the result in the register dedicated for the sum
    "inc %[cnt]        \n\t"   // Increments the counter by 1
    "cp  %[cnt], %[limit]\n\t" // Compares the counter to the limit
    "brne loop%=      \n\t"   // If the two are not equal, the compiler jumps back to loop and repeats all previous actions. Once the two are equal, the loop breaks.

    : [sum]   "=r"(sum), // Output operands
      [cnt]   "=r"(cnt),
      [limit] "=r"(limit)
    :                              // Input operands (empty in this case)
    : "cc”.                   // Clobbered list, “cc” tells the compiler that status flags have been changed
  );

  Serial.print("Sum(1..10) = "); 
  Serial.print(sum);            // Prints results of sum in decimal form
  Serial.print(" (0x");
  Serial.print(sum, HEX);    // Prints sum in hexadecimal form
  Serial.println(")");
}

void loop() {
  // Loop is empty
}
