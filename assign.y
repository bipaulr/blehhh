%{
#include <stdio.h>
int yylex(void);
void yyerror(const char *msg) {
    (void)msg;
    printf("Invalid Assignment\n");
}
%}
%token ID NUM
%%
input : input stmt '\n'     { printf("Valid Assignment\n"); }
      | input error '\n'    { yyerrok; }
      |
      ;
stmt : ID '=' NUM '+' ID ';'
     | ID '=' ID '+' NUM ';'
     ;
%%
int main(void) {
    return yyparse();
}
