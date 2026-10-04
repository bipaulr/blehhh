%{
#include <stdlib.h>
#define YYSTYPE double      /* every value passed between lex and yacc is a double */
#include "y.tab.h"
%}
%%
  /* a number such as 12 or 3.5: store its value in yylval, tell yacc it is N */
[0-9]+(\.[0-9]+)?   { yylval = atof(yytext); return N; }
  /* skip spaces and tabs */
[ \t]               ;
  /* + - * / ( ) and newline are passed to yacc as themselves */
.|\n                { return yytext[0]; }
%%
int yywrap(void) {
    return 1;
}
