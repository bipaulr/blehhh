%{
#include <stdio.h>
int yylex(void);
void yyerror(const char *msg) {
    (void)msg;
    printf("Invalid FOR statement\n");
}
%}
/* T = the word int, REL = < > <= >= == !=, INC = ++ or -- */
%token FOR T ID NUM REL INC
%left '+' '-'
%left '*' '/'
%%
/* one complete for statement, then end of line */
start  : forstmt '\n'
                { printf("Valid FOR statement\n"); YYACCEPT; }
       ;
/* for ( init ; condition ; update ) body    -- each of the three parts may be empty */
forstmt : FOR '(' init ';' cond ';' update ')' body
        ;
/* init part:  i = 0   or   int i = 0   or empty */
init   : ID '=' expr
       | T ID '=' expr
       | /* empty */
       ;
/* condition part:  i < 10   or empty */
cond   : expr REL expr
       | /* empty */
       ;
/* update part:  i++  ++i  i--  --i  i = i + 1   or empty */
update : ID INC
       | INC ID
       | ID '=' expr
       | /* empty */
       ;
/* a simple arithmetic expression: names, numbers, + - * / and brackets */
expr   : expr '+' expr
       | expr '-' expr
       | expr '*' expr
       | expr '/' expr
       | '(' expr ')'
       | ID
       | NUM
       ;
/* body:  { statements }   or one statement   or just ; */
body   : '{' stmts '}'
       | stmt
       | ';'
       ;
/* zero or more statements inside a block */
stmts  : stmts stmt
       | /* empty */
       ;
/* a statement: an assignment, or another for loop inside this one */
stmt   : ID '=' expr ';'
       | forstmt
       ;
%%
int main(void) {
    printf("Enter a FOR statement: ");
    fflush(stdout);
    return yyparse();
}
