#include "tariff.h"
long demo(long units) { return SQUARE(units + 1) + MAX(units, 100) + RUPEES_TO_PAISE(2.5); }
long rate(void) { return RATE2_PAISE; }
