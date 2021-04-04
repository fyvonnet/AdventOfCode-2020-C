#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <pcre.h>
#include <toylib.h>

#define OVECMAX 15


typedef struct { int qt; char *name; } contained_bags;
typedef array_type(char *) array_string;

char *get_match_string(char *str, int a, int b)
{
    size_t len = b - a;
    char *match = malloc(len + 1);

    memcpy(match, str + a, len);
    match[len] = '\0';

    return match;
}

int get_match_int(char *str, int a, int b)
{
    size_t len = b - a;
    char match[len + 1];

    memcpy(match, str + a, len);
    match[len] = '\0';

    return atoi(match);
}

int contains_shiny_gold(char *bag, toytree *bags_tree, toytree *set, toyqueue *q)
{
    toyqueue_reset(q);
    toyqueue_enqueue(q, bag);

    while (!toyqueue_empty(q)) {
        contained_bags *cbs = toytree_search(bags_tree, toyqueue_dequeue(q));
        char *name;

        if (cbs)
            for (int i = 0; (name = cbs[i].name); i++) {
                if ((toytree_search(set, name)) || (!strcmp(name, "shiny gold"))) {
                    toytree_insert(set, name, (void *)1);
                    return 1;
                }
                else
                    toyqueue_enqueue(q, name);
            }
    }
    return 0;
}


int main()
{
    static char pattern[] = "^(.*) bags contain (.*)\\.$";
    static char pattern2[] = "(\\d+) (.*) bag";
    const char *err_msg;
    int err;
    int offsets[OVECMAX];
    char *buffer = NULL;
    size_t n = 0;

    pcre *re  = pcre_compile(pattern,  0, &err_msg, &err, NULL);
    pcre *re2 = pcre_compile(pattern2, 0, &err_msg, &err, NULL);
    FILE *fp = fopen("inputs/day07", "r");
    toytree *bags_tree = toytree_new((int(*)(const void *, const void *))strcmp);
    toytree *set = toytree_new((int(*)(const void *, const void *))strcmp);

    array_string all_bags;
    array_init(all_bags);

    while (getline(&buffer, &n, fp) != EOF) {
        pcre_exec(re, NULL, buffer, strlen(buffer), 0, 0, offsets, OVECMAX);
        char *bag_name = get_match_string(buffer, offsets[2], offsets[3]);
        char *bags = get_match_string(buffer, offsets[4], offsets[5]);

        if (strcmp(bags, "no other bags")) {
            char *token = strtok(bags, ",");
            contained_bags *cbs = malloc(6 * sizeof(contained_bags));
            int i;

            array_add(all_bags, char*, bag_name);

            for (i = 0; token != NULL; i++) {
                pcre_exec(re2, NULL, token, strlen(token), 0, 0, offsets, OVECMAX);
                cbs[i].qt   = get_match_int(token, offsets[2], offsets[3]);
                cbs[i].name = get_match_string(token, offsets[4], offsets[5]);
                token = strtok(NULL, ",");
            }

            cbs[i].name = NULL;
            toytree_insert(bags_tree, bag_name, cbs);
        }
    }

    int count = 0;
    toyqueue *q = toyqueue_new(1000);
    for (int i = 0; i < array_size(all_bags); i++)
        count += contains_shiny_gold(array_ref(all_bags, i), bags_tree, set, q);

    printf("%d\n", count);

    toyqueue_reset(q);
    count = -1;
    toyqueue_enqueue(q, (void *)1);
    toyqueue_enqueue(q, (void *)"shiny gold");

    while (!toyqueue_empty(q)) {
        int qt = (uintptr_t)toyqueue_dequeue(q);
        char *name = (char *)toyqueue_dequeue(q);

        count += qt;
        contained_bags *bags = toytree_search(bags_tree, name);

        if (bags)
            for (int i = 0; bags[i].name; i++) {
                toyqueue_enqueue(q, (void *)(uintptr_t)(qt * bags[i].qt));
                toyqueue_enqueue(q, (void *)bags[i].name);
            }
    }

    printf("%d\n", count);

    return(EXIT_SUCCESS);
}

