#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main()
{
    FILE *fp = fopen("inputs/day03", "r");
    int product = 1;
    char line[64];
    int rights[] = { 1, 3, 5, 7, 1 };
    int downs[]  = { 1, 1, 1, 1, 2 };
    int counts[] = { 0, 0, 0, 0, 0 };

    for (int row = 0; fscanf(fp, "%s", line) != EOF; row++)
        for (int s = 0; s < 5; s++)
            if (((row % downs[s]) == 0) && (line[((row * rights[s]) / downs[s]) % strlen(line)] == '#'))
                counts[s]++;

    for (int s = 0; s < 5; s++)
        product *= counts[s];

    printf("%d\n%d\n", counts[1], product);

    exit(EXIT_SUCCESS);
}

