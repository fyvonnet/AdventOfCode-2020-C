#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <pcre.h>
#include <toylib.h>


struct instr {
    enum {NOP, ACC, JMP} op;
    int arg; };

typedef array_type(struct instr) array_instrs;


int run_program(array_instrs instrs)
{
    struct instr in;
    int ptr = 0, acc = 0;
    char *visited = calloc(array_size(instrs), 1);

    for (;;) {
        if (visited[ptr]) {
            free(visited);
            return (- acc);
        }

        if (ptr >= array_size(instrs)) {
            free(visited);
            return acc;
        }

        visited[ptr] = 1;

        in = array_ref(instrs, ptr);
        switch(in.op) {
            case NOP:
                ptr++;
                break;
            case ACC:
                acc += in.arg;
                ptr++;
                break;
            case JMP:
                ptr += in.arg;
                break;
        }
    }
}

void op_switch(array_instrs instrs, int n)
{
    struct instr *ptr = &array_ref(instrs, n);
    if (ptr->op == NOP) ptr->op = JMP;
    else ptr->op = NOP;
}

int main()
{
    array_instrs instrs;
    struct instr in;
    size_t n = 0;
    char *buffer = NULL;

    array_init(instrs);

    FILE *fp = fopen("inputs/day08", "r");
    while (getline(&buffer, &n, fp) != EOF) {
        switch (buffer[0]) {
            case 'n':
                in.op = NOP;
                break;
            case 'a':
                in.op = ACC;
                break;
            case 'j':
                in.op = JMP;
                break;
        }
        in.arg = atoi(buffer + 4);
        array_add(instrs, array_instrs, in);
    }
    fclose(fp);

    printf("%d\n", - run_program(instrs));

    int i = 0;
    for(;;) {
        in = array_ref(instrs, i);
        if (in.op != ACC) {
            op_switch(instrs, i);
            int r = run_program(instrs);
            if (r > 0) {
                printf("%d\n", r);
                exit(EXIT_SUCCESS);
            }
            op_switch(instrs, i);
        }
        i++;
    }
}

