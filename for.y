%{
#include <stdio.h>
int yylex(void);
void yyerror(const char *msg) {
    (void)msg;
    printf("Invalid FOR statement\n");
}
%}
%token FOR T ID NUM REL INC
%left '+' '-'
%left '*' '/'
%%
start  : forstmt '\n'
                { printf("Valid FOR statement\n"); YYACCEPT; }
       ;
forstmt : FOR '(' init ';' cond ';' update ')' body
        ;
init   : ID '=' expr
       | T ID '=' expr
       |
       ;
cond   : expr REL expr
       |
       ;
update : ID INC
       | INC ID
       | ID '=' expr
       |
       ;
expr   : expr '+' expr
       | expr '-' expr
       | expr '*' expr
       | expr '/' expr
       | '(' expr ')'
       | ID
       | NUM
       ;
body   : '{' stmts '}'
       | stmt
       | ';'
       ;
stmts  : stmts stmt
       |
       ;
stmt   : ID '=' expr ';'
       | forstmt
       ;
%%
int main(void) {
    printf("Enter a FOR statement: ");
    fflush(stdout);
    return yyparse();
}
