%{
#include "y.tab.h"
%}
%%
for                          { return FOR; }
int                          { return T; }
[a-zA-Z_][a-zA-Z0-9_]*       { return ID; }
[0-9]+                       { return NUM; }
"<="|">="|"=="|"!="|"<"|">"  { return REL; }
"++"|"--"                    { return INC; }
[ \t]                        ;
.|\n                         { return yytext[0]; }
%%
int yywrap(void) {
    return 1;
}
