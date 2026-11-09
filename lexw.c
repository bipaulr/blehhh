#include <stdio.h>
#include <ctype.h>
#include <string.h>

int main(void) {
    char *keywords[] = {
        "int", "float", "char", "if", "else", "while", "for", "return", NULL
    };
    char word[66];
    int ch, next, len, i, is_keyword, has_dot;

    while ((ch = getchar()) != EOF) {
        if (isspace(ch)) {
            continue;
        }

        if (isalpha(ch) || ch == '_') {
            len = 0;
            while ((isalnum(ch) || ch == '_') && len < 63) {
                word[len++] = ch;
                ch = getchar();
            }
            word[len] = '\0';
            ungetc(ch, stdin);

            is_keyword = 0;
            for (i = 0; keywords[i] != NULL; i++) {
                if (strcmp(word, keywords[i]) == 0) {
                    is_keyword = 1;
                }
            }
            printf("%s : %s\n", word, is_keyword ? "Keyword" : "Identifier");
        }
        else if (isdigit(ch)) {
            len = 0;
            has_dot = 0;
            while ((isdigit(ch) || (ch == '.' && !has_dot)) && len < 63) {
                if (ch == '.') has_dot = 1;
                word[len++] = ch;
                ch = getchar();
            }
            word[len] = '\0';
            ungetc(ch, stdin);
            printf("%s : Number\n", word);
        }
        else if (ch == '"') {
            len = 0;
            word[len++] = '"';
            while ((ch = getchar()) != EOF && ch != '"' && len < 63) {
                word[len++] = ch;
            }
            if (ch != '"') {
                fputs("Unterminated or too long string literal\n", stderr);
                return 1;
            }
            word[len++] = '"';
            word[len] = '\0';
            printf("%s : String\n", word);
        }
        else if (strchr("+-*/%=<>!", ch)) {
            next = getchar();
            if ((ch == '>' || ch == '<' || ch == '=' || ch == '!') && next == '=') {
                printf("%c= : Operator\n", ch);
            } else {
                ungetc(next, stdin);
                printf("%c : Operator\n", ch);
            }
        }
        else if (strchr(";,(){}", ch)) {
            printf("%c : Delimiter\n", ch);
        }
        else {
            printf("%c : Unknown\n", ch);
        }
    }

    return 0;
}
