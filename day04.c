#include <stdlib.h>
#include <stdio.h>
#include <string.h>


void read_until(char* input, char *output, int *i, char char_stop)
{
    int j = 0;

    while ((input[*i] != char_stop) && (input[*i] != '\n'))
        output[j++] = input[(*i)++];
    output[j] = '\0';
    (*i)++;
}

int main()
{
    char *line = NULL;
    size_t n = 0;
    int fields_count = 0;
    int count_complete = 0;
    int read_ok; 

    char expected_fields[7][4] = { "byr", "iyr", "eyr", "hgt", \
        "hcl", "ecl", "pid" };

    FILE *fp = fopen("inputs/day04", "r");

    do {
        read_ok = (getline(&line, &n, fp) != -1);
        if (read_ok && (strlen(line) > 1)) {
            int i = 0;
            while (line[i] != '\0') {
                char key[20], val[20];

                read_until(line, key, &i, ':');
                read_until(line, val, &i, ' ');

                for (int i = 0; i < 7; i++)
                    if (!strcmp(key, expected_fields[i])) {
                        fields_count++;
                        break;
                    }

            }
        }
        else {
            if (fields_count == 7)
                count_complete++;
            fields_count = 0;
        }
    } while (read_ok);

    printf("%d\n", count_complete);

    exit(EXIT_SUCCESS);
}
