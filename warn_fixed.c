#include <stdio.h>
int count_over(int n, const int *units, int limit)
{
    int c = 0;                                       /* initialised */
    for (int i = 0; i < n; ++i)                      /* same signedness */
        if (units[i] > limit) c++;                   /* parameter now used */
    return c;
}
int main(void) { int u[3] = {50, 150, 250}; printf("%d\n", count_over(3, u, 100)); return 0; }
