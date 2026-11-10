/* Task 5 - include guards, conditional debugging (#ifdef DEBUG), runtime vs compile-time debug */
#include <stdio.h>
#include <string.h>
#include "tariff.h"
#include "tariff.h"        /* included twice on purpose: include guard makes this harmless */

int main(int argc, char **argv)
{
    int runtime_debug = (argc > 1 && strcmp(argv[1], "--debug") == 0);
    Customer c = { 3001, CAT_DOMESTIC, 1000, 1180, 0 };
    Bill b;
    int st = generate_bill(&c, &b);          /* DBG() inside prints only if compiled with -DDEBUG */
    DBG("main: generate_bill returned %d", st);
    if (runtime_debug) fprintf(stderr, "[RUNTIME-DEBUG] status=%d units=%ld total=%ld\n", st, b.units, b.total_paise);
    printf("Bill for HH %ld: units=%ld total=", c.household_no, b.units);
    print_money(b.total_paise); printf("\n");
    return 0;
}
