%{
#include <stdio.h>

int yylex(void);

void yyerror(const char *msg)
{
    (void)msg;
    printf("Invalid arithmetic expression\n");
}
%}

%token ID NUM

%%
input : expr '\n'
            { printf("Valid arithmetic expression\n"); YYACCEPT; }
      ;

expr : expr '+' term
     | expr '-' term
     | term
     ;

term : term '*' factor
     | term '/' factor
     | factor
     ;

factor : '+' factor
       | '-' factor
       | '(' expr ')'
       | ID
       | NUM
       ;
%%

int main(void)
{
    printf("Enter an arithmetic expression: ");
    fflush(stdout);
    return yyparse();
}
