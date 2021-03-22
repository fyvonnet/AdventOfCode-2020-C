#include <stdlib.h>
#include <stdio.h>
#include <toylib.h>

typedef array_type(int) int_array;


int search (int_array input, int index, int n_entries, int sum, int product)
{
    if (n_entries == 0) {
        if (sum == 2020)
            return product;
        else
            return -1;
    }
    else {
        for (int i = index ; i < array_size(input); i++) {
            int entry = array_ref(input, i);
            int ret = search(input, index + 1, n_entries - 1, sum + entry, product * entry);
            if (ret != -1) return ret;
        }
        return -1;
    }
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

