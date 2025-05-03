#include <stdio.h>

int main(int argc, char** argv) {
    FILE *fp;

    if (argc < 2) {
        printf("useage c-shell <filename>\n");
        return 0;
    }

    char* filename = argv[1];
    fp = fopen(filename, "r");

    fclose(fp);
    return 0;
}
