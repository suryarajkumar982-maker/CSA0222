#include <stdio.h>
int main(void)
{
    int units = 100          /* missing semicolon -> COMPILE-TIME (syntax) error */
    printf("%d\n", units);
    return 0;
}
