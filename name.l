%{
#include <stdio.h>
%}

%option caseless

%%
.*name.*    { printf("Rejected\n"); }
.+          { printf("Accepted\n"); }
\n          ;
%%

int yywrap(void)
{
    return 1;
}

int main(void)
{
    printf("Enter a string: ");
    fflush(stdout);
    yylex();
    return 0;
}
