/* Recursive descent parser. Left recursion is already removed (e = epsilon, i = id):
       E  -> T E'
       E' -> + T E' | e
       T  -> F T'
       T' -> * F T' | e
       F  -> ( E ) | i
   One function per non-terminal. Each function looks at the next input character,
   decides which production to use, and calls the functions of the symbols on the right. */
#include <stdio.h>
#include <stdlib.h>

char ip[100];                     /* input string */
int pos = 0;                      /* index of the next unread character */

void E(void), E_prime(void), T(void), T_prime(void), F(void);

void error(void) {
    printf("\nSyntax Error\n");
    exit(1);
}

/* consume the expected terminal c, or fail */
void match(char c) {
    if (ip[pos] == c) {
        pos++;
    } else {
        error();
    }
}

void E(void) {                    /* E -> T E' */
    printf("E -> T E'\n");
    T();
    E_prime();
}

void E_prime(void) {              /* E' -> + T E' | e */
    if (ip[pos] == '+') {
        printf("E' -> + T E'\n");
        match('+');
        T();
        E_prime();
    } else {                      /* anything else: use epsilon */
        printf("E' -> e\n");
    }
}

void T(void) {                    /* T -> F T' */
    printf("T -> F T'\n");
    F();
    T_prime();
}

void T_prime(void) {              /* T' -> * F T' | e */
    if (ip[pos] == '*') {
        printf("T' -> * F T'\n");
        match('*');
        F();
        T_prime();
    } else {
        printf("T' -> e\n");
    }
}

void F(void) {                    /* F -> ( E ) | i */
    if (ip[pos] == 'i') {
        printf("F -> i\n");
        pos++;
    } else if (ip[pos] == '(') {
        printf("F -> ( E )\n");
        match('(');
        E();
        match(')');
    } else {
        error();                  /* neither i nor ( : cannot continue */
    }
}

int main(void) {
    printf("Enter input expression: ");
    if (scanf("%99s", ip) != 1) {
        fputs("No input\n", stderr);
        return 1;
    }
    printf("\nSequence of production rules:\n");
    E();                          /* start symbol */
    if (ip[pos] == '\0') {        /* whole input consumed => accepted */
        printf("\nString Accepted\n");
    } else {
        printf("\nSyntax Error\n"); /* leftover characters */
    }
    return 0;
}
