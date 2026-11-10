%{
#include <stdio.h>

int single = 0;
int multi = 0;
%}

%%
\"([^\"\\\n]|\\.)*\"                { }
'([^'\\\n]|\\.)'                    { }
"//".*                              { single++; printf("Single-line comment: %s\n", yytext); }
"/*"([^*]|\*+[^*/])*\*+"/"          { multi++; printf("Multi-line comment:\n%s\n", yytext); }
.|\n                                { }
%%

int yywrap(void)
{
    printf("Single-line comments: %d\n", single);
    printf("Multi-line comments: %d\n", multi);
    return 1;
}

int main(void)
{
    printf("Enter C code (Ctrl+D to finish):\n");
    fflush(stdout);
    yylex();
    return 0;
}
