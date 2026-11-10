%{
#include <stdio.h>

int count[7];
const char *name[7] = {"+", "-", "*", "/", "%", "=", "=="};
int has_input = 0;

void print_counts(void)
{
    int i, first = 1;

    for (i = 0; i < 7; i++) {
        if (count[i] > 0) {
            if (!first) {
                printf(", ");
            }
            printf("%s : %d", name[i], count[i]);
            first = 0;
        }
        count[i] = 0;
    }
    if (first) {
        printf("No operators");
    }
    printf("\n");
    has_input = 0;
}
%}

%%
\"([^\"\\\n]|\\.)*\"                                { has_input = 1; }
"=="                                                { count[6]++; has_input = 1; }
"++"|"--"|"->"|"+="|"-="|"*="|"/="|"%="|"<="|">="|"!="   { has_input = 1; }
"+"                                                 { count[0]++; has_input = 1; }
"-"                                                 { count[1]++; has_input = 1; }
"*"                                                 { count[2]++; has_input = 1; }
"/"                                                 { count[3]++; has_input = 1; }
"%"                                                 { count[4]++; has_input = 1; }
"="                                                 { count[5]++; has_input = 1; }
\n                                                  { if (has_input) print_counts(); }
.                                                   { has_input = 1; }
%%

int yywrap(void)
{
    if (has_input) {
        print_counts();
    }
    return 1;
}

int main(void)
{
    printf("Enter an expression: ");
    fflush(stdout);
    yylex();
    return 0;
}
