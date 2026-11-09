#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

char op_stack[100];
int op_top = -1;
char val_stack[100][12];
int val_top = -1;
int tcount = 1;

int prec(char c) {
    if (c == '*' || c == '/') return 2;
    if (c == '+' || c == '-') return 1;
    return 0;
}

void fail(const char *why) {
    printf("Invalid expression: %s\n", why);
    exit(1);
}

void generate(void) {
    char op, a[12], b[12], t[12];
    if (op_top < 0 || val_top < 1 || op_stack[op_top] == '(') {
        fail("operator or operand missing");
    }
    op = op_stack[op_top--];
    strcpy(b, val_stack[val_top--]);
    strcpy(a, val_stack[val_top--]);
    sprintf(t, "t%d", tcount++);
    printf("%s = %s %c %s\n", t, a, op, b);
    strcpy(val_stack[++val_top], t);
}

int main(void) {
    char line[100], result[12], tok[12], c;
    int p = 0, q = 0, k, expect_operand = 1;

    printf("Enter an assignment (e.g. x=a+b*c): ");
    if (scanf("%99s", line) != 1) {
        fputs("No input\n", stderr);
        return 1;
    }
    while (line[p] && line[p] != '=' && q < 11) {
        result[q++] = line[p++];
    }
    result[q] = '\0';
    if (line[p] != '=' || q == 0) {
        fail("expected  name=expression");
    }
    p++;

    for (; line[p]; p++) {
        c = line[p];
        if (isalnum((unsigned char)c) || c == '_') {
            if (!expect_operand) fail("two operands in a row");
            k = 0;
            while (isalnum((unsigned char)line[p]) || line[p] == '_') {
                if (k >= 11) fail("name too long");
                tok[k++] = line[p++];
            }
            tok[k] = '\0';
            p--;
            strcpy(val_stack[++val_top], tok);
            expect_operand = 0;
        } else if (c == '(') {
            if (!expect_operand) fail("operator missing before (");
            op_stack[++op_top] = c;
        } else if (c == ')') {
            if (expect_operand) fail("operand missing before )");
            while (op_top >= 0 && op_stack[op_top] != '(') generate();
            if (op_top < 0) fail("unbalanced )");
            op_top--;
        } else if (c == '+' || c == '-' || c == '*' || c == '/') {
            if (expect_operand) fail("operand missing before operator");
            while (op_top >= 0 && prec(op_stack[op_top]) >= prec(c)) {
                generate();
            }
            op_stack[++op_top] = c;
            expect_operand = 1;
        } else {
            fail("unknown character");
        }
    }
    if (expect_operand) fail("expression ends early");
    while (op_top >= 0) {
        if (op_stack[op_top] == '(') fail("unbalanced (");
        generate();
    }
    printf("%s = %s\n", result, val_stack[val_top]);
    return 0;
}
