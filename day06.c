#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main()
{
    size_t read_ok, n = 0;
    char q1[26], q2[26], *line = NULL;
    int npersons = 0, count1 = 0, count2 = 0;
    FILE *fp = fopen("inputs/day06", "r");

    for (int i = 0; i < 26; i++)
        q1[i] = q2[i] = 0;

    do {
        read_ok = (getline(&line, &n, fp) != -1);

        if (read_ok && (strlen(line) > 1)) {
            npersons++;
            for (int i = 0; i < strlen(line); i++) {
                int j = line[i] - 'a';
                q1[j] = 1; q2[j]++;
            }
        }
        else {
            for (int i = 0; i < 26; i++) {
                count1 += q1[i];
                if (q2[i] == npersons)
                    count2++;
            }

            npersons = 0;
            for (int i = 0; i < 26; i++)
                q1[i] = q2[i] = 0;
        }
    } while (read_ok);

    printf("%d\n%d\n", count1, count2);

    exit(EXIT_SUCCESS);
}
