%{
#include <stdio.h>
#include "y.tab.h"
%}

%%
[a-zA-Z]    { return L; }
[0-9]       { return D; }
\n          { return '\n'; }
.           { return yytext[0]; }
%%

int yywrap(void)
{
    return 1;
}
