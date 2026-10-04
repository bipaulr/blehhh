%{
/* token codes L and D are defined by the yacc file */
#include "y.tab.h"
%}
%%
  /* a letter */
[a-zA-Z]    { return L; }
  /* a digit */
[0-9]       { return D; }
  /* anything else (newline, _, -, space ...) is handed to yacc as itself;
     yacc only expects newline, so any other character is a syntax error */
.|\n        { return yytext[0]; }
%%
int yywrap(void) {
    return 1;
}
