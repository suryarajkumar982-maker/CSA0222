#include <stdio.h>
int count_over(int n, const int *units, int limit)   /* 'limit' unused */
{
    int c;                                           /* used uninitialised */
    size_t i;
    int unused_var = 5;                              /* unused variable    */
    for (i = 0; i < n; ++i)                          /* signed/unsigned compare */
        if (units[i] > 100) c++;
    return c;
}
int main(void) { int u[3] = {50, 150, 250}; printf("%d\n", count_over(3, u, 200)); return 0; }
