%{
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "y.tab.h"
static char *copy_text(const char *text) {
    char *copy = malloc(strlen(text) + 1);
    if (copy == NULL) {
        fputs("Out of memory\n", stderr);
        exit(EXIT_FAILURE);
    }
    strcpy(copy, text);
    return copy;
}
%}
%%
  /* a name (a, x1, total) or a number (42): keep the text, tell yacc which one it is */
[a-zA-Z_][a-zA-Z0-9_]*|[0-9]+   { yylval.text = copy_text(yytext);
                                  return isdigit(yytext[0]) ? NUM : ID; }
  /* skip spaces and tabs */
[ \t]                           ;
  /* + - * / ( ) and newline are passed to yacc as themselves */
.|\n                            { return yytext[0]; }
%%
int yywrap(void) {
    return 1;
}
