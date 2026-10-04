/* Lexical analyzer: splits C-like text into tokens.
   Spaces, tabs and newlines are skipped. */
#include <stdio.h>
#include <ctype.h>
#include <string.h>
int main(void) {
    char *keywords[] = {"int", "float", "char", "if", "else", "while", "for", "return", NULL};
    char word[64];
    int ch, len, i, is_keyword;
    while ((ch = getchar()) != EOF) {
        /* 1. skip white space */
        if (isspace(ch)) {
            continue;
        }
        /* 2. identifier or keyword: starts with a letter or _, then letters, digits, _ */
        if (isalpha(ch) || ch == '_') {
            len = 0;
            while ((isalnum(ch) || ch == '_') && len < 63) {
                word[len++] = ch;
                ch = getchar();
            }
            if (isalnum(ch) || ch == '_') {
                fputs("Identifier exceeds 63 characters\n", stderr);
                return 1;
            }
            word[len] = '\0';
            if (ch != EOF) ungetc(ch, stdin); /* give back the character ending the word */
            is_keyword = 0;
            for (i = 0; keywords[i] != NULL; i++) {
                if (strcmp(word, keywords[i]) == 0) {
                    is_keyword = 1;
                }
            }
            printf("%s\t%s\n", is_keyword ? "KEYWORD" : "IDENTIFIER", word);
        }
        /* 3. number: one or more digits */
        else if (isdigit(ch)) {
            len = 0;
            while (isdigit(ch) && len < 63) {
                word[len++] = ch;
                ch = getchar();
            }
            if (isdigit(ch)) {
                fputs("Number exceeds 63 characters\n", stderr);
                return 1;
            }
            word[len] = '\0';
            if (ch != EOF) ungetc(ch, stdin);
            printf("NUMBER\t%s\n", word);
        }

        /* 4. anything else is a single-character symbol */
        else {
            printf("SYMBOL\t%c\n", ch);
        }
    }
    return 0;
}
