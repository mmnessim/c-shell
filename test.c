#include "utils.h"
#include "string.h"
#include <assert.h>
#include <stdio.h>

void test_parse();

int main() {
    test_parse();
}

void test_parse() {
    printf("test_parse()\n");

    struct ParsedInput p;
    // Empty input
    char input[] = "";
    p = parse(input, strlen(input));
    assert(strcmp(p.command, "") == 0);
    assert(strcmp(p.flag, "") == 0);
    assert(strcmp(p.argument, "") == 0);
    printf("  Empty input passed\n");

    // Command only
    char input2[] = "cat";
    struct ParsedInput p2 = parse(input2, strlen(input2));
    assert(strcmp(p2.command, "cat") == 0);
    assert(strcmp(p2.flag, "") == 0);
    assert(strcmp(p2.argument, "") == 0);
    printf("  Command only passed\n");

    // Command and flag, but no argument
    char input3[] = "cat -n";
    struct ParsedInput p3 = parse(input3, strlen(input3));
    assert(strcmp(p3.command, "cat") == 0);
    assert(strcmp(p3.flag, "-n") == 0);
    assert(strcmp(p3.argument, "") == 0);
    printf("  Command and flag passed\n");

    // Command, flag, and argument
    char input4[] = "cat -n main.c";
    struct ParsedInput p4 = parse(input4, strlen(input4));
    assert(strcmp(p4.command, "cat") == 0);
    assert(strcmp(p4.flag, "-n") == 0);
    assert(strcmp(p4.argument, "main.c") == 0);
    printf("  Command, flag, and argument passed\n");

    // Extra words
    char input5[] = "cat -n main.c blorg extra stuff";
    struct ParsedInput p5 = parse(input5, strlen(input5));
    assert(strcmp(p5.command, "cat") == 0);
    assert(strcmp(p5.flag, "-n") == 0);
    assert(strcmp(p5.argument, "main.c") == 0);
    printf("  Extra words passed\n");

    // Just spaces
    char input6[] = "  ";
    struct ParsedInput p6 = parse(input6, strlen(input6));
    assert(strcmp(p6.command, "") == 0);
    assert(strcmp(p6.flag, "") == 0);
    assert(strcmp(p6.argument, "") == 0);
    printf("  Just spaces passed\n");

    // Space before first word
    char input7[] = " cat";
    struct ParsedInput p7 = parse(input7, strlen(input7));
    assert(strcmp(p7.command, "") == 0);
    assert(strcmp(p7.flag, "") == 0);
    assert(strcmp(p7.argument, "") == 0);
    printf("  Space before first word passed\n");

    printf("test_parse() passed\n");
}
