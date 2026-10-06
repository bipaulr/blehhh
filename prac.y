%{
#include <stdio.h>

int yylex(void);

void yyerror(const char *s)
{
    (void)s;
    printf("Invalid Variable\n");
}
%}

%token L D

%%
variable : L rest '\n'  { printf("Valid Variable\n"); YYACCEPT; }
         ;
rest : rest L
     | rest D 
     | /* empty */
     ;
%%

int main(void)
{
    printf("Enter a variable name: ");
    fflush(stdout);
    return yyparse();
}

