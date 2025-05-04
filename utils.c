#include "utils.h"
#include <stdio.h>
#include <string.h>

int cat(char *filename) {
    FILE *fp;
    fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("Could not open file");
        return 1;
    }

    char line[MAX_LINE_LEN];
    while (fgets(line, MAX_LINE_LEN, fp) != NULL) {
        printf("%s", line);
    }

    fclose(fp);
    return 0;
}

#ifdef __unix__
int ls(char* path) {
    DIR *d;
    struct dirent *dir;
    d = opendir(path);

    if (d == NULL) {
        perror("Could not open directory");
        return 1;
    }

    while ((dir = readdir(d)) != NULL) {
        printf("%s\n", dir->d_name);
    }

    closedir(d);
    return 0;
}

#elif _WIN32 || _WIN64
int ls(char* path) {
    printf("Windows\n");
    WIN32_FIND_DATA ffd;
    LARGE_INTEGER filesize;
    char szDir[MAX_PATH];
    size_t arg_len;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    DWORD dwError = 0;

    StringCchCopy(szDir, MAX_PATH, path);
    StringCchCat(szDir, MAX_PATH, TEXT("\\*"));

    hFind = FindFirstFile(szDir, &ffd);

    if (INVALID_HANDLE_VALUE == hFind) {
      return dwError;
    }

    do {
        printf("  %s\n", ffd.cFileName);
    } while (FindNextFile(hFind, &ffd) != 0);

    FindClose(hFind);
    return dwError;
}
#endif

struct ParsedInput parse(char* raw_input, size_t len) {
    struct ParsedInput p;

    if (raw_input[0] == ' ') {
        p.argument = ""; // empty strings to avoid segfault
        p.command = "";
        p.flag = "";
        return p;
    }

    char* saveptr;
    char* tok = strtok_r(raw_input, " ", &saveptr);

    // Empty input, return empty struct
    if (tok == NULL) {
        p.argument = "";
        p.command = "";
        p.flag = "";
        return p;
    }

    // First word is always command
    p.command = tok;
    tok = strtok_r(NULL, " ", &saveptr);

    // Return early if only command is given
    if (tok == NULL) {
        p.argument = "";
        p.flag = "";
        return p;
    }

    // If second word begins with "-" parse it as a flag
    // Otherwise parse it as argument and return early
    if (strncmp("-", tok, 1) == 0) {
        p.flag = tok;
        p.argument = "";
    } else {
        p.argument = tok;
        p.flag = "";
        return p;
    }

    tok = strtok_r(NULL, " ", &saveptr);

    if (tok == NULL) {
        p.argument = "";
        return p;
    }

    // Will only be reached if three words are passed
    p.argument = tok;
    return p;
}
