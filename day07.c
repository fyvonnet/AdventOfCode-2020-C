#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <pcre.h>
#include <toylib.h>

#define OVECMAX 15


typedef struct { int qt; uintptr_t name; } contained_bags;
typedef array_type(uintptr_t) array_int;


int compare (const void *a, const void *b)
{
    return (uintptr_t)a - (uintptr_t)b;
}

char *get_match_string(char *str, int a, int b)
{
    size_t len = b - a;
    char *match = malloc(len + 1);

    memcpy(match, str + a, len);
    match[len] = '\0';

    return match;
}

uintptr_t get_match_hash(char *str, int a, int b)
{
    uintptr_t hash = 5381;

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

int contains_shiny_gold(uintptr_t bag, toytree *bags_tree, toytree *set, toyqueue *q, uintptr_t shiny_gold_hash)
{
    toyqueue_reset(q);
    toyqueue_enqueue(q, (void *)bag);

    while (!toyqueue_empty(q)) {
        contained_bags *cbs = toytree_search(bags_tree, toyqueue_dequeue(q));
        uintptr_t name;

        if (cbs)
            for (int i = 0; (name = cbs[i].name); i++) {
                int found = (uintptr_t)toytree_search(set, (void *)name);
                if ((found == 1) || (name == shiny_gold_hash)) {
                    toytree_insert(set, (void *)name, (void *)1);
                    return 1;
                }
                else if (found != -1)
                    toyqueue_enqueue(q, (void *)name);
            }
    }
    toytree_insert(set, (void *)bag, (void *)-1);
    return 0;
}


int main()
{
    static char pattern[] = "^(.*) bags contain (.*)\\.$";
    static char pattern2[] = "(\\d+) (.+) bag";
    const char *err_msg;
    int err;
    int offsets[OVECMAX];
    char *buffer = NULL;
    size_t n = 0;
    char shiny_gold_str[] = "shiny gold";
    uintptr_t shiny_gold_hash = get_match_hash(shiny_gold_str, 0, strlen(shiny_gold_str));

    pcre *re  = pcre_compile(pattern,  0, &err_msg, &err, NULL);
    pcre *re2 = pcre_compile(pattern2, 0, &err_msg, &err, NULL);
    FILE *fp = fopen("inputs/day07", "r");
    toytree *bags_tree = toytree_new(compare);
    toytree *set       = toytree_new(compare);

    array_int all_bags;
    array_init(all_bags);

    while (getline(&buffer, &n, fp) != EOF) {
        pcre_exec(re, NULL, buffer, strlen(buffer), 0, 0, offsets, OVECMAX);
        uintptr_t bag_name = get_match_hash(buffer, offsets[2], offsets[3]);
        char *bags = get_match_string(buffer, offsets[4], offsets[5]);

        if (strcmp(bags, "no other bags")) {
            char *token = strtok(bags, ",");
            contained_bags *cbs = malloc(6 * sizeof(contained_bags));
            int i = 0;

            array_add(all_bags, uintptr_t, bag_name);

            for (i = 0; token != NULL; i++) {
                pcre_exec(re2, NULL, token, strlen(token), 0, 0, offsets, OVECMAX);

                cbs[i].qt   = get_match_int(token, offsets[2], offsets[3]);
                cbs[i].name = get_match_hash(token, offsets[4], offsets[5]);
                token = strtok(NULL, ",");
            }

            cbs[i].name = 0;
            toytree_insert(bags_tree, (void *)bag_name, cbs);
        }
        buffer = NULL; n = 0;
    }

    int count = 0;
    toyqueue *q = toyqueue_new(1000);

    for (int i = 0; i < array_size(all_bags); i++)
        count += contains_shiny_gold(array_ref(all_bags, i), bags_tree, set, q, shiny_gold_hash);

    printf("%d\n", count);

    toyqueue_reset(q);
    count = -1;
    toyqueue_enqueue(q, (void *)1);
    toyqueue_enqueue(q, (void *)shiny_gold_hash);

    while (!toyqueue_empty(q)) {
        int qt = (uintptr_t)toyqueue_dequeue(q);
        uintptr_t name = (uintptr_t)toyqueue_dequeue(q);

        count += qt;
        contained_bags *bags = toytree_search(bags_tree, (void *)name);

        if (bags)
            for (int i = 0; bags[i].name != 0; i++) {
                toyqueue_enqueue(q, (void *)(uintptr_t)(qt * bags[i].qt));
                toyqueue_enqueue(q, (void *)bags[i].name);
            }
    }

    printf("%d\n", count);

    return(EXIT_SUCCESS);
}

