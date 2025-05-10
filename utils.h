#ifndef _CSHELL_UTILS_H
#define _CSHELL_UTILS_H

#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>


#define MAX_LINE_LEN 256

struct ParsedInput {
    char* command;
    char* flag;
    char* argument;
    int redirect;
    char* second_arg;
};
struct ParsedInput parse(char* raw_input, size_t len);
void help();
int cat(struct ParsedInput p);
int ls(struct ParsedInput p);
int cd(struct ParsedInput p);
int touch(struct ParsedInput p);
int rm(struct ParsedInput p);
int echo(struct ParsedInput p);

#ifdef __unix__
#include <dirent.h>

#elif _WIN32
#include <windows.h>
#include <strsafe.h>

#endif

#endif
