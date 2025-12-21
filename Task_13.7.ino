#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

#define NUM_PRIMES_TARGET 2000

#define LIMIT       20000 // We need to search at least up to the 2000th prime (~17389). We set LIMIT a bit higher so the sieve covers enough numbers

// Only stores odd numbers in the sieve (3,5,7,...). For index i, the actual value is: value = 2*i + 3. For example: i = 0, value = 2*0 + 3 = 3
// ODD_COUNT shows how many odd numbers exist between 3 and LIMIT
#define ODD_COUNT   ((LIMIT - 1) / 2)

// We use a bitset: each odd number gets 1 bit in the "composite" array. BITSET_SIZE is the number of bytes we need to store ODD_COUNT bits
#define BITSET_SIZE ((ODD_COUNT + 7) / 8)

static uint8_t composite[BITSET_SIZE]; // Global bitset array: composite[i] holds 8 flags (bits). (bit = 1: number is composite, bit = 0: number is prime)

// Mark index "idx" as composite. We need to locate which byte and which bit inside that byte: byte index = idx / 8, bit index  = idx % 8. For example, 11 is in byte 1 (11/8 = 1 R3) and bit 3. Then we set that bit to 1.
static inline void set_composite(uint16_t idx)
{
    composite[idx >> 3] |= (1 << (idx & 7));
}

// Tests whether index "idx" is composite
static inline uint8_t is_composite(uint16_t idx)
{
    return (composite[idx >> 3] & (1 << (idx & 7))) != 0;
}

int main(void)
{

    DDRB |= (1 << DDB5) | (1 << DDB0); // Sets PB5 (onboard LED) and PB0 (to measure time with oscilloscope) as outputs

    PORTB &= ~((1 << PB5) | (1 << PB0)); // Starts with LED off and timin pin low

    cli(); // Disables all interrupts so that nothing disturbs the timing

    // Sets all bits in "composite" array to 0, assuming all odds are prime initially
    for (uint16_t i = 0; i < BITSET_SIZE; i++)
    {
        composite[i] = 0;
    }

 
    PORTB |= (1 << PB0);   // Sets PB0 high to mark the start of the computation (oscilloscope rising edge)


    // We will sieve over odd numbers only, using the mapping: idx -> value = 2*idx + 3.  We only need to use base primes up to sqrt(LIMIT)
    uint16_t max_base = LIMIT;

    // Computes an integer approximation of sqrt(LIMIT) using a simple loop
    uint16_t r = 1;
    while ((uint32_t)r * (uint32_t)r <= max_base) r++;
    uint16_t sqrt_limit = r - 1;

    // Outer loop iterates over all odd candidates (via idx):  idx = 0 → p = 3, idx = 1 → p = 5
    for (uint16_t idx = 0;; idx++)
    {
        uint16_t p = (uint16_t)(2 * idx + 3);   // Converts index "idx" to the actual odd number p

        if (p > sqrt_limit) // Once p exceeds sqrt(LIMIT), we can stop, as Any composite <= LIMIT must have a factor <= sqrt(LIMIT). So larger p do not need to be used as base primes
            break;

        if (is_composite(idx)) // If it has already been marked as a composite we skip it
            continue;   

        // At this point, p is a prime. According to the sieve algorithm, we start marking from p^2 because any smaller multiple k*p (where k < p) was already marked when processing prime k
        uint32_t p2 = (uint32_t)p * (uint32_t)p;
        if (p2 > LIMIT)
            break;

        // Converts the number p^2 to an index in our odd-only array: value = 2*j + 3  =>  j = (value - 3)/2
        uint16_t start_idx = (uint16_t)((p2 - 3) >> 1);

        // Marks all odd multiples of p (p^2, p^2+2p, p^2+4p,...)
        for (uint16_t j = start_idx; j < ODD_COUNT; j += p)
        {
            set_composite(j);
        }
    }

    // Number of primes that have been found so far
    uint16_t count_primes = 0;

    // Handles prime 2 separately because sieve only stores odds (3,5,7,...)
    count_primes++;
    PINB |= (1 << PB5);   

    // Counts odd primes from the sieve
    uint16_t idx = 0;
    while (count_primes < NUM_PRIMES_TARGET && idx < ODD_COUNT)
    {
        if (!is_composite(idx)) // If this index is not composite, then the number is prime
        {
            count_primes++;
            PINB |= (1 << PB5); // Toggles LED for each prime found
        }
        idx++; // Moves on to the next odd number
    }

    PORTB &= ~(1 << PB0);  // Pulls PB0 low to mark the end of computation (oscilloscope falling edge). The pulse width between rising and falling edges gives us the total runtime

    PORTB |= (1 << PB5); // Leaves LED on permanently to show we reached 2000 primes

    // Stops forever
    for (;;)
    {

    }

    return 0;
}
