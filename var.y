%{
#include <stdio.h>
int yylex(void);
void yyerror(const char *msg) {
    (void)msg;
    printf("Invalid Variable\n");
}
%}
%token L D
%%
variable : L rest '\n'    { printf("Valid Variable\n"); YYACCEPT; }
         ;
rest : rest L
     | rest D
     |
     ;
%%
int main(void) {
    printf("Enter a variable name: ");
    fflush(stdout);
    return yyparse();
}
