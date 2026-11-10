%{
#include <stdio.h>

int total = 0;
%}

KW  auto|break|case|char|const|continue|default|do|double|else|enum|extern|float|for|goto|if|int|long|register|return|short|signed|sizeof|static|struct|switch|typedef|union|unsigned|void|volatile|while

%%
\"([^\"\\\n]|\\.)*\"                { }
"//".*                              { }
"/*"([^*]|\*+[^*/])*\*+"/"          { }
{KW}                                { printf("Keyword: %s\n", yytext); total++; }
[A-Za-z_][A-Za-z0-9_]*              { }
.|\n                                { }
%%

int yywrap(void)
{
    printf("Total keywords: %d\n", total);
    return 1;
}

int main(void)
{
    printf("Enter C code (Ctrl+D to finish):\n");
    fflush(stdout);
    yylex();
    return 0;
}
