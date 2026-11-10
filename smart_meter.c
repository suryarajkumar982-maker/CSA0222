/* Task 12 - smart-meter billing: Wh readings every 15 min, 64-bit totals, runtime tariff, peak pricing */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>

typedef struct {
    int64_t slab1_wh, slab2_wh;             /* slab limits in Wh              */
    int64_t rate1, rate2, rate3;            /* paise per kWh                  */
    int     peak_start, peak_end, peak_pct; /* hours [start,end), surcharge % */
    int64_t fixed_paise;
} Tariff;

static int load_tariff(const char *path, Tariff *t)
{
    FILE *fp = fopen(path, "r"); char line[128], key[64]; long long v; int found = 0;
    if (!fp) { perror("tariff file"); return -1; }
    while (fgets(line, sizeof line, fp)) {
        if (line[0] == '#' || sscanf(line, "%63s %lld", key, &v) != 2) continue;
        if      (!strcmp(key, "SLAB1_LIMIT_KWH"))        { t->slab1_wh = v * 1000; ++found; }
        else if (!strcmp(key, "SLAB2_LIMIT_KWH"))        { t->slab2_wh = v * 1000; ++found; }
        else if (!strcmp(key, "RATE1_PAISE_PER_KWH"))    { t->rate1 = v; ++found; }
        else if (!strcmp(key, "RATE2_PAISE_PER_KWH"))    { t->rate2 = v; ++found; }
        else if (!strcmp(key, "RATE3_PAISE_PER_KWH"))    { t->rate3 = v; ++found; }
        else if (!strcmp(key, "PEAK_START_HOUR"))        { t->peak_start = (int)v; ++found; }
        else if (!strcmp(key, "PEAK_END_HOUR"))          { t->peak_end = (int)v; ++found; }
        else if (!strcmp(key, "PEAK_SURCHARGE_PERCENT")) { t->peak_pct = (int)v; ++found; }
        else if (!strcmp(key, "FIXED_CHARGE_PAISE"))     { t->fixed_paise = v; ++found; }
    }
    fclose(fp);
    return found == 9 ? 0 : -1;
}

/* cost of `wh` watt-hours when `cum` Wh were already consumed, in (paise x Wh) units */
static int64_t slab_cost_pwh(const Tariff *t, int64_t cum, int64_t wh)
{
    int64_t cost = 0;
    const int64_t upto[3] = { t->slab1_wh, t->slab2_wh, INT64_MAX };
    const int64_t rate[3] = { t->rate1, t->rate2, t->rate3 };
    for (int i = 0; i < 3 && wh > 0; ++i) {
        if (cum >= upto[i]) continue;
        int64_t room = upto[i] - cum, take = wh < room ? wh : room;
        cost += take * rate[i]; cum += take; wh -= take;
    }
    return cost;
}

int main(int argc, char **argv)
{
    const char *cfg = argc > 1 ? argv[1] : "tariff.cfg";
    Tariff t;
    if (load_tariff(cfg, &t) != 0) { fprintf(stderr, "bad tariff file %s\n", cfg); return 1; }
    printf("Loaded tariff from %s: slabs %" PRId64 "/%" PRId64 " Wh, rates %" PRId64 "/%" PRId64 "/%" PRId64
           " paise/kWh, peak %02d:00-%02d:00 +%d%%, fixed %" PRId64 " paise\n",
           cfg, t.slab1_wh, t.slab2_wh, t.rate1, t.rate2, t.rate3, t.peak_start, t.peak_end, t.peak_pct, t.fixed_paise);

    /* read one day of readings */
    int hour[96], n = 0; int64_t wh[96]; char line[64];
    FILE *fp = fopen("inputs/readings_day.csv", "r");
    if (!fp) { perror("readings"); return 1; }
    while (fgets(line, sizeof line, fp) && n < 96) {
        int h, m; long long w;
        if (line[0] == '#' || sscanf(line, "%d:%d,%lld", &h, &m, &w) != 3) continue;
        hour[n] = h; wh[n] = w; ++n;
    }
    fclose(fp);
    printf("Readings per day = %d, per year = %d (365 x %d)\n", n, 365 * n, n);

    /* bill one 30-day month, interval by interval (chronological slab allocation) */
    int64_t cum = 0, peak_wh = 0, normal_pwh = 0, peak_pwh = 0;
    for (int day = 0; day < 30; ++day)
        for (int i = 0; i < n; ++i) {
            int is_peak = hour[i] >= t.peak_start && hour[i] < t.peak_end;
            int64_t c = slab_cost_pwh(&t, cum, wh[i]);
            if (is_peak) { peak_pwh += c; peak_wh += wh[i]; } else normal_pwh += c;
            cum += wh[i];
        }
    int64_t scaled = normal_pwh * 100 + peak_pwh * (100 + t.peak_pct);       /* paise x Wh x 100 */
    int64_t energy = (scaled + 50000) / 100000;                               /* /1000 (Wh->kWh), /100 (%), rounded */
    int64_t total  = energy + t.fixed_paise;
    printf("\n-- Monthly bill (30 days = %d readings) --\n", 30 * n);
    printf("Total consumption : %" PRId64 " Wh (%.3f kWh)   [16-bit register would wrap %" PRId64 " times]\n", cum, cum / 1000.0, cum / 65536);
    printf("Peak consumption  : %" PRId64 " Wh   Off-peak: %" PRId64 " Wh\n", peak_wh, cum - peak_wh);
    printf("Energy charge     : %" PRId64 " paise   Fixed: %" PRId64 " paise   TOTAL: %" PRId64 " paise = Rs %" PRId64 ".%02" PRId64 "\n",
           energy, t.fixed_paise, total, total / 100, total % 100);
    /* same month WITHOUT peak surcharge, for evidence */
    int64_t flat = (normal_pwh + peak_pwh + 500) / 1000 + t.fixed_paise;
    printf("Same month without peak rule: %" PRId64 " paise -> peak rule adds %" PRId64 " paise\n", flat, total - flat);

    /* Impact analysis: largest possible values (assumed maximum load 10 kW per household) */
    const int64_t MAX_W = 10000, READINGS = 35040, HOUSEHOLDS = 800000, MAXRATE = 300;
    int64_t max_wh_interval = MAX_W / 4;                       /* 15 min = 1/4 hour */
    int64_t max_year_wh     = max_wh_interval * READINGS;
    int64_t max_district_wh = max_year_wh * HOUSEHOLDS;
    int64_t max_pwh_house   = max_year_wh * MAXRATE * (100 + t.peak_pct);   /* paise x Wh x 100 */
    printf("\n-- Largest possible values (10 kW max load, %" PRId64 " households) --\n", HOUSEHOLDS);
    printf("Max Wh per 15-min reading      : %" PRId64 "   (16-bit max 65535 -> fits)\n", max_wh_interval);
    printf("Max Wh per household per year  : %" PRId64 "   (uint32 max 4294967295 -> %s)\n", max_year_wh, max_year_wh > 4294967295LL ? "OVERFLOW" : "fits uint32, NOT 16-bit");
    printf("Max Wh for whole district/year : %" PRId64 "   (uint32 max 4294967295 -> %s)\n", max_district_wh, max_district_wh > 4294967295LL ? "needs 64-bit" : "fits");
    printf("Max intermediate paise*Wh*100  : %" PRId64 "   (int32 max 2147483647 -> %s)\n", max_pwh_house, max_pwh_house > 2147483647LL ? "needs 64-bit" : "fits");
    printf("INT64_MAX                      : %" PRId64 "\n", (int64_t)INT64_MAX);
    return 0;
}
