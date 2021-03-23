#include <stdlib.h>
#include <stdio.h>
#include <toylib.h>

typedef array_type(int) int_array;


int search (int_array input, int depth)
{
    struct {
        int prod;
        int sum;
        int index;
    } data = { 1, 0, 0 }, saves[depth];

    int layer = 0, len = array_size(input);

    for(;;) {
        if ((layer == depth) && (data.sum == 2020))
            return data.prod;

        if ((layer == depth) || (data.index == len)) {
            layer--;
            data = saves[layer];
            data.index++;
        }
        else {
            saves[layer] = data;
            data.prod *= array_ref(input, data.index);
            data.sum  += array_ref(input, data.index);
            data.index++;
            layer++;
        }
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


    printf("%d\n", search(input, 2));
    printf("%d\n", search(input, 3));


    exit(EXIT_SUCCESS);
}

