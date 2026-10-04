%{
#include <stdio.h>
int yylex(void);
void yyerror(const char *msg) {
    (void)msg;
    puts("Invalid FOR statement");
}
%}
/* T = the word int, REL = < > <= >= == !=, INC = ++ or -- */
%token FOR T ID NUM REL INC
%left '+' '-'
%left '*' '/'
%%
/* for ( init ; condition ; update ) body    then end of line */
for_stmt : FOR '(' init ';' cond ';' update ')' body '\n'
                { puts("Valid FOR statement"); YYACCEPT; }
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
/* update part:  i++   or   i = i + 1   or empty */
update : ID INC
       | ID '=' expr
       | /* empty */
       ;
/* a simple arithmetic expression: names, numbers and + - * / */
expr   : expr '+' expr
       | expr '-' expr
       | expr '*' expr
       | expr '/' expr
       | ID
       | NUM
       ;
/* body:  a lone ;   or a block { x = 1; y = 2; } */
body   : ';'
       | '{' stmts '}'
       ;
/* zero or more assignment statements inside a block */
stmts  : stmts ID '=' expr ';'
       | /* empty */
       ;
%%
int main(void) {
    return yyparse();
}
