#include <stdio.h>
long energy_charge_paise(long units);      /* declared, but billing.c is NOT linked */
int main(void) { printf("%ld\n", energy_charge_paise(150)); return 0; }
