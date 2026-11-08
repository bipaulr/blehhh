%{
#include <stdio.h>
%}

%option caseless

%%
.*paul.*    { printf("NOT ACCEPTED (Contains Paul)\n"); }
.+          { printf("ACCEPTED (Does not contain Paul)\n"); }
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
