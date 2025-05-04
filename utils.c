#include "utils.h"
#include <dirent.h>

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
