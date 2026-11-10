/* Task 6 - scanf vs fgets()+sscanf() vs fgets()+strtol() */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
int main(void)
{
    char line[64];
    printf("%-14s | %-26s | %-34s\n", "input line", "sscanf(\"%d\") (ret,val)", "strtol (value, leftover, errno)");
    while (fgets(line, sizeof line, stdin)) {
        int v = 0, r = sscanf(line, "%d", &v);
        char *end; errno = 0; long t = strtol(line, &end, 10);
        int junk = 0; for (char *p = end; *p; ++p) if (*p != ' ' && *p != '\n') junk = 1;
        char shown[40]; int k = 0; for (char *p = line; *p && *p != '\n' && k < 38; ++p) shown[k++] = *p; shown[k] = 0;
        printf("%-14s | ret=%d val=%-11d | val=%-11ld %s errno=%d\n", shown, r, v, t,
               (end == line) ? "NO-DIGITS" : junk ? "TRAILING-JUNK" : "clean", errno);
    }
    return 0;
}
