#include <stdio.h>
#include <string.h>
#include "utils.h"
#include <unistd.h>

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
    char cwd[256];

    while (1) {
        getcwd(cwd, sizeof(cwd));
        printf("%s c-shell$ ", cwd);
        char input[256];

        fgets(input, 256, stdin);
        input[strcspn(input, "\n")] = 0;

        struct ParsedInput p = parse(input, sizeof(input));
        printf("%s %s %s\n", p.command, p.flag, p.argument);

        if (strncmp("cat", input, strlen("cat")) == 0) {
            cat("../main.c");
        }
    }
}
