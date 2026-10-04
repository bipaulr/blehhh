%option caseless
%%
  /* A line that contains "name" anywhere (any letter case) is rejected. */
.*name.*    { puts("REJECT"); }
  /* Any other non-empty line is accepted.
     Both rules match the whole line when "name" is present;
     lex then chooses the rule written first, so REJECT wins. */
.+          { puts("ACCEPT"); }
  /* ignore the newline itself */
\n          ;
%%
int main(void) {
    yylex();
    return 0;
}
int yywrap(void) {
    return 1;
}
