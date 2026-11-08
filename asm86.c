/* Back end: three-address code to 8086 assembly (MOV, ADD, SUB, MUL, DIV).
   Input lines like  t1=b*c  or  t1 = b * c  or  x=t2 ; type exit to stop.

   8086 rules used here:
     ADD/SUB AX, src   works on AX and a variable
     MUL src           multiplies AX by src, result in AX (one operand only)
     DIV src           divides DX:AX by src, quotient in AX (so clear DX first)
     MUL and DIV cannot take a constant, so a number is first copied into BX.
   Every result is moved back to its variable with  MOV result, AX. */
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
        /* split "res = a1 op a2" into pieces; spaces are optional */
        n = sscanf(line, " %9[A-Za-z0-9_] = %9[A-Za-z0-9_] %c %9[A-Za-z0-9_]", res, a1, &op, a2);
        if (n == 2) {                                   /* plain copy: x = t2 */
            printf("MOV AX, %s\n", a1);
            printf("MOV %s, AX\n", res);
        } else if (n == 4) {
            printf("MOV AX, %s\n", a1);                 /* load first operand into AX */
            switch (op) {
                case '+': printf("ADD AX, %s\n", a2); break;
                case '-': printf("SUB AX, %s\n", a2); break;
                case '*':
                    if (isdigit((unsigned char)a2[0])) {
                        printf("MOV BX, %s\n", a2);     /* MUL needs a register or variable */
                        strcpy(a2, "BX");
                    }
                    printf("MUL %s\n", a2);             /* AX = AX * a2 */
                    break;
                case '/':
                    if (isdigit((unsigned char)a2[0])) {
                        printf("MOV BX, %s\n", a2);     /* DIV needs a register or variable */
                        strcpy(a2, "BX");
                    }
                    printf("MOV DX, 0\n");              /* clear DX before DIV */
                    printf("DIV %s\n", a2);             /* AX = AX / a2 */
                    break;
                default:
                    printf("; unknown operator %c\n", op);
            }
            printf("MOV %s, AX\n", res);                /* store the result */
        } else if (strspn(line, " \t\r\n") != strlen(line)) {
            printf("; invalid three-address code\n");
        }
    }
    return 0;
}
