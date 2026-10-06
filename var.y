%{
#include <stdio.h>
int yylex(void);
/* yacc calls this when the input does not fit the grammar */
void yyerror(const char *msg) {
    (void)msg;
    printf("Invalid variable\n");
}
%}
%token L D          /* L = a letter, D = a digit */
%%
/* A variable is: one letter, then any number of letters or digits, then end of line. */
variable : L rest '\n'    { printf("Valid variable\n"); YYACCEPT; }
         ;
/* rest = zero or more letters or digits */
rest : rest L
     | rest D
     | /* empty */
     ;
%%
int main(void) {
    return yyparse();
}
