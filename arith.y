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

/* Addition and subtraction combine complete terms. */
expr : expr '+' term
     | expr '-' term
     | term
     ;

/* Multiplication and division bind more tightly. */
term : term '*' factor
     | term '/' factor
     | factor
     ;

/* Unary signs and parentheses are allowed; empty parentheses are not. */
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
