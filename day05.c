#include <stdlib.h>
#include <stdio.h>
#include <math.h>


int convert_bin(char *str)
{
    int sum = 0;

    for (int i = 0; i < 10; i++)
        if ((str[i] == 'B') || (str[i] == 'R'))
            sum += pow(2, 9 - i);

    return sum;
}

int main()
{
    int max = 0;
    FILE *fp = fopen("inputs/day05", "r");
    char str[9];
    char seats[1024];

    for (int i = 0; i < 1024; i++)
        seats[i] = 0;

    while (fscanf(fp, "%s", str) != -1) {
        int id = convert_bin(str);
        seats[id] = 1;
        if (id > max) max = id;
    }

    printf("%d\n", max);

    for (int i = 0; i < 1024; i++)
        if (!seats[i] && (seats[i - 1]) && (seats[i + 1])) {
            printf("%d\n", i);
            break;
        }

    return(EXIT_SUCCESS);
}
