/* Task 2 - data types: sizes, ranges, float drift, 16-bit rollover, double vs paise */
#include <stdio.h>
#include <limits.h>
#include <float.h>
#include <stdint.h>
#include <time.h>

int main(void)
{
    printf("=== sizeof (bytes) on this machine ===\n");
    printf("char=%zu short=%zu int=%zu long=%zu long long=%zu float=%zu double=%zu\n",
           sizeof(char), sizeof(short), sizeof(int), sizeof(long), sizeof(long long),
           sizeof(float), sizeof(double));
    printf("unsigned short=%zu int32_t=%zu int64_t=%zu\n", sizeof(unsigned short), sizeof(int32_t), sizeof(int64_t));

    printf("\n=== Ranges from limits.h / float.h ===\n");
    printf("USHRT_MAX = %u\n", (unsigned)USHRT_MAX);
    printf("INT_MIN   = %d   INT_MAX = %d\n", INT_MIN, INT_MAX);
    printf("UINT_MAX  = %u\n", UINT_MAX);
    printf("LONG_MIN  = %ld   LONG_MAX = %ld\n", LONG_MIN, LONG_MAX);
    printf("LLONG_MAX = %lld\n", LLONG_MAX);
    printf("FLT_MAX = %e  FLT_DIG = %d  FLT_EPSILON = %e\n", FLT_MAX, FLT_DIG, FLT_EPSILON);
    printf("DBL_MAX = %e  DBL_DIG = %d  DBL_EPSILON = %e\n", DBL_MAX, DBL_DIG, DBL_EPSILON);

    printf("\n=== Float precision: adding Rs 1234.57 for 4000 bills ===\n");
    {
        float f = 0.0f; double d = 0.0; long paise = 0;
        for (int i = 0; i < 4000; ++i) { f += 1234.57f; d += 1234.57; paise += 123457L; }
        printf("expected exact total      : Rs 4938280.00\n");
        printf("float  sum (rupees)       : %.4f   error = %+.4f\n", f, f - 4938280.0f);
        printf("double sum (rupees)       : %.6f   error = %+.8f\n", d, d - 4938280.0);
        printf("integer paise sum         : %ld paise = Rs %ld.%02ld   error = 0\n", paise, paise / 100, paise % 100);
        printf("float: 16777216.0f + 1.0f == 16777216.0f ? %s\n", (16777216.0f + 1.0f == 16777216.0f) ? "yes (1 is lost!)" : "no");
        printf("0.1 + 0.2 == 0.3 (double) ? %s\n", (0.1 + 0.2 == 0.3) ? "yes" : "no");
    }

    printf("\n=== 16-bit unsigned meter register rollover ===\n");
    {
        unsigned short r = 65535;
        printf("r = 65535;  r = r + 1;  -> r = ");
        r = r + 1;
        printf("%u   (wraps modulo 65536)\n", r);

        unsigned short prev = 65530, curr = 4;          /* meter went 65530 -> 65535 -> 0 -> 4 */
        unsigned short units16 = (unsigned short)(curr - prev);
        long units_if = (curr >= prev) ? (long)curr - prev : 65536L - prev + curr;
        printf("prev=%u curr=%u : naive curr-prev (as int) = %d\n", prev, curr, curr - prev);
        printf("                : (unsigned short)(curr-prev) = %u  (modular arithmetic gives correct units)\n", units16);
        printf("                : 65536 - prev + curr         = %ld\n", units_if);
        printf("Total energy one 16-bit register can show: %ld units before wrapping\n", 65535L + 1);
    }

    printf("\n=== double rupees vs integer paise: 20,000,000 additions ===\n");
    {
        volatile double dsum = 0.0; volatile long psum = 0;
        clock_t t0 = clock();
        for (long i = 0; i < 20000000L; ++i) dsum += 0.01;
        clock_t t1 = clock();
        for (long i = 0; i < 20000000L; ++i) psum += 1;
        clock_t t2 = clock();
        printf("double: sum of 20,000,000 x 0.01 = %.6f (exact answer 200000.000000)   time %.3f s\n",
               dsum, (double)(t1 - t0) / CLOCKS_PER_SEC);
        printf("long  : sum of 20,000,000 x 1 paise = %ld paise = Rs %ld.%02ld exactly   time %.3f s\n",
               psum, psum / 100, psum % 100, (double)(t2 - t1) / CLOCKS_PER_SEC);
        printf("Space: double = %zu bytes, long = %zu bytes per amount\n", sizeof(double), sizeof(long));
    }
    return 0;
}
