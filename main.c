#include <stdio.h>

#define MAX_LINE_LEN 256

int main(int argc, char** argv) {
    FILE *fp;

    if (argc < 2) {
        printf("useage c-shell <filename>\n");
        return 0;
    }

    char* filename = argv[1];
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
