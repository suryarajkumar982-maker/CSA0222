/* main.c - batch billing driver: reads lines "household category prev curr rollover(y/n)" from stdin */
#include <stdio.h>
#include <string.h>
#include "tariff.h"

int main(void)
{
    char line[256], ps[32], cs[32], rs[32];
    long hh; int cat, ok = 0, bad = 0, lineno = 0;
    long grand = 0;

    printf("%-6s %-5s %-11s %-12s %7s %12s %10s %12s\n",
           "Line", "HH", "Category", "Reading", "Units", "Energy", "Fixed", "Total");
    while (fgets(line, sizeof line, stdin)) {
        Customer c; Bill b; int st;
        ++lineno;
        if (line[0] == '#' || line[0] == '\n') continue;
        if (sscanf(line, "%ld %d %31s %31s %31s", &hh, &cat, ps, cs, rs) != 5) {
            printf("%-6d ERROR: malformed line -> %s", lineno, line); ++bad; continue;
        }
        c.household_no = hh; c.category = cat;
        c.rollover = (rs[0] == 'y' || rs[0] == 'Y');
        if ((st = parse_reading(ps, &c.prev)) != BILL_OK || (st = parse_reading(cs, &c.curr)) != BILL_OK) {
            printf("%-6d ERROR (HH %ld): %s  [prev=%s curr=%s]\n", lineno, hh, status_text(st), ps, cs); ++bad; continue;
        }
        if ((st = generate_bill(&c, &b)) != BILL_OK) {
            printf("%-6d ERROR (HH %ld): %s  [prev=%u curr=%u rollover=%c]\n",
                   lineno, hh, status_text(st), c.prev, c.curr, c.rollover ? 'y' : 'n'); ++bad; continue;
        }
        printf("%-6d %-5ld %-11s %5u->%-6u %7ld ", lineno, hh, CATEGORY_NAME[cat], c.prev, c.curr, b.units);
        printf("%12ld %10ld %12ld   (", b.energy_paise, b.fixed_paise, b.total_paise);
        print_money(b.total_paise); printf(")\n");
        grand += b.total_paise; ++ok;
    }
    printf("\nValid bills: %d   Rejected: %d   Grand total: ", ok, bad);
    print_money(grand); printf("  (%ld paise)\n", grand);
    return 0;
}
