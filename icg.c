#include <stdio.h>
#include <ctype.h>
#include <string.h>

char opStack[100];
char valStack[100][20];
int opTop = -1, valTop = -1;
int tcount = 1;

int prec(char c)
{
    if (c == '*' || c == '/')
        return 2;
    if (c == '+' || c == '-')
        return 1;
    return 0;
}

void generate()
{
    char op = opStack[opTop--];
    char b[20], a[20], t[20];

    strcpy(b, valStack[valTop--]);
    strcpy(a, valStack[valTop--]);
    sprintf(t, "t%d", tcount++);
    printf("%s = %s %c %s\n", t, a, op, b);
    strcpy(valStack[++valTop], t);
}

int main()
{
    char line[100], result[20], tok[20];

    printf("Enter an assignment (e.g. x=a+b*c): ");
    scanf("%99s", line);

    int p = 0, q = 0;
    while (line[p] != '=' && line[p] != '\0')
        result[q++] = line[p++];
    result[q] = '\0';
    p++;

    for (; line[p]; p++) {
        char c = line[p];
        if (isalnum(c) || c == '_') {
            int k = 0;
            while (isalnum(line[p]) || line[p] == '_')
                tok[k++] = line[p++];
            tok[k] = '\0';
            p--;
            strcpy(valStack[++valTop], tok);
        }
        else if (c == '(')
            opStack[++opTop] = c;
        else if (c == ')') {
            while (opStack[opTop] != '(')
                generate();
            opTop--;
        }
        else {
            while (opTop >= 0 && prec(opStack[opTop]) >= prec(c))
                generate();
            opStack[++opTop] = c;
        }
    }
    while (opTop >= 0)
        generate();

    printf("%s = %s\n", result, valStack[valTop]);
    return 0;
}
