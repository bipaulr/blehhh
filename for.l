%{
#include "y.tab.h"
%}
%%
  /* keywords come before the name rule: when two rules match the same text, the first wins */
for                          { return FOR; }
int                          { return T; }
  /* names and numbers */
[a-zA-Z_][a-zA-Z0-9_]*       { return ID; }
[0-9]+                       { return NUM; }
  /* comparison operators, and ++ or -- */
"<="|">="|"=="|"!="|"<"|">"  { return REL; }
"++"|"--"                    { return INC; }
  /* skip spaces and tabs */
[ \t]                        ;
  /* ( ) ; = + - * / { } and newline are passed to yacc as themselves */
.|\n                         { return yytext[0]; }
%%
int yywrap(void) {
    return 1;
}
