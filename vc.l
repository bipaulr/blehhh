%{
#include <stdio.h>

int vowels = 0;
int consonants = 0;
int has_input = 0;

void print_counts(void)
{
    printf("Vowels: %d\n", vowels);
    printf("Consonants: %d\n", consonants);
    vowels = 0;
    consonants = 0;
    has_input = 0;
}
%}

%%
[aeiouAEIOU]    {
                    printf("Vowel: %c\n", yytext[0]);
                    vowels++;
                    has_input = 1;
                }
[a-zA-Z]        {
                    printf("Consonant: %c\n", yytext[0]);
                    consonants++;
                    has_input = 1;
                }
\n              { print_counts(); }
.               { has_input = 1; } /* Ignore spaces, digits and punctuation. */
%%

int yywrap(void)
{
    /* Also summarize a final string that does not end with a newline. */
    if (has_input) {
        print_counts();
    }
    return 1;
}

int main(void)
{
    printf("Enter a string: ");
    fflush(stdout);
    yylex();
    return 0;
}
