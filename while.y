%{
#include <stdio.h>

int yylex(void);

void yyerror(const char *msg)
{
    (void)msg;
    printf("Invalid WHILE statement\n");
}
%}

%token WHILE ID NUM REL
%left '+' '-'
%left '*' '/'

%%
while_stmt : WHILE '(' cond ')' body '\n'
                 { printf("Valid WHILE statement\n"); YYACCEPT; }
           ;

cond : expr REL expr
     | expr
     ;

expr : expr '+' expr
     | expr '-' expr
     | expr '*' expr
     | expr '/' expr
     | '(' expr ')'
     | ID
     | NUM
     ;

body : ';'
     | '{' stmts '}'
     ;

stmts : stmts ID '=' expr ';'
      |
      ;
%%

int main(void)
{
    printf("Enter a WHILE statement: ");
    fflush(stdout);
    return yyparse();
}
