#include "utils.h"

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
