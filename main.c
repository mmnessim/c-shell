#include <stdio.h>
#include <string.h>
#include "utils.h"

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

        if (strcmp(p.command, "") == 0) {
            continue;
        } else if (strcmp(p.command, "help") == 0) {
            help();
        } else if (strcmp(p.command, "cat") == 0) {
            cat(p);
        } else if (strcmp(p.command, "ls") == 0) {
            ls(p);
        } else if (strcmp(p.command, "cd") == 0) {
            cd(p);
        } else if (strcmp(p.command, "touch") == 0) {
            touch(p);
        } else if (strcmp(p.command, "rm") == 0) {
            rm(p);
        } else if (strcmp(p.command, "echo") == 0) {
            echo(p);
        } else if (strcmp(p.command, "exit") == 0) {
            return;
        }
    }
}
