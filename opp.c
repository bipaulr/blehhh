/* Operator precedence parser for  E -> E+E | E*E | id   (the letter i stands for id).
   Compare the terminal nearest the top of the stack with the next input terminal:
     '<'  stack side is weaker   -> SHIFT the input symbol
     '>'  stack side is stronger -> REDUCE the handle on top of the stack
   Accept when the stack is $E and the input is $. */
#include <stdio.h>
#include <string.h>

char terms[] = "i+*$";            /* order of the rows and columns of the table */

/* rel[row][col]: row = terminal on the stack, col = next input terminal */
char rel[4][4] = {
    /*         i    +    *    $  */
    /* i */ { 'e', '>', '>', '>' },   /* e = error: i cannot be followed by i   */
    /* + */ { '<', '>', '<', '>' },   /* * binds tighter, + is left associative */
    /* * */ { '<', '>', '>', '>' },
    /* $ */ { '<', '<', '<', '=' }    /* $ against $ means: finished            */
};

char stk[100];                    /* parse stack: terminals and E */
int top = 0;

/* row/column number of a terminal, -1 if the character is not in the table */
int idx(char c) {
    char *p;
    if (c == '\0') {
        return -1;                /* strchr would match the string terminator */
    }
    p = strchr(terms, c);
    return p ? (int)(p - terms) : -1;
}

/* nearest terminal below any E on the stack */
char top_terminal(void) {
    int k;
    for (k = top; k >= 0; k--) {
        if (stk[k] != 'E') {
            return stk[k];
        }
    }
    return '$';
}

void show(const char *input, const char *action) {
    stk[top + 1] = '\0';
    printf("%-10s %-10s %s\n", stk, input, action);
}

int main(void) {
    char ip[50], msg[30], a, b, r;
    int i = 0;

    printf("Enter input string (i for id, end with $): ");
    if (scanf("%49s", ip) != 1) {
        fputs("No input\n", stderr);
        return 1;
    }
    stk[0] = '$';
    stk[1] = '\0';
    printf("\n%-10s %-10s %s\n", "STACK", "INPUT", "ACTION");

    while (1) {
        a = top_terminal();
        b = ip[i];
        if (idx(b) < 0) {         /* unknown character, or the $ was left out */
            show(ip + i, "Invalid symbol: rejected");
            break;
        }
        if (a == '$' && b == '$') {                       /* both ends reached */
            show(ip + i, strcmp(stk, "$E") == 0 ? "String is accepted"
                                                : "String is not accepted");
            break;
        }
        r = rel[idx(a)][idx(b)];
        if (r == '<') {                                   /* SHIFT */
            stk[++top] = b;
            sprintf(msg, "Shift %c", b);
            i++;
            show(ip + i, msg);
        } else if (r == '>') {                            /* REDUCE: check the handle */
            if (stk[top] == 'i') {                        /* E -> i */
                stk[top] = 'E';
                show(ip + i, "Reduce E -> i");
            } else if (top >= 2 && stk[top] == 'E' && stk[top - 2] == 'E' &&
                       (stk[top - 1] == '+' || stk[top - 1] == '*')) { /* E -> E+E or E*E */
                sprintf(msg, "Reduce E -> E%cE", stk[top - 1]);
                top -= 2;
                stk[top] = 'E';
                show(ip + i, msg);
            } else {
                show(ip + i, "No handle: string is not accepted");
                break;
            }
        } else {
            show(ip + i, "Error: string is not accepted");
            break;
        }
    }
    return 0;
}
