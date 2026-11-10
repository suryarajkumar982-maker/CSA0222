/* Task 4 - macro safety and macro-vs-function */
#include <stdio.h>
#include <time.h>
#include "tariff.h"

#define BAD_SQUARE(x) x * x            /* WRONG: no parentheses */
#define BAD_MAX(a, b) a > b ? a : b    /* WRONG: no parentheses */
static inline long max_inline(long a, long b) { return a > b ? a : b; }
__attribute__((noinline)) long sq_func(long x) { return x * x; }
static inline long sq_inline(long x) { return x * x; }

int main(void)
{
    printf("RUPEES_TO_PAISE(12.34) = %ld paise,  RUPEES_TO_PAISE(99.99) = %ld paise\n", RUPEES_TO_PAISE(12.34), RUPEES_TO_PAISE(99.99));
    printf("MAX(150, 230) = %d\n", MAX(150, 230));

    printf("\n-- SQUARE without parentheses --\n");
    printf("BAD_SQUARE(2+3)  expands to 2+3 * 2+3 = %d   (expected 25)\n", BAD_SQUARE(2 + 3));
    printf("SQUARE(2+3)      expands to ((2+3)*(2+3)) = %d\n", SQUARE(2 + 3));
    printf("BAD_MAX result inside expression: 10 + BAD_MAX(1,2) = %d (expected 12)\n", 10 + BAD_MAX(1, 2));

    printf("\n-- MAX(a,b) evaluates an argument twice --\n");
    {
        int i = 5, j = 3;
        int m = MAX(i++, j++);
        printf("macro   : MAX(i++, j++) -> m=%d, i=%d, j=%d  (i incremented TWICE)\n", m, i, j);
        i = 5; j = 3;
        long f = max_inline(i++, j++);
        printf("function: max_inline(i++, j++) -> m=%ld, i=%d, j=%d  (each argument evaluated once)\n", f, i, j);
    }

    printf("\n-- Speed: 300,000,000 iterations (volatile sink) --\n");
    {
        volatile long sink = 0; clock_t t;
        t = clock(); for (long i = 0; i < 300000000L; ++i) sink += SQUARE(i & 1023);
        printf("macro SQUARE        : %.3f s\n", (double)(clock() - t) / CLOCKS_PER_SEC);
        t = clock(); for (long i = 0; i < 300000000L; ++i) sink += sq_inline(i & 1023);
        printf("static inline       : %.3f s\n", (double)(clock() - t) / CLOCKS_PER_SEC);
        t = clock(); for (long i = 0; i < 300000000L; ++i) sink += sq_func(i & 1023);
        printf("noinline function   : %.3f s\n", (double)(clock() - t) / CLOCKS_PER_SEC);
    }
    return 0;
}
