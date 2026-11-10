%{
#include <stdio.h>

int ints = 0;
int floats = 0;
%}

%%
\"([^\"\\\n]|\\.)*\"                { }
[A-Za-z_][A-Za-z0-9_]*              { }
[0-9]+\.[0-9]+                      { printf("%s : Float\n", yytext); floats++; }
[0-9]+                              { printf("%s : Integer\n", yytext); ints++; }
.|\n                                { }
%%

int yywrap(void)
{
    printf("Integers: %d\n", ints);
    printf("Floats: %d\n", floats);
    return 1;
}

int main(void)
{
    printf("Enter text (Ctrl+D to finish):\n");
    fflush(stdout);
    yylex();
    return 0;
}
