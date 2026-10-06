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
/* Check one complete statement followed by Enter. */
while_stmt : WHILE '(' cond ')' body '\n'
                 { printf("Valid WHILE statement\n"); YYACCEPT; }
           ;

/* A condition must be present: a comparison or a numeric expression. */
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

/* Same body style as the FOR checker: semicolon or assignment block. */
body : ';'
     | '{' stmts '}'
     ;

stmts : stmts ID '=' expr ';'
      | /* empty */
      ;
%%

int main(void)
{
    printf("Enter a WHILE statement: ");
    fflush(stdout);
    return yyparse();
}
