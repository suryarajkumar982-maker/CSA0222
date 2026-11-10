/* Task 6 - formatted input/output with scanf / printf */
#include <stdio.h>
int main(void)
{
    int hh; unsigned int u; long l; long long ll; float f; double d; char c; char s[32];
    int n = scanf("%d %u %ld %lld %f %lf %c %31s", &hh, &u, &l, &ll, &f, &d, &c, s);
    if (n != 8) { printf("INVALID INPUT: scanf converted only %d of 8 fields -> rejecting\n", n); return 1; }
    printf("%%d  int           : %d\n", hh);
    printf("%%u  unsigned int  : %u\n", u);
    printf("%%ld long          : %ld\n", l);
    printf("%%lld long long    : %lld\n", ll);
    printf("%%f  float         : %f   (%%.2f -> %.2f)\n", f, f);
    printf("%%lf double (scanf): %lf  (printf uses %%f/%%lf for double)\n", d);
    printf("%%c  char          : %c\n", c);
    printf("%%s  string        : %s\n", s);
    long paise = (long)u;   /* amount stored in paise */
    printf("Amount %ld paise  ->  Rs %ld and %02ld paise  (Rs %ld.%02ld)\n", paise, paise / 100, paise % 100, paise / 100, paise % 100);
    return 0;
}
