/* billing.c - core billing logic */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include "tariff.h"

const long FIXED_CHARGE_PAISE[CAT_COUNT] = { 5000L, 15000L, 2500L };   /* Rs 50 / 150 / 25 */
const char *const CATEGORY_NAME[CAT_COUNT] = { "Domestic", "Commercial", "Agriculture" };

/* Validate & convert a text reading to 16-bit register value */
int parse_reading(const char *s, unsigned short *out)
{
    const char *p = s;
    char *end;
    long v;
    if (s == NULL || *s == '\0') return ERR_FORMAT;
    if (*p == '-') {                         /* negative number?          */
        for (++p; *p; ++p) if (!isdigit((unsigned char)*p)) return ERR_FORMAT;
        return (p == s + 1) ? ERR_FORMAT : ERR_NEGATIVE;
    }
    for (; *p; ++p) if (!isdigit((unsigned char)*p)) return ERR_FORMAT;  /* letters etc. */
    errno = 0;
    v = strtol(s, &end, 10);
    if (errno == ERANGE || v > METER_MAX) return ERR_RANGE;
    *out = (unsigned short)v;
    return BILL_OK;
}

/* units consumed, handling 16-bit rollover */
int compute_units(unsigned short prev, unsigned short curr, int rollover, long *units)
{
    if (rollover) {
        if (curr >= prev) return ERR_ROLLOVER_FLAG;       /* cannot have wrapped */
        *units = METER_MODULUS - prev + curr;
    } else {
        if (curr < prev) return ERR_DECREASE;             /* reading went down   */
        *units = (long)curr - prev;
    }
    return BILL_OK;
}

/* telescopic slabs using macros (if-else version) */
long energy_charge_paise(long units)
{
    if (units <= SLAB1_LIMIT)
        return units * RATE1_PAISE;
    else if (units <= SLAB2_LIMIT)
        return SLAB1_LIMIT * RATE1_PAISE + (units - SLAB1_LIMIT) * RATE2_PAISE;
    else
        return SLAB1_LIMIT * RATE1_PAISE
             + (SLAB2_LIMIT - SLAB1_LIMIT) * RATE2_PAISE
             + (units - SLAB2_LIMIT) * RATE3_PAISE;
}

/* same tariff, table-driven (array + loop) */
long energy_charge_paise_array(long units)
{
    static const struct { long upto; long rate; } slab[] = {
        { SLAB1_LIMIT, RATE1_PAISE }, { SLAB2_LIMIT, RATE2_PAISE }, { LONG_MAX, RATE3_PAISE }
    };
    long total = 0, lower = 0;
    for (size_t i = 0; i < sizeof slab / sizeof slab[0] && units > lower; ++i) {
        long top = (units < slab[i].upto) ? units : slab[i].upto;
        total += (top - lower) * slab[i].rate;
        lower = slab[i].upto;
    }
    return total;
}

int generate_bill(const Customer *c, Bill *b)
{
    int st;
    if (c->household_no < 1 || c->household_no > MAX_HOUSEHOLD_NO) return ERR_HOUSEHOLD;
    if (c->category < 0 || c->category >= CAT_COUNT)               return ERR_CATEGORY;
    st = compute_units(c->prev, c->curr, c->rollover, &b->units);
    if (st != BILL_OK) return st;
    b->energy_paise = energy_charge_paise(b->units);
    b->fixed_paise  = FIXED_CHARGE_PAISE[c->category];
    b->total_paise  = b->energy_paise + b->fixed_paise;
    DBG("hh=%ld prev=%u curr=%u units=%ld energy=%ld fixed=%ld total=%ld",
        c->household_no, c->prev, c->curr, b->units, b->energy_paise, b->fixed_paise, b->total_paise);
    return BILL_OK;
}

const char *status_text(int s)
{
    switch (s) {
    case BILL_OK:           return "OK";
    case ERR_FORMAT:        return "reading is not a number (letters/symbols)";
    case ERR_NEGATIVE:      return "negative reading";
    case ERR_RANGE:         return "reading outside 0..65535";
    case ERR_DECREASE:      return "current < previous without rollover";
    case ERR_ROLLOVER_FLAG: return "rollover flagged but current >= previous";
    case ERR_CATEGORY:      return "invalid category (0=Dom,1=Com,2=Agri)";
    case ERR_HOUSEHOLD:     return "invalid household number";
    default:                return "unknown error";
    }
}

void print_money(long paise) { printf("Rs %ld.%02ld", paise / 100, paise % 100); }
