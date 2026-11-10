/* Task 9 - compare fixed slab conditions (if-else) with array+loop; boundary assertions; timing */
#include <stdio.h>
#include <time.h>
#include "tariff.h"
int main(void)
{
    int mismatches = 0;
    for (long u = 0; u <= 1000; ++u)
        if (energy_charge_paise(u) != energy_charge_paise_array(u)) { ++mismatches; printf("MISMATCH at %ld\n", u); }
    printf("Compared if-else vs array/loop for units 0..1000 -> mismatches = %d\n", mismatches);
    long chk[][2] = { {0, 0}, {100, 15000}, {101, 15200}, {200, 35000}, {201, 35300} };
    for (size_t i = 0; i < sizeof chk / sizeof chk[0]; ++i)
        printf("units=%-4ld expected energy=%-6ld if-else=%-6ld array=%-6ld %s\n", chk[i][0], chk[i][1],
               energy_charge_paise(chk[i][0]), energy_charge_paise_array(chk[i][0]),
               (energy_charge_paise(chk[i][0]) == chk[i][1] && energy_charge_paise_array(chk[i][0]) == chk[i][1]) ? "PASS" : "FAIL");
    volatile long sink = 0; clock_t t = clock();
    for (long r = 0; r < 20000; ++r) for (long u = 0; u < 1000; ++u) sink += energy_charge_paise(u);
    double t1 = (double)(clock() - t) / CLOCKS_PER_SEC; t = clock();
    for (long r = 0; r < 20000; ++r) for (long u = 0; u < 1000; ++u) sink += energy_charge_paise_array(u);
    double t2 = (double)(clock() - t) / CLOCKS_PER_SEC;
    printf("Time for 20,000,000 bill calculations: if-else = %.3f s, array+loop = %.3f s\n", t1, t2);
    return 0;
}
