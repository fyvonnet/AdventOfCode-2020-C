#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pcre.h>

#define OVECMAX 15


char *get_match_string(char *str, int a, int b)
{
    size_t len = b - a;
    char *match = malloc(len + 1);

    memcpy(match, str + a, len);
    match[len] = '\0';

    return match;
}

int main()
{
    static char *pattern = "^(\\d+)-(\\d+) (\\w): (\\w+)$";
    const char *err_msg;
    int err;
    size_t n = 0;
    int offsets[OVECMAX];
    char *buffer = NULL;
    int valid1 = 0, valid2 = 0;

    pcre *re = pcre_compile(pattern, 0, &err_msg, &err, NULL);
    FILE *fp = fopen("inputs/day02", "r");

    while (getline(&buffer, &n, fp) != EOF) {
        pcre_exec(re, NULL, buffer, strlen(buffer), 0, 0, offsets, OVECMAX);

        int a = atoi(get_match_string(buffer, offsets[2], offsets[3]));
        int b = atoi(get_match_string(buffer, offsets[4], offsets[5]));
        char c = *(buffer + offsets[6]);
        char *password = get_match_string(buffer, offsets[8], offsets[9]);
        int count = 0;
        int char_a = password[a - 1];
        int char_b = password[b - 1];

        for (int i = 0; i < strlen(password); i++) 
            if (password[i] == c)
                count++;

        if ((count >= a) && (count <= b))
            valid1++;

        if (((char_a == c) && (char_b != c)) || ((char_a != c) && (char_b == c)))
            valid2++;
    }

    printf("%d\n", valid1);
    printf("%d\n", valid2);

    exit(EXIT_SUCCESS);
}
