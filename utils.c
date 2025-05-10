#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Display help menu
void help() {
    printf("  useage <command> <optional flags> <arguments>\n");
    printf("  cat: display file contents\n");
    printf("    -n TODO display number lines\n");
    printf("  ls: display contents of a directory\n");
    printf("    -a TODO display hidden files and folders\n");
    printf("  touch: create a file\n");
    printf("  rm: remove a file\n");
    printf("  help: display this help menu\n\n");
    printf("Written by Mounir Nessim\nmmnessim@gmail.com\nhttps://github.com/mmnessim/c-shell\n");
}

// Simple implementation of cat builtin
// Displays file contents or error
// -n flag adds line numbers
// Returns 0 for success and 1 for error
int cat(struct ParsedInput p) {
    FILE *fp;
    fp = fopen(p.argument, "r");
    if (fp == NULL) {
        perror("Could not open file");
        return 1;
    }
    char line[MAX_LINE_LEN];

    if (p.redirect == 1) {
        FILE *newfile;
        newfile = fopen(p.second_arg, "w");

        if (newfile == NULL) {
            perror("Could not write to file");
            return 1;
        }

        if (strcmp(p.flag, "-n") == 0) {
            int line_no = 1;
            while (fgets(line, MAX_LINE_LEN, fp) != NULL) {
                fprintf(newfile, "%d: %s", line_no, line);
                line_no ++;
            }
        } else {
            while (fgets(line, MAX_LINE_LEN, fp) != NULL) {
                fprintf(newfile, "%s", line);
            }
        }

        fclose(newfile);
    }

    if (strcmp(p.flag, "-n") == 0) {
        int line_no = 1;
        while (fgets(line, MAX_LINE_LEN, fp) != NULL) {
            printf("%d: %s", line_no, line);
            line_no ++;
        }
    } else {
        while (fgets(line, MAX_LINE_LEN, fp) != NULL) {
            printf("%s", line);
        }
    }

    fclose(fp);
    return 0;
}

#ifdef __unix__
int ls(struct ParsedInput p) {
    DIR *d;
    struct dirent *dir;
    if (strcmp(p.argument, "") == 0) {
        d = opendir(".");
    } else {
        d = opendir(p.argument);
    }

    if (d == NULL) {
        perror("Could not open directory");
        return 1;
    }

    if (p.redirect == 1) {

        FILE *newfile;
        newfile = fopen(p.second_arg, "w");

        if (newfile == NULL) {
            perror("Could not write to file");
            return 1;
        };

        while ((dir = readdir(d)) != NULL) {
            fprintf(newfile, "  %s\n", dir->d_name);
        }

        fclose(newfile);
    } else {
        while ((dir = readdir(d)) != NULL) {
            if (strncmp(dir->d_name, ".", 1) == 0 &&
                    !(strstr(p.flag, "a") != NULL)) {
                continue;
            }
            printf("%s\n", dir->d_name);
        }
    }

    closedir(d);
    return 0;
}

#elif _WIN32 || _WIN64
int ls(struct ParsedInput p) {
    if (strcmp(p.argument, "") == 0) {
        p.argument = ".";
    }
    WIN32_FIND_DATA ffd;
    LARGE_INTEGER filesize;
    char szDir[MAX_PATH];
    size_t arg_len;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    DWORD dwError = 0;

    StringCchCopy(szDir, MAX_PATH, p.argument);
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

// Simple implementation of touch builtin
int touch(struct ParsedInput p) {
    FILE *fp;
    fp = fopen(p.argument, "w");

    if (fp == NULL) {
        perror("Could not create file");
        return 1;
    }

    fclose(fp);

    return 0;
}

int rm(struct ParsedInput p) {
    int res = remove(p.argument);

    if (res != 0) {
        perror("Could not delete file");
        return 1;
    }
    return 0;
}

int cd(struct ParsedInput p) {
    char *path = p.argument;
    if (chdir(path) != 0) {
        perror("failed to change directory");
        return 1;
    }
    return 0;
}

int echo(struct ParsedInput p) {
    printf("%s\n", p.argument);

    if (p.redirect == 1) {
        FILE *newfile;
        newfile = fopen(p.second_arg, "w");
        if (newfile == NULL) {
            perror("Could not write file");
            return 1;
        }

        fprintf(newfile, "%s", p.argument);

        fclose(newfile);
    }

    return 0;
}

struct ParsedInput parse(char* raw_input, size_t len) {
    struct ParsedInput p;
    p.argument = "";
    p.command = "";
    p.flag = "";
    p.redirect = 0;
    p.second_arg = "";

    if (raw_input[0] == ' ') {
        return p;
    }

    //// FIRST WORD
    char* saveptr;
    char* tok = strtok_r(raw_input, " ", &saveptr);

    // Empty input, return empty struct
    if (tok == NULL) {
        return p;
    }

    // First word is always command
    //// DEBUG
    //printf("tok1: %s\n", tok);
    p.command = tok;

    //// SECOND WORD
    tok = strtok_r(NULL, " ", &saveptr);
    if (tok == NULL) {
        return p;
    }
    //// DEBUG
    //printf("tok2: %s\n", tok);

    // If second word begins with "-" parse it as a flag
    // Otherwise parse it as argument and return early
    if (strncmp("-", tok, 1) == 0) {
        p.flag = tok;

        //// THIRD WORD
        tok = strtok_r(NULL, " ", &saveptr);
        if (tok == NULL) {
            return p;
        }
        //// DEBUG
        //printf("tok3: %s\n", tok);

        if (strncmp(tok, "\"", 1) == 0) {
            //// Capture input in quotes in third position
            char *string = (char *)malloc(100 * sizeof(char));
            strcpy(string, tok);
            memmove(string, string+1, strlen(string)); // Remove initial "
            strcat(string, " "); // Append space
            tok = strtok_r(NULL, "\"", &saveptr);
            if (tok == NULL) {
                return p;
            }
            strcat(string, tok);
            p.argument = string;
        } else {
            p.argument = tok;
        }

        //// FOURTH WORD
        tok = strtok_r(NULL, " ", &saveptr);
        if (tok == NULL) {
            return p;
        }

        if (strcmp(tok, ">>") == 0) {
            p.redirect = 1;
        }

        //// FIFTH WORD
        tok = strtok_r(NULL, " ", &saveptr);
        if (tok == NULL) {
            return p;
        }

        p.second_arg = tok;

        return p;

    } else if (strncmp(tok, "\"", 1) == 0) {
        //// Capture input in quotes in second position

        // Does this need to be freed at some point?
        char *string = (char *)malloc(100 * sizeof(char));
        strcpy(string, tok);
        memmove(string, string+1, strlen(string)); // Remove initial "
        strcat(string, " "); // Append space
        tok = strtok_r(NULL, "\"", &saveptr);
        if (tok == NULL) {
            return p;
        }
        strcat(string, tok);
        p.argument = string;

        //// Continue parsing
        //// THIRD WORD
        tok = strtok_r(NULL, " ", &saveptr);
        if (tok == NULL) {
            return p;
        }

        if (strcmp(tok, ">>") == 0) {
            p.redirect = 1;
        }

        //// FOURTH WORD
        tok = strtok_r(NULL, " ", &saveptr);
        if (tok == NULL) {
            return p;
        }
        //// DEBUG
        //printf("tok4: %s\n", tok);
        p.second_arg = tok;

    } else {
        p.argument = tok;

        //// THIRD WORD
        tok = strtok_r(NULL, " ", &saveptr);
        if (tok == NULL) {
            return p;
        }
        //// DEBUG
        //printf("tok3: %s\n", tok);

        if (strcmp(tok, ">>") == 0) {
            p.redirect = 1;
        }

        //// FOURTH WORD
        tok = strtok_r(NULL, " ", &saveptr);
        if (tok == NULL) {
            return p;
        }
        //// DEBUG
        //printf("tok4: %s\n", tok);
        p.second_arg = tok;
    }

    return p;
}
