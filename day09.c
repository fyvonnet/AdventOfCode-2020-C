#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <pcre.h>
#include <toylib.h>

#define PREAMBLE_LEN    25


typedef array_type(long) array_long;

long *make_sums(array_long numbers, int start)
{
    long *line = malloc(PREAMBLE_LEN * sizeof(long));

    for (int i = 0; i < PREAMBLE_LEN; i++)
        line[i] = array_ref(numbers, start) + array_ref(numbers, start + i);

    return line;
}


int sum_found(long num, long **sums, int start)
{
    for (int i = start; i < start + PREAMBLE_LEN; i++)
        for (int j = 0; j < PREAMBLE_LEN; j++)
            if (num == sums[i][j])
                return 1;

    return 0;
}


int main()
{

    char *buffer = NULL; size_t n = 0;
    long **sums;
    int invalid_number;
    array_long numbers;

    array_init(numbers);

    FILE *fp = fopen("inputs/day09", "r");
    while (getline (&buffer, &n, fp) != EOF) {
        long num;
        fscanf(fp, "%lu", &num);
        array_add(numbers, long, num);
    }
    fclose(fp);

    sums = malloc(array_size(numbers) * sizeof(long *));

    for (int i = 0; i < PREAMBLE_LEN; i++)
        sums[i] = make_sums(numbers, i);

    for (int i = PREAMBLE_LEN; i < array_size(numbers); i++) {
        long n = array_ref(numbers, i);
        if (!sum_found(n, sums, i - PREAMBLE_LEN)) {
            invalid_number = n;
            printf("%lu\n", n);
            break;
        }
        sums[i] = make_sums(numbers, i);
    }

    for (int i = 0; i < array_size(numbers); i++) {
        int sum = array_ref(numbers, i);
        for (int j = i + 1; j < array_size(numbers) - 1 && sum < invalid_number; j++) {
            sum += array_ref(numbers, j);
            if (sum == invalid_number) {
                long min, max;
                min = max = array_ref(numbers, i);
                for (int k = i + 1; k <= j; k++) {
                    long n = array_ref(numbers, k);
                    if (n > max) max = n;
                    if (n < min) min = n;
                }
                printf("%lu\n", min + max);
                exit(EXIT_SUCCESS);
            }
        }
    }

    return(EXIT_FAILURE);
}

