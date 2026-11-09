%{
#include <stdlib.h>
#define YYSTYPE double
#include "y.tab.h"
%}
%%
[0-9]+(\.[0-9]+)?   { yylval = atof(yytext); return N; }
[ \t]               ;
.|\n                { return yytext[0]; }
%%
int yywrap(void) {
    return 1;
}
