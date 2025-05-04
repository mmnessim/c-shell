#include <stdio.h>
#include <string.h>
#include "utils.h"
#include <unistd.h>

void repl();

int main() {
    repl();
    return 0;
}

void repl() {
    char cwd[256];

    while (1) {
        getcwd(cwd, sizeof(cwd));
        printf("c-shell:%s $ ", cwd);
        char input[256];

        fgets(input, 256, stdin);
        input[strcspn(input, "\n")] = 0;

        struct ParsedInput p = parse(input, sizeof(input));
        printf("Command: %s Flag: %s Argument:%s\n", p.command, p.flag, p.argument);

        if (strcmp(p.command, "") == 0) {
            continue;
        } else if (strcmp(p.command, "help") == 0) {
            help();
        } else if (strcmp(p.command, "cat") == 0) {
            cat(p.argument);
        } else if (strcmp(p.command, "ls") == 0) {
            ls(p.argument);
        }
    }
}
