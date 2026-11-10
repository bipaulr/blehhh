%{
#include <stdio.h>
int yylex(void);
void yyerror(const char *msg) {
    (void)msg;
    printf("Invalid IF statement\n");
}
%}
%token IF ID NUM REL
%left '+' '-'
%left '*' '/'
%%
input : input ifstmt '\n'   { printf("Valid IF statement\n"); }
      | input error '\n'    { yyerrok; }
      |
      ;
ifstmt : IF '(' cond ')' stmt
       ;
stmt : ifstmt
     | ID '=' expr ';'
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
%%
int main(void) {
    return yyparse();
}
