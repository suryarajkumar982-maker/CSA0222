# Rural Electricity Co-operative - Billing Program (C)

Student: R. Surya  |  Register No.: 1924260114C  |  SIMATS Engineering - C Programming Lab, Assignment 1, Q1

## Project layout
| File | Purpose | Task |
|------|---------|------|
| tariff.h | constants, macros, enums, structs, include guard, DBG(), prototypes | 3,4,5 |
| billing.c | parse_reading, compute_units, slab charges (if-else + array), generate_bill | 1-3,9 |
| main.c | batch driver (stdin) | 9 |
| task2_types.c | sizeof, limits.h/float.h, float drift, 16-bit rollover, double vs paise | 2 |
| task3_const.c | #define vs const vs enum; RATE2 = 230 | 3 |
| task4_macros.c, macro_expand.c | macro safety, gcc -E, macro vs inline timing | 4 |
| task5_debug.c, noguard.h, noguard_demo.c | include guard, -DDEBUG, <> vs "" | 5 |
| task6_io.c, task6_wrong.c, task6_compare.c | scanf/printf, wrong specifiers, scanf vs strtol | 6 |
| task7_conv.c | conversions and precision | 7 |
| run_task8.sh, err_*.c, warn.c, warn_fixed.c | compilation stages, errors, warnings | 8 |
| test_slabs.c, inputs/tests_input.txt, inputs/five_bills.txt | testing, slabs vs array | 9 |
| task10_scale.c | float/int/long/long long at 4,000 / 60,000 / 800,000 bills | 10 |
| smart_meter.c, tariff.cfg, inputs/readings_day.csv | smart-meter redesign | 12 |
| run_all.sh | compiles and runs everything; results in outputs/ | all |

## Build and run
    gcc -Wall -Wextra -o billing main.c billing.c
    ./billing < inputs/tests_input.txt
    gcc -Wall -Wextra -DRATE2_PAISE=230 -o billing_r230 main.c billing.c   # Task 3 tariff change
    gcc -Wall -Wextra -DDEBUG -o t5_debug task5_debug.c billing.c          # Task 5 debugging
    gcc -Wall -Wextra -o smart_meter smart_meter.c && ./smart_meter tariff.cfg   # Task 12
    ./run_all.sh && ./run_task8.sh                                          # everything

## Assumptions
Telescopic slabs 100 @ 150 paise, next 100 @ 200 paise, rest @ 300 paise; fixed charge 5000/15000/2500 paise
(Domestic/Commercial/Agriculture); all money in integer paise; 16-bit unsigned meter register; district = 800,000 households.
Tested with gcc 13.3 on Ubuntu 24.04 (LP64).  On Windows sizeof(long) = 4.
