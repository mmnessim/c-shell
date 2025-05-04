#include <stdio.h>
#include <string.h>
#include "utils.h"

void repl();

int main() {
    //if (argc < 2) {
    //    printf("useage c-shell <filename>\n");
    //    return 0;
    //}
//
    //char* filename = argv[1];
//
    //cat(filename);

    ls(".");
    repl();
    return 0;
}

void repl() {
    while (1) {
        printf("$ ");
        char input[256];

        fgets(input, 256, stdin);
        input[strcspn(input, "\n")] = 0;

        if (strncmp("cat", input, strlen("cat")) == 0) {
            cat("../main.c");
        }
    }
}
