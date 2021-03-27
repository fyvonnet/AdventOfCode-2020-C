#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


int is_between(char *str, int min, int max)
{
    int num = atoi(str);
    return (num >= min) && (num <= max);
}

int (fields_valid(char fields[][20]))
{
    if (!is_between(fields[0], 1920, 2002)) return 0;
    if (!is_between(fields[1], 2010, 2020)) return 0;
    if (!is_between(fields[2], 2020, 2030)) return 0;

    {
        char c, str[10], *unit, *hgt;
        int j;

        hgt = fields[3];
        for (j = 0; isdigit(c = hgt[j]); j++)
            str[j] = c;
        str[j] = '\0';
        unit = hgt + j;
        if (!((!strcmp(unit, "cm") && is_between(str, 150, 193)) || (!strcmp(unit, "in") && is_between(str, 59, 76))))
            return 0;
    }

    {
        char *hcl = fields[4];

        if (hcl[0] != '#') return 0;
        if (strlen(hcl + 1) != 6) return 0;
        for (int i = 1; i < 7; i++) if (!isxdigit(hcl[i])) return 0;
    }

    {
        char *ecl = fields[5];
        char colors[][4] = { "amb", "blu", "brn", "gry", "grn", "hzl", "oth" };
        int i;

        for (i = 0; (i < 7) && (strcmp(ecl, colors[i])); i++) ;
        if (i == 7) return 0;
    }

    {
        char *pid = fields[6];

        if (strlen(pid) != 9) return 0;
        for (int i = 0; i < 9; i++) if (!isdigit(pid[i])) return 0;
    }

    return 1;
}

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
    int count_valid = 0;
    int read_ok; 
    char key[20], val[20];
    char values[7][20];


    char expected_fields[7][4] = { "byr", "iyr", "eyr", "hgt", \
        "hcl", "ecl", "pid" };

    FILE *fp = fopen("inputs/day04", "r");

    do {
        read_ok = (getline(&line, &n, fp) != -1);
        if (read_ok && (strlen(line) > 1)) {
            int i = 0;
            while (line[i] != '\0') {
                read_until(line, key, &i, ':');
                read_until(line, val, &i, ' ');

                for (int i = 0; i < 7; i++)
                    if (!strcmp(key, expected_fields[i])) {
                        fields_count++;
                        strcpy(values[i], val);
                        break;
                    }

            }
        }
        else {
            if (fields_count == 7) {
                count_complete++;
                if (fields_valid(values))
                    count_valid++;
            }
            fields_count = 0;
        }
    } while (read_ok);

    printf("%d\n", count_complete);
    printf("%d\n", count_valid);


    exit(EXIT_SUCCESS);
}
