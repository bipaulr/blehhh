%{
#include <stdio.h>
#define YYSTYPE double
int yylex(void);
void yyerror(const char *msg) {
    (void)msg;
    printf("Invalid Expression\n");
}
%}
%token N
%left '+' '-'
%left '*' '/'
%right UMINUS
%%
input : input expr '\n'     { printf("Result = %g\n", $2); }
      | input error '\n'    { yyerrok; }
      |
      ;
expr  : expr '+' expr       { $$ = $1 + $3; }
      | expr '-' expr       { $$ = $1 - $3; }
      | expr '*' expr       { $$ = $1 * $3; }
      | expr '/' expr       { if ($3 == 0) {
                                  printf("Error: Division by zero\n");
                                  YYERROR;
                              }
                              $$ = $1 / $3; }
      | '(' expr ')'        { $$ = $2; }
      | '-' expr %prec UMINUS { $$ = -$2; }
      | N
      ;
%%
int main(void) {
    return yyparse();
}
