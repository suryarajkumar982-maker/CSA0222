#!/bin/bash
cd "$(dirname "$0")"; mkdir -p stages outputs; O=outputs/task8.txt
{
echo '===== STAGE 1: PREPROCESSING  (gcc -E) ====='
echo '$ gcc -E main.c -o stages/main.i'; gcc -E main.c -o stages/main.i
echo '$ wc -l main.c stages/main.i'; wc -l main.c stages/main.i
echo '$ grep -c "^#define" stages/main.i   (macros are gone after preprocessing)'; grep -c '^#define' stages/main.i
echo '$ grep -n "RATE\|DBG\|TARIFF_H" stages/main.i | head -3   (no macro names remain)'; grep -n 'RATE\|DBG\|TARIFF_H' stages/main.i | head -3; echo '(no output = none left)'
echo '$ tail -n 6 stages/main.i   (main() body: #include expanded, comments removed)'; tail -n 6 stages/main.i | cut -c1-110
echo
echo '===== STAGE 2: COMPILATION TO ASSEMBLY (gcc -S) ====='
echo '$ gcc -S main.c -o stages/main.s'; gcc -S main.c -o stages/main.s
echo '$ head -n 12 stages/main.s'; head -n 12 stages/main.s
echo
echo '===== STAGE 3: ASSEMBLY TO OBJECT (gcc -c) ====='
echo '$ gcc -c main.c -o stages/main.o ; gcc -c billing.c -o stages/billing.o'; gcc -c main.c -o stages/main.o; gcc -c billing.c -o stages/billing.o
echo '$ file stages/main.o'; file stages/main.o
echo '$ nm stages/main.o | grep " U "   (U = undefined, resolved only at link time)'; nm stages/main.o | grep ' U '
echo '$ nm stages/billing.o | grep " T "   (T = functions defined here)'; nm stages/billing.o | grep ' T '
echo
echo '===== STAGE 4: LINKING ====='
echo '$ gcc stages/main.o stages/billing.o -o stages/billing_linked'; gcc stages/main.o stages/billing.o -o stages/billing_linked && echo 'linked OK'
echo '$ ./stages/billing_linked < inputs/five_bills.txt | tail -3'; ./stages/billing_linked < inputs/five_bills.txt | tail -3
echo
echo '===== COMPILE-TIME vs LINK-TIME vs RUNTIME ERRORS ====='
echo '$ gcc -o err_compile err_compile.c'; gcc -o err_compile err_compile.c 2>&1 | head -6
echo '$ gcc -o err_link err_link.c'; gcc -o err_link err_link.c 2>&1 | grep -v 'warning' | head -4
echo '$ gcc -o err_runtime err_runtime.c && ./err_runtime'; gcc -o err_runtime err_runtime.c && ./err_runtime; echo "exit status = $? (136 = 128 + SIGFPE)"
echo
echo '===== WARNINGS: gcc -Wall -Wextra -O2 warn.c ====='
echo '$ gcc -Wall -Wextra -O2 -c warn.c'; gcc -Wall -Wextra -O2 -c warn.c -o /dev/null 2>&1 | grep -E 'warning'  | sed 's/^warn.c://'
echo '--- after fixing (warn_fixed.c) ---'
echo '$ gcc -Wall -Wextra -O2 -o warn_fixed warn_fixed.c'; gcc -Wall -Wextra -O2 -o warn_fixed warn_fixed.c && echo '(no warnings)'; ./warn_fixed
echo
echo '===== SINGLE-FILE vs SEPARATE COMPILATION ====='
TIMEFORMAT='   real %3Rs'
echo '$ time gcc -o billing_single main.c billing.c   (everything recompiled)'; time gcc -o stages/billing_single main.c billing.c
echo '$ gcc -c billing.c ; gcc -c main.c ; gcc main.o billing.o   (initial separate build)'; gcc -c billing.c -o stages/billing.o; gcc -c main.c -o stages/main.o; gcc stages/main.o stages/billing.o -o stages/billing_sep
echo '(edit only main.c ...) $ time { gcc -c main.c ; gcc main.o billing.o }   (billing.o reused)'; time { gcc -c main.c -o stages/main.o; gcc stages/main.o stages/billing.o -o stages/billing_sep; }
} > $O 2>&1
