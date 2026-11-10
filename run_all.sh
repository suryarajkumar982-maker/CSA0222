#!/bin/bash
# Compile and run every task; all output is stored in outputs/
cd "$(dirname "$0")"; O=outputs; mkdir -p $O
W="-Wall -Wextra"
{
echo '$ gcc -Wall -Wextra -o billing main.c billing.c'; gcc $W -o billing main.c billing.c && echo '(compiled with no warnings)'
echo '$ ./billing < inputs/tests_input.txt'; ./billing < inputs/tests_input.txt
} > $O/task9_tests.txt 2>&1
{ echo '$ gcc -Wall -Wextra -o test_slabs test_slabs.c billing.c -O2'; gcc $W -O2 -o test_slabs test_slabs.c billing.c; echo '$ ./test_slabs'; ./test_slabs
  echo; echo '$ ./billing < inputs/five_bills.txt   (five bills, compare with manual table)'; ./billing < inputs/five_bills.txt; } > $O/task9_slabs.txt 2>&1
{ echo '$ gcc -Wall -Wextra -o task2_types task2_types.c'; gcc $W -O0 -o task2_types task2_types.c; echo '$ ./task2_types'; ./task2_types; } > $O/task2.txt 2>&1
{ echo '--- Run 1: original RATE2_PAISE = 200 ---'; echo '$ gcc -Wall -Wextra -o task3 task3_const.c billing.c'; gcc $W -o task3 task3_const.c billing.c; echo '$ ./task3'; ./task3
  echo; echo '--- Run 2: RATE2_PAISE changed to 230 (no source edit: -DRATE2_PAISE=230) ---'; echo '$ gcc -Wall -Wextra -DRATE2_PAISE=230 -o task3_r230 task3_const.c billing.c'; gcc $W -DRATE2_PAISE=230 -o task3_r230 task3_const.c billing.c; echo '$ ./task3_r230'; ./task3_r230
  echo; echo '--- Five bills re-run with RATE2_PAISE=230 ---'; gcc $W -DRATE2_PAISE=230 -o billing_r230 main.c billing.c; ./billing_r230 < inputs/five_bills.txt; } > $O/task3.txt 2>&1
{ echo '$ gcc -Wall -Wextra -O0 -o task4_macros task4_macros.c'; gcc $W -O0 -o task4_macros task4_macros.c; echo '$ ./task4_macros   (-O0)'; ./task4_macros
  echo; echo '$ gcc -O2 -o task4_macros_O2 task4_macros.c ; ./task4_macros_O2   (-O2)'; gcc -O2 -o task4_macros_O2 task4_macros.c; ./task4_macros_O2 | tail -3; } > $O/task4_run.txt 2>&1
{ echo '$ cat macro_expand.c'; cat macro_expand.c; echo; echo '$ gcc -E macro_expand.c | tail -n 4'; gcc -E macro_expand.c | tail -n 4; } > $O/task4_expand.txt 2>&1
{ echo '$ gcc -Wall -Wextra -o t5_nodebug task5_debug.c billing.c'; gcc $W -o t5_nodebug task5_debug.c billing.c; echo '$ ./t5_nodebug'; ./t5_nodebug
  echo; echo '$ gcc -Wall -Wextra -DDEBUG -o t5_debug task5_debug.c billing.c'; gcc $W -DDEBUG -o t5_debug task5_debug.c billing.c; echo '$ ./t5_debug'; ./t5_debug
  echo; echo '$ ./t5_nodebug --debug   (runtime debugging via command-line flag)'; ./t5_nodebug --debug
  echo; echo '$ ls -l t5_nodebug t5_debug   (executable size, bytes)'; ls -l t5_nodebug t5_debug | awk '{print $5, $9}'
  echo; echo '$ size t5_nodebug t5_debug'; size t5_nodebug t5_debug
  echo; echo '--- include guard: header WITHOUT guard included twice ---'; echo '$ gcc noguard_demo.c'; gcc noguard_demo.c 2>&1 | head -8
  echo; echo '--- #include <file> vs #include "file" ---'; printf '#include <tariff.h>\nint main(void){return 0;}\n' > angle.c
  echo '$ gcc -c angle.c            (angle brackets: system paths only)'; gcc -c angle.c 2>&1 | head -4
  echo '$ gcc -I. -c angle.c        (-I. adds current dir to the <> search path)'; gcc -I. -c angle.c && echo 'compiled OK'
  printf '#include "tariff.h"\nint main(void){return 0;}\n' > quote.c; echo '$ gcc -c quote.c            (quotes: current dir first, then system paths)'; gcc -c quote.c && echo 'compiled OK'
  echo; echo '--- system include search path (excerpt) ---'; echo | gcc -E -Wp,-v -xc - 2>&1 | sed -n '/#include <...> search/,/End of search/p'; } > $O/task5.txt 2>&1
{ echo '$ gcc -Wall -Wextra -o task6_io task6_io.c'; gcc $W -o task6_io task6_io.c
  echo '$ cat inputs/input_valid.txt'; cat inputs/input_valid.txt; echo '$ ./task6_io < inputs/input_valid.txt'; ./task6_io < inputs/input_valid.txt
  echo; echo '$ cat inputs/input_invalid.txt'; cat inputs/input_invalid.txt; echo '$ ./task6_io < inputs/input_invalid.txt'; ./task6_io < inputs/input_invalid.txt
  echo; echo '--- wrong format specifiers: compile with -Wall ---'; echo '$ gcc -Wall -Wformat -o task6_wrong task6_wrong.c'; gcc -Wall -Wformat -o task6_wrong task6_wrong.c 2>&1 | grep -E 'warning' | sed 's/^task6_wrong.c://'
  echo '$ ./task6_wrong'; ./task6_wrong
  echo; echo '--- scanf vs fgets+sscanf vs strtol ---'; gcc $W -o task6_compare task6_compare.c; echo '$ ./task6_compare < inputs/compare_lines.txt'; ./task6_compare < inputs/compare_lines.txt; } > $O/task6.txt 2>&1
{ echo '$ gcc -Wall -Wextra -o task7_conv task7_conv.c -lm   (compiler warnings below are intentional)'; gcc $W -o task7_conv task7_conv.c -lm 2>&1 | grep warning | sed 's/^task7_conv.c://'; echo '$ ./task7_conv'; ./task7_conv; } > $O/task7.txt 2>&1
{ echo '$ gcc -Wall -Wextra -o task10_scale task10_scale.c billing.c -O2'; gcc $W -O2 -o task10_scale task10_scale.c billing.c; echo '$ ./task10_scale'; ./task10_scale; } > $O/task10.txt 2>&1
{ echo '$ gcc -Wall -Wextra -o smart_meter smart_meter.c'; gcc $W -o smart_meter smart_meter.c; echo '$ ./smart_meter tariff.cfg'; ./smart_meter tariff.cfg
  echo; echo '--- change tariff WITHOUT recompiling: RATE3 300 -> 350 in tariff2.cfg ---'; sed 's/^RATE3_PAISE_PER_KWH .*/RATE3_PAISE_PER_KWH        350/' tariff.cfg > tariff2.cfg; echo '$ ./smart_meter tariff2.cfg'; ./smart_meter tariff2.cfg | sed -n '1p;/Energy charge/p'; } > $O/task12.txt 2>&1
echo done
