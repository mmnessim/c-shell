#ifndef _CSHELL_UTILS_H
#define _CSHELL_UTILS_H

#include <stdio.h>

#define MAX_LINE_LEN 256

struct ParsedInput {
    char* command;
    char* flag;
    char* argument;
};
struct ParsedInput parse(char* raw_input, size_t len);
int cat(char* filename);
int ls(char* path);

#ifdef __unix__
#include <dirent.h>

#elif _WIN32
#include <windows.h>
#include <strsafe.h>

#endif

#endif
