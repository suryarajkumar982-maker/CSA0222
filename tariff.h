/* tariff.h - constants, macros, types and prototypes (Tasks 3,4,5)  */
#ifndef TARIFF_H            /* include guard: prevents double inclusion */
#define TARIFF_H

#include <stdio.h>

/* ---- Tariff constants (all money in PAISE; 100 paise = 1 rupee) ---- */
#ifndef RATE1_PAISE         /* overridable: gcc -DRATE2_PAISE=230       */
#define RATE1_PAISE 150L    /* units   1..100  : Rs 1.50 / unit         */
#endif
#ifndef RATE2_PAISE
#define RATE2_PAISE 200L    /* units 101..200  : Rs 2.00 / unit         */
#endif
#ifndef RATE3_PAISE
#define RATE3_PAISE 300L    /* units  >200     : Rs 3.00 / unit         */
#endif
#define SLAB1_LIMIT     100L
#define SLAB2_LIMIT     200L
#define METER_MAX       65535L      /* 16-bit unsigned register max      */
#define METER_MODULUS   65536L      /* register wraps modulo 2^16        */
#define MAX_HOUSEHOLD_NO 999999L

/* enum constant set: customer categories (type-safe-ish, debugger-visible) */
enum Category { CAT_DOMESTIC = 0, CAT_COMMERCIAL = 1, CAT_AGRICULTURE = 2, CAT_COUNT };

/* const objects (defined in billing.c) */
extern const long FIXED_CHARGE_PAISE[CAT_COUNT];
extern const char *const CATEGORY_NAME[CAT_COUNT];

/* ---- Safe macros ---- */
#define RUPEES_TO_PAISE(r)  ((long)((r) * 100.0 + 0.5))
#define MAX(a, b)           (((a) > (b)) ? (a) : (b))
#define SQUARE(x)           ((x) * (x))

/* ---- Conditional debugging (compile with gcc -DDEBUG) ---- */
#ifdef DEBUG
#define DBG(fmt, ...) fprintf(stderr, "[DEBUG] %s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#else
#define DBG(fmt, ...) ((void)0)
#endif

/* ---- Data types ---- */
typedef struct {
    long           household_no;
    int            category;       /* enum Category */
    unsigned short prev, curr;     /* 16-bit meter register values */
    int            rollover;       /* 1 = meter wrapped past 65535 */
} Customer;

typedef struct {
    long units;
    long energy_paise;
    long fixed_paise;
    long total_paise;
} Bill;

/* ---- Error codes ---- */
enum BillStatus { BILL_OK = 0, ERR_FORMAT, ERR_NEGATIVE, ERR_RANGE, ERR_DECREASE,
                  ERR_ROLLOVER_FLAG, ERR_CATEGORY, ERR_HOUSEHOLD };

/* ---- Prototypes ---- */
int         parse_reading(const char *s, unsigned short *out);
int         compute_units(unsigned short prev, unsigned short curr, int rollover, long *units);
long        energy_charge_paise(long units);          /* if-else slabs   */
long        energy_charge_paise_array(long units);    /* array + loop    */
int         generate_bill(const Customer *c, Bill *b);
const char *status_text(int status);
void        print_money(long paise);

#endif /* TARIFF_H */
