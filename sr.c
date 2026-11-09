#include <stdio.h>
#include <string.h>

char stk[100];
int top = -1;
char tok[100];

void show(int pos, const char *action) {
    int k;
    printf("$");
    for (k = 0; k <= top; k++) {
        if (stk[k] == 'i') printf("id"); else printf("%c", stk[k]);
    }
    printf("\t\t");
    for (k = pos; tok[k]; k++) {
        if (tok[k] == 'i') printf("id"); else printf("%c", tok[k]);
    }
    printf("$\t\t%s\n", action);
}

int reduce(int pos) {
    char look = tok[pos];
    if (top >= 0 && stk[top] == 'i') {
        stk[top] = 'E';
        show(pos, "REDUCE E->id");
        return 1;
    }
    if (top >= 2 && stk[top] == ')' && stk[top - 1] == 'E' && stk[top - 2] == '(') {
        top -= 2;
        stk[top] = 'E';
        show(pos, "REDUCE E->(E)");
        return 1;
    }
    if (top >= 2 && stk[top] == 'E' && stk[top - 1] == '*' && stk[top - 2] == 'E') {
        top -= 2;
        stk[top] = 'E';
        show(pos, "REDUCE E->E*E");
        return 1;
    }
    if (top >= 2 && stk[top] == 'E' && stk[top - 1] == '+' && stk[top - 2] == 'E'
        && look != '*') {
        top -= 2;
        stk[top] = 'E';
        show(pos, "REDUCE E->E+E");
        return 1;
    }
    return 0;
}

int main(void) {
    char s[100];
    int n = 0, k, pos = 0;

    printf("GRAMMAR is E->E+E | E->E*E | E->(E) | E->id\n");
    printf("enter input string: ");
    if (scanf("%99s", s) != 1) {
        fputs("No input\n", stderr);
        return 1;
    }
    for (k = 0; s[k]; ) {
        if (s[k] == 'i' && s[k + 1] == 'd') {
            tok[n++] = 'i';
            k += 2;
        } else if (s[k] == '+' || s[k] == '*' || s[k] == '(' || s[k] == ')') {
            tok[n++] = s[k++];
        } else {
            printf("\nInvalid symbol '%c'\nString rejected\n", s[k]);
            return 0;
        }
    }
    tok[n] = '\0';

    printf("\nSTACK\t\tINPUT\t\tACTION\n");
    while (1) {
        while (reduce(pos)) {
            ;
        }
        if (tok[pos] == '\0') {
            break;
        }
        stk[++top] = tok[pos++];
        show(pos, "SHIFT");
    }

    if (top == 0 && stk[0] == 'E') {
        printf("\nString accepted\n");
    } else {
        printf("\nString rejected\n");
    }
    return 0;
}
