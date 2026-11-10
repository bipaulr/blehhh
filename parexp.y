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
input : input E '\n'        { printf("Valid Expression\n"); }
      | input error '\n'    { yyerrok; }
      |
      ;
E : E '+' T
  | E '-' T
  | T
  ;
T : T '*' F
  | T '/' F
  | F
  ;
F : '(' E ')'
  | NUM
  ;
%%
int main(void) {
    return yyparse();
}
