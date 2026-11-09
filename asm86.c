#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(void) {
    char line[60], res[10], a1[10], a2[10], op;
    int n;

    printf("Enter three address code (type exit to stop):\n");
    while (fgets(line, sizeof line, stdin)) {
        if (strncmp(line, "exit", 4) == 0) {
            break;
        }
        n = sscanf(line, " %9[A-Za-z0-9_] = %9[A-Za-z0-9_] %c %9[A-Za-z0-9_]", res, a1, &op, a2);
        if (n == 2) {
            printf("MOV AX, %s\n", a1);
            printf("MOV %s, AX\n", res);
        } else if (n == 4) {
            printf("MOV AX, %s\n", a1);
            switch (op) {
                case '+': printf("ADD AX, %s\n", a2); break;
                case '-': printf("SUB AX, %s\n", a2); break;
                case '*':
                    if (isdigit((unsigned char)a2[0])) {
                        printf("MOV BX, %s\n", a2);
                        strcpy(a2, "BX");
                    }
                    printf("MUL %s\n", a2);
                    break;
                case '/':
                    if (isdigit((unsigned char)a2[0])) {
                        printf("MOV BX, %s\n", a2);
                        strcpy(a2, "BX");
                    }
                    printf("MOV DX, 0\n");
                    printf("DIV %s\n", a2);
                    break;
                default:
                    printf("; unknown operator %c\n", op);
            }
            printf("MOV %s, AX\n", res);
        } else if (strspn(line, " \t\r\n") != strlen(line)) {
            printf("; invalid three-address code\n");
        }
    }
    return 0;
}
