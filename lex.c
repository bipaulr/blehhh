/* Lexical analyzer: reads a file and splits C-like text into tokens.
   Spaces, tabs and newlines are skipped. */
#include <stdio.h>
#include <ctype.h>
#include <string.h>
int main(int argc, char *argv[]) {
    char *keywords[] = {"int", "float", "char", "if", "else", "while", "for", "return", NULL};
    char word[64];
    char filename[512];
    FILE *source;
    int ch, len, i, is_keyword;
    if (argc > 2) {
        fprintf(stderr, "Usage: %s [source-file]\n", argv[0]);
        return 1;
    }
    /* Accept a file argument, or ask for its name when run as ./lex. */
    if (argc == 2) {
        source = fopen(argv[1], "r");
    } else {
        printf("Enter source filename: ");
        fflush(stdout);
        if (fgets(filename, sizeof(filename), stdin) == NULL) return 1;
        filename[strcspn(filename, "\r\n")] = '\0';
        source = fopen(filename, "r");
    }
    if (source == NULL) {
        perror("Cannot open source file");
        return 1;
    }
    while ((ch = fgetc(source)) != EOF) {
        /* 1. skip white space */
        if (isspace(ch)) {
            continue;
        }
        /* 2. identifier or keyword: starts with a letter or _, then letters, digits, _ */
        if (isalpha(ch) || ch == '_') {
            len = 0;
            while ((isalnum(ch) || ch == '_') && len < 63) {
                word[len++] = ch;
                ch = fgetc(source);
            }
            if (isalnum(ch) || ch == '_') {
                fputs("Identifier exceeds 63 characters\n", stderr);
                fclose(source);
                return 1;
            }
            word[len] = '\0';
            if (ch != EOF) ungetc(ch, source); /* return the character ending the word */
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
                ch = fgetc(source);
            }
            if (isdigit(ch)) {
                fputs("Number exceeds 63 characters\n", stderr);
                fclose(source);
                return 1;
            }
            word[len] = '\0';
            if (ch != EOF) ungetc(ch, source);
            printf("NUMBER\t%s\n", word);
        }

        /* 4. anything else is a single-character symbol */
        else {
            printf("SYMBOL\t%c\n", ch);
        }
    }
    if (ferror(source)) {
        fputs("Error reading source file\n", stderr);
        fclose(source);
        return 1;
    }
    fclose(source);
    return 0;
}
