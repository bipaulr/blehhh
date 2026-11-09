#include <stdio.h>
#include <ctype.h>
#include <string.h>
int main(int argc, char *argv[]) {
    char *keywords[] = {"int", "float", "char", "if", "else", "while", "for", "return", NULL};
    char word[66];
    char filename[512];
    FILE *source;
    int ch, next, len, i, is_keyword, has_dot;
    if (argc > 2) {
        fprintf(stderr, "Usage: %s [source-file]\n", argv[0]);
        return 1;
    }
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
        if (isspace(ch)) {
            continue;
        }
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
            if (ch != EOF) ungetc(ch, source);
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
                ch = fgetc(source);
            }
            if (isdigit(ch)) {
                fputs("Number exceeds 63 characters\n", stderr);
                fclose(source);
                return 1;
            }
            word[len] = '\0';
            if (ch != EOF) ungetc(ch, source);
            printf("%s : Number\n", word);
        }
        else if (ch == '"') {
            len = 0;
            word[len++] = '"';
            while ((ch = fgetc(source)) != EOF && ch != '"' && len < 63) {
                word[len++] = ch;
            }
            if (ch != '"') {
                fputs("Unterminated or too long string literal\n", stderr);
                fclose(source);
                return 1;
            }
            word[len++] = '"';
            word[len] = '\0';
            printf("%s : String\n", word);
        }
        else if (strchr("+-*/%=<>!", ch)) {
            next = fgetc(source);
            if ((ch == '>' || ch == '<' || ch == '=' || ch == '!') && next == '=') {
                printf("%c= : Operator\n", ch);
            } else {
                if (next != EOF) ungetc(next, source);
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
    if (ferror(source)) {
        fputs("Error reading source file\n", stderr);
        fclose(source);
        return 1;
    }
    fclose(source);
    return 0;
}
