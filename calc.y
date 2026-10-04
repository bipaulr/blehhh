%{
#include <stdio.h>
#define YYSTYPE double      /* every grammar symbol carries a double value */
int yylex(void);
void yyerror(const char *msg) {
    (void)msg;
    puts("error");
}
%}
%token N                /* a number */
/* Precedence: later lines bind tighter. */
%left '+' '-'
%left '*' '/'
%right UMINUS           /* unary minus, as in -5 */
%%
/* The input is a list of lines, each holding one expression. */
input : input expr '\n'     { printf("= %g\n", $2); }   /* print the result */
      | input error '\n'    { yyerrok; }                /* bad line: skip it and carry on */
      | /* empty */
      ;
/* $$ is the value of the rule, $1 $2 $3 are the values of its parts */
expr  : expr '+' expr       { $$ = $1 + $3; }
      | expr '-' expr       { $$ = $1 - $3; }
      | expr '*' expr       { $$ = $1 * $3; }
      | expr '/' expr       { if ($3 == 0) {
                                  puts("division by zero");
                                  YYERROR;
                              }
                              $$ = $1 / $3; }
      | '(' expr ')'        { $$ = $2; }
      | '-' expr %prec UMINUS { $$ = -$2; }
      | N                   /* a number keeps its own value */
      ;
%%
int main(void) {
    return yyparse();
}
