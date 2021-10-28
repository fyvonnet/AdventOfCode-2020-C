#include <stdlib.h>
#include <stdio.h>
#include <toylib.h>

typedef array_type(int) array_int;

int compar_int(const void *a, const void *b)
{
    return *((int *)a) - *((int *)b);
}

int main()
{
    array_int input;
    array_init(input);
    int entry;

    array_add(input, int, 0); // charging outlet

    FILE *fp = fopen("inputs/day10", "r");

    while ((fscanf(fp, "%d", &entry)) != EOF)
        array_add(input, int, entry);

    fclose(fp);

    array_sort(input, int, compar_int);

    array_add(input, int, 3 + array_ref(input, array_size(input) - 1)); // device

    int diff1 = 0, diff3 = 0;

    for (int i = 0; i < array_size(input) - 1; i++)
        switch (array_ref(input, 1 + i) - array_ref(input, i)) {
            case 1: diff1++; break;
            case 3: diff3++; break;
        }

    printf("%d\n", diff1 * diff3);

    return(EXIT_SUCCESS);
}

