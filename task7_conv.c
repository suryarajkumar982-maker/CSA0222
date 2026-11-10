/* Task 7 - type conversion and precision */
#include <stdio.h>
#include <math.h>
int main(void)
{
    printf("== 1. Integer promotion ==\n");
    unsigned short a = 60000, b = 60000;
    printf("a + b = %d (both promoted to int, so NO 16-bit wrap); (unsigned short)(a+b) = %u\n", a + b, (unsigned)(unsigned short)(a + b));
    char c1 = 100, c2 = 100;
    printf("char 100 + char 100 = %d (promoted to int; not -56)\n", c1 + c2);

    printf("\n== 2. Usual arithmetic conversions (signed vs unsigned) ==\n");
    int neg = -1; unsigned int one = 1u;
    printf("(-1 < 1u) is %d   <- UNEXPECTED: -1 is converted to 4294967295u\n", neg < one);

    printf("\n== 3. Integer division errors ==\n");
    printf("7 / 2 = %d,  7 / 2.0 = %.1f,  1 / 3 * 3.0 = %.1f (UNEXPECTED: 0)\n", 7 / 2, 7 / 2.0, 1 / 3 * 3.0);
    long units = 250, total = 52500;
    printf("avg paise/unit: total/units = %ld (integer, truncated) vs (double) = %.2f\n", total / units, (double)total / units);
    printf("(units/100)*100 = %ld  (not 250) <- slab rounding bug\n", (units / 100) * 100);

    printf("\n== 4. Explicit casts and narrowing ==\n");
    long big = 5000000000L;
    printf("(int)5000000000L = %d  (truncated; sizeof(long)=%zu, sizeof(int)=%zu)\n", (int)big, sizeof(long), sizeof(int));
    printf("(unsigned char)300 = %d,  (int)3.99 = %d,  (int)-3.99 = %d\n", (unsigned char)300, (int)3.99, (int)-3.99);

    printf("\n== 5. Rounding fractional charge to paise: 33 units x Rs 1.505 ==\n");
    double rupees = 33 * 1.505;
    printf("rupees (double)        = %.17f\n", rupees);
    printf("(long)(rupees*100)     = %ld   <- truncation (wrong)\n", (long)(rupees * 100));
    printf("lround(rupees*100)     = %ld   <- right here only because rupees*100 rounds to 4966.5; not guaranteed\n", lround(rupees * 100));
    printf("integer: (33*1505+5)/10= %ld   <- exact (rate kept as 1505 milli-rupees)\n", (33L * 1505L + 5L) / 10L);

    printf("\n== 6. Other unexpected expression results ==\n");
    printf("0.1 + 0.2 == 0.3 ? %d\n", 0.1 + 0.2 == 0.3);
    float fs = 0; for (int i = 0; i < 10; ++i) fs += 0.1f;
    printf("float: ten times 0.1f = %.8f, == 1.0f ? %d\n", fs, fs == 1.0f);
    printf("sizeof(char) > -1 ? %d  (sizeof is size_t, unsigned: -1 becomes huge -> 0)\n", sizeof(char) > -1);
    printf("5 / 2 * 2.0 = %.1f  but 5 * 2.0 / 2 = %.1f\n", 5 / 2 * 2.0, 5 * 2.0 / 2);

    printf("\n== 7. Accuracy: float vs integer paise (1,000,000 x Rs 0.07) ==\n");
    float fsum = 0; double dsum = 0; long psum = 0;
    for (int i = 0; i < 1000000; ++i) { fsum += 0.07f; dsum += 0.07; psum += 7; }
    printf("exact = Rs 70000.00 | float = %.4f (err %+.4f) | double = %.6f (err %+.8f) | paise = %ld.%02ld\n",
           fsum, fsum - 70000.0, dsum, dsum - 70000.0, psum / 100, psum % 100);
    printf("Space: float 4 B, double 8 B, long 8 B (int32 4 B).  Time: integer add is 1 cycle; FP add 3-5 cycles latency.\n");
    return 0;
}
