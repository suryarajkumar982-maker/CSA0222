/* Task 10 - float vs integer-paise totals; int/long/long long overflow at district scale */
#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include <time.h>
#include "tariff.h"

static long bill_for(long i)            /* deterministic pseudo-consumption: 0..449 units */
{
    long units = (i * 37L) % 450L;
    return energy_charge_paise(units) + FIXED_CHARGE_PAISE[i % 3];
}

int main(void)
{
    static const long N[] = { 4000L, 60000L, 800000L };
    printf("%-9s %-18s %-20s %-18s %-14s %-14s\n", "Bills", "exact (int64 paise)", "float rupees", "double rupees", "int32 total", "long total");
    for (int k = 0; k < 3; ++k) {
        float fsum = 0.0f; double dsum = 0.0; long long exact = 0; int32_t i32 = 0; long lng = 0; int ovf32 = 0;
        for (long i = 0; i < N[k]; ++i) {
            long p = bill_for(i);
            fsum += (float)p / 100.0f; dsum += (double)p / 100.0; exact += p; lng += p;
            if (__builtin_add_overflow(i32, (int32_t)p, &i32)) ovf32 = 1;
        }
        printf("%-9ld Rs %-15lld %-20.2f %-18.4f %-14s %ld\n", N[k], exact / 100, fsum, dsum, ovf32 ? "OVERFLOW" : "ok", lng);
        printf("          exact = %lld paise | float error = %+.2f Rs | double error = %+.6f Rs | int32 wrapped value = %d\n",
               exact, fsum - (double)exact / 100.0, dsum - (double)exact / 100.0, i32);
    }

    printf("\n-- long long vs double for district totals (exactness limit) --\n");
    long long big = 9007199254740993LL;             /* 2^53 + 1 */
    double dbig = (double)big;
    printf("2^53+1 as long long = %lld ; stored in double = %.0f  (double cannot hold it exactly)\n", big, dbig);
    printf("Largest district total (800,000 bills x 12 months x Rs 50,000 max) = %lld paise (< 2^53 = 9007199254740992, so both fit)\n", 800000LL * 12 * 5000000LL);
    printf("Max values: INT_MAX=%d (Rs %d.%02d), LONG_MAX=%ld, LLONG_MAX=%lld\n", INT_MAX, INT_MAX / 100, INT_MAX % 100, LONG_MAX, LLONG_MAX);

    printf("\n-- Actual vs minimum guaranteed by C standard --\n");
    printf("%-12s %-12s %-14s %-s\n", "type", "this system", "C standard min", "actual range");
    printf("%-12s %-12zu %-14s %d .. %d\n", "int", sizeof(int), "2 B (16 bit)", INT_MIN, INT_MAX);
    printf("%-12s %-12zu %-14s %ld .. %ld\n", "long", sizeof(long), "4 B (32 bit)", LONG_MIN, LONG_MAX);
    printf("%-12s %-12zu %-14s %lld .. %lld\n", "long long", sizeof(long long), "8 B (64 bit)", LLONG_MIN, LLONG_MAX);
    printf("%-12s %-12zu %-14s 0 .. %u\n", "unsigned short", sizeof(unsigned short), "2 B (16 bit)", (unsigned)USHRT_MAX);
    printf("Standard guarantees only: int >= -32767..32767, long >= +-2147483647, long long >= +-9223372036854775807\n");

    printf("\n-- Time: 50,000,000 additions --\n");
    volatile double ds = 0; volatile long long ls = 0; clock_t t = clock();
    for (long i = 0; i < 50000000L; ++i) ds += 12.34;
    double td = (double)(clock() - t) / CLOCKS_PER_SEC; t = clock();
    for (long i = 0; i < 50000000L; ++i) ls += 1234;
    double tl = (double)(clock() - t) / CLOCKS_PER_SEC;
    printf("double = %.3f s   long long = %.3f s   (both 8 bytes of space)\n", td, tl);
    return 0;
}
