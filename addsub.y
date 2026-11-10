%{
#include <stdio.h>
int yylex(void);
void yyerror(const char *msg) {
    (void)msg;
    printf("Invalid Expression\n");
}
%}
%token NUM
%%
input : input expr '\n'     { printf("Result = %d\n", $2); }
      | input error '\n'    { yyerrok; }
      |
      ;
expr  : expr '+' NUM        { $$ = $1 + $3; }
      | expr '-' NUM        { $$ = $1 - $3; }
      | NUM
      ;
%%
int main(void) {
    return yyparse();
}
