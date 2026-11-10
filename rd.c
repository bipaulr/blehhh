#include <stdio.h>
#include <stdlib.h>

char ip[100];
int pos = 0;

void E(void), E_prime(void), T(void), T_prime(void), F(void);

void error(void) {
    printf("\nSyntax Error\n");
    exit(1);
}

void match(char c) {
    if (ip[pos] == c) {
        pos++;
    } else {
        error();
    }
}

void E(void) {
    printf("E -> T E'\n");
    T();
    E_prime();
}

void E_prime(void) {
    if (ip[pos] == '+') {
        printf("E' -> + T E'\n");
        match('+');
        T();
        E_prime();
    } else {
        printf("E' -> e\n");
    }
}

void T(void) {
    printf("T -> F T'\n");
    F();
    T_prime();
}

void T_prime(void) {
    if (ip[pos] == '*') {
        printf("T' -> * F T'\n");
        match('*');
        F();
        T_prime();
    } else {
        printf("T' -> e\n");
    }
}

void F(void) {
    if (ip[pos] == 'i') {
        printf("F -> i\n");
        pos++;
    } else if (ip[pos] == '(') {
        printf("F -> ( E )\n");
        match('(');
        E();
        match(')');
    } else {
        error();
    }
}

int main(void) {
    printf("Enter input expression: ");
    scanf("%99s", ip);
    printf("\nSequence of production rules:\n");
    E();
    if (ip[pos] == '\0') {
        printf("\nString Accepted\n");
    } else {
        printf("\nSyntax Error\n");
    }
    return 0;
}
