#include <stdlib.h>
#include <stdio.h>
#include <toylib.h>

typedef array_type(int) int_array;


int search (int_array input, int index, int entries, int sum, int product)
{
    if (entries == 0) { if (sum == 2020) return product; else return -1; }

    if (index == array_size(input)) return -1;

    int ret = search(input, index + 1, entries - 1, sum + array_ref(input, index), product * array_ref(input, index));
    if (ret == -1) return search(input, index + 1, entries, sum, product);
    else return ret;
}


int main()
{
    FILE *fp = fopen("inputs/day01", "r");
    int entry;
    int_array input;
    array_init(input);

    while ((fscanf(fp, "%d", &entry)) != EOF)
        array_add(input, int, entry);

    fclose(fp);


    printf("%d\n", search(input, 0, 2, 0, 1));
    printf("%d\n", search(input, 0, 3, 0, 1));


    exit(EXIT_SUCCESS);
}

