#include "utils.h"
#include "string.h"
#include <assert.h>
#include <stdio.h>

void test_parse();
void test_parse_redirection();
void test_parse_with_quote();

int main() {
    test_parse();
    test_parse_redirection();
    test_parse_with_quote();
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

    // Space before first word
    char input8[] = "cat main.c";
    struct ParsedInput p8 = parse(input8, strlen(input8));
    assert(strcmp(p8.command, "cat") == 0);
    assert(strcmp(p8.flag, "") == 0);
    assert(strcmp(p8.argument, "main.c") == 0);
    printf("  Command and argument passed\n");

    printf("test_parse() passed\n\n");
}

void test_parse_redirection() {
    printf("test_parse_redirections()\n");

    struct ParsedInput p;
    // Empty input
    char input[] = "cat main.c >> file.txt";
    p = parse(input, strlen(input));
    assert(strcmp(p.command, "cat") == 0);
    assert(strcmp(p.flag, "") == 0);
    assert(strcmp(p.argument, "main.c") == 0);
    assert(p.redirect == 1);
    assert(strcmp(p.second_arg, "file.txt") == 0);
    printf("  Redirect and second_arg passed\n");

    char input2[] = "cat -n main.c >> file.txt";
    struct ParsedInput p2 = parse(input2, strlen(input2));
    assert(strcmp(p2.command, "cat") == 0);
    assert(strcmp(p2.flag, "-n") == 0);
    assert(strcmp(p2.argument, "main.c") == 0);
    assert(p2.redirect == 1);
    assert(strcmp(p2.second_arg, "file.txt") == 0);
    printf("  Redirect, flag, and second_arg passed\n");

}

void test_parse_with_quote() {
    //// TODO
    // Add test cases for quote wrapped text with flags

    printf("test_parse_with_quote()\n");

    char input[] = "echo \"This is a test\"";
    struct ParsedInput p = parse(input, strlen(input));
    printf("%s\n", p.argument);

    assert(strcmp(p.command, "echo") == 0);
    assert(strcmp(p.argument, "This is a test") == 0);

    printf("  Quotes around argument passed\n");
}
