#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <pcre.h>
#include <toylib.h>

#define OVECMAX 15


typedef enum { CONTAINS, DOESNT_CONTAIN, UNKNOWN } bag_status;
typedef intptr_t bag_name;
typedef array_type(char*) array_string;
typedef struct {
    bag_name name;
    bag_name cnames[6];
    int cqts[6];
    bag_status status;
} bags_array_elm;

bags_array_elm *bags_array;
int input_len;
bag_name shiny_gold_hash;


int compare_bags(const void *a, const void *b)
{
    bag_name x = ((bags_array_elm *)a)->name;
    bag_name y = ((bags_array_elm *)b)->name;

    return (x > y) - (x < y);
}

bag_name get_match_hash(char *str, int a, int b)
{
    bag_name hash = 5381;

    for (int i = a; i < b; i++)
        hash = (hash * 33) + str[i];

    return hash;
}

int get_match_int(char *str, int a, int b)
{
    int n = 0;

    for (int i = a; i < b; i++)
        n = (n * 10) + (str[i] - '0');

    return n;
}

int get_index(bag_name name)
{
    int low = 0;
    int hi  = input_len - 1;

    for (;;) {
        int mid = low + round((hi - low) / 2);

        bags_array_elm *bag = bags_array + mid;

        if (bag->name < name)
            low = mid + 1;
        else if (bag->name > name)
            hi = mid - 1;
        else
            return mid;
    }
}

bag_status contains_shiny_gold(bag_name name) 
{
    bags_array_elm *bag = bags_array + get_index(name);

    if (bag->status == UNKNOWN) {
        for (int i = 0; bag->cnames[i] != 0; i++)
            if ((bag->cnames[i] == shiny_gold_hash) || (contains_shiny_gold(bag->cnames[i]) == CONTAINS)) {
                bag->status = CONTAINS;
                return CONTAINS;
            }
        bag->status = DOESNT_CONTAIN;

    }

    return bag->status;
}

int main()
{
    array_string input;
    array_init(input);

    char shiny_gold_str[] = "shiny gold";
    shiny_gold_hash = get_match_hash(shiny_gold_str, 0, strlen(shiny_gold_str));

    FILE *fp = fopen("inputs/day07", "r");
    size_t r = 0;

    while (r != -1) {
        char *buffer = NULL;
        size_t n = 0;
        r = getline(&buffer, &n, fp);
        if (r != -1)
            array_add(input, char*, buffer);
    }

    fclose(fp);

    input_len = array_size(input);
    bags_array = (bags_array_elm *)malloc(input_len * sizeof(bags_array_elm));


    static char pattern[] = "^(.*) bags contain (.*)\\.$";
    static char pattern2[] = "(\\d+) (.+) bag";
    const char *err_msg;
    int err;
    int offsets[OVECMAX];
    pcre *re  = pcre_compile(pattern,  0, &err_msg, &err, NULL);
    pcre *re2 = pcre_compile(pattern2, 0, &err_msg, &err, NULL);

    for (int i = 0; i < input_len; i++) {
        char *line = array_ref(input, i);
        pcre_exec(re, NULL, line, strlen(line), 0, 0, offsets, OVECMAX);
        bag_name name = get_match_hash(line, offsets[2], offsets[3]);
        bags_array_elm *elm = bags_array + i;
        elm->name = name;
        char *bags = line + offsets[4];

        if (strncmp(bags, "no other bags", 13)) {
            char *token = strtok(bags, ",");
            int j = 0;

            while (token != NULL) {
                pcre_exec(re2, NULL, token, strlen(token), 0, 0, offsets, OVECMAX);
                elm->cqts[j]   = get_match_int(token, offsets[2], offsets[3]);
                elm->cnames[j] = get_match_hash(token, offsets[4], offsets[5]);
                token = strtok(NULL, ",");
                j++;
            }

            elm->cnames[j] = 0;
            elm->status = UNKNOWN;
        }
        else {
            elm->status = DOESNT_CONTAIN;
            elm->cnames[0] = 0;
        }
    }

    qsort(bags_array, input_len, sizeof(bags_array_elm), compare_bags);

    int counter = 0;

    for (int i = 0; i < input_len; i++)
        if (contains_shiny_gold(bags_array[i].name) == CONTAINS)
            counter++;

    printf("%d\n", counter);

    toystack *stack = toystack_new(1000);
    toystack_push(stack, (void *)1);
    toystack_push(stack, (void *)shiny_gold_hash);
    counter = -1;

    while (!toystack_empty(stack)) {
        int bag_index = get_index((bag_name)toystack_pop(stack));
        int multiplier = (int)(uintptr_t)toystack_pop(stack);
        counter += multiplier;
        bags_array_elm *bag = bags_array + bag_index;
        for (int i = 0; bag->cnames[i] != 0; i++) {
            toystack_push(stack, (void *)(multiplier * bag->cqts[i]));
            toystack_push(stack, (void *)bag->cnames[i]);
        }
    }

    printf("%d\n", counter);


    return(EXIT_SUCCESS);
}

