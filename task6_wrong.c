/* Task 6 - incorrect format specifiers (undefined behaviour; compile with -Wall -Wformat) */
#include <stdio.h>
int main(void)
{
    long long big = 5000000000LL;
    printf("printf(\"%%d\", 3.14)        -> %d\n", 3.14);
    printf("printf(\"%%f\", 42)          -> %f\n", 42);
    printf("printf(\"%%d\", 5000000000LL)-> %d   (high bits lost)\n", big);
    printf("printf(\"%%u\", -1)          -> %u\n", -1);
    printf("printf(\"%%c\", 321)         -> %c   (321 mod 256 = 65)\n", 321);
    return 0;
}
