#include <stdlib.h>
#include <stdio.h>
#include <toylib.h>

typedef array_type(int) int_array;

int main()
{
    FILE *fp = fopen("inputs/day01", "r");
    int entry;
    int_array input;
    array_init(input);
    int a, b, c;

    while ((fscanf(fp, "%d", &entry)) != EOF)
        array_add(input, int, entry);

    fclose(fp);


    a = 0; b = 1;

    while (array_ref(input, a) + array_ref(input, b) != 2020) {
        b++;
        if (b == array_size(input)) {
            a++;
            b = a + 1;
        }
    }

    printf("%d\n", array_ref(input, a) * array_ref(input, b));


    a = 0; b = 1; c = 2;

    while (array_ref(input, a) + array_ref(input, b) + array_ref(input, c) != 2020) {
        c++;
        if (c == array_size(input)) {
            b++;
            if (b == array_size(input) - 1) {
                a++;
                b = a + 1;
            }
            c = b + 1;
        }

    }

    printf("%d\n", array_ref(input, a) * array_ref(input, b) * array_ref(input, c));


    exit(EXIT_SUCCESS);
}

