#include <stdio.h>
int main(void)
{
    volatile long units = 0, total = 5000;
    printf("average paise per unit = %ld\n", total / units);   /* compiles & links fine; crashes when run */
    return 0;
}
