/* Task 3 - constants: #define vs const vs enum; billing with macros; RATE2_PAISE change */
#include <stdio.h>
#include "tariff.h"

/* const-variable versions of the same tariff (for comparison only) */
static const long K_RATE1 = 150L, K_RATE2 = 200L, K_RATE3 = 300L;
/* enum version of the slab limits */
enum SlabLimit { E_SLAB1 = 100, E_SLAB2 = 200 };

static long bill_using_macros(long units)
{
    if (units <= SLAB1_LIMIT) return units * RATE1_PAISE + FIXED_CHARGE_PAISE[CAT_DOMESTIC];
    if (units <= SLAB2_LIMIT) return SLAB1_LIMIT * RATE1_PAISE + (units - SLAB1_LIMIT) * RATE2_PAISE + FIXED_CHARGE_PAISE[CAT_DOMESTIC];
    return SLAB1_LIMIT * RATE1_PAISE + (SLAB2_LIMIT - SLAB1_LIMIT) * RATE2_PAISE + (units - SLAB2_LIMIT) * RATE3_PAISE + FIXED_CHARGE_PAISE[CAT_DOMESTIC];
}

int main(void)
{
    static const long sample[] = { 0, 50, 100, 150, 200, 250, 300 };
    printf("Tariff in force: RATE1=%ld RATE2=%ld RATE3=%ld paise/unit, fixed(domestic)=%ld paise\n",
           (long)RATE1_PAISE, (long)RATE2_PAISE, (long)RATE3_PAISE, FIXED_CHARGE_PAISE[CAT_DOMESTIC]);
    printf("%-8s %-12s %-10s\n", "Units", "Total paise", "Total Rs");
    for (size_t i = 0; i < sizeof sample / sizeof sample[0]; ++i) {
        long p = bill_using_macros(sample[i]);
        printf("%-8ld %-12ld %ld.%02ld\n", sample[i], p, p / 100, p % 100);
    }
    printf("\n-- macro / const / enum comparison --\n");
    printf("RATE2 via #define = %ld, via const = %ld (same value: %s)  <- const needs its own manual edit\n", (long)RATE2_PAISE, K_RATE2, (long)RATE2_PAISE == K_RATE2 ? "yes" : "NO - differs");
    printf("const object has an address & occupies memory: &K_RATE1 = %p, sizeof = %zu\n", (const void *)&K_RATE1, sizeof K_RATE1);
    printf("#define has no address/size of its own (it is replaced by the literal %ldL before compilation)\n", (long)RATE1_PAISE);
    printf("enum value E_SLAB2 = %d, sizeof(enum SlabLimit) = %zu\n", E_SLAB2, sizeof(enum SlabLimit));
    (void)K_RATE3; (void)E_SLAB1;
    switch (100) { case E_SLAB1: puts("enum/#define usable as case labels (compile-time constants)"); break; default: break; }
    /* case K_RATE1: would be a compile ERROR in C - const int is not a constant expression */
    return 0;
}
