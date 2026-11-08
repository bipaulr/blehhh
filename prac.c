
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main() {
    char expr[100], tokens[50][20];
    int n = 0, count = 1;

    printf("Enter expression: ");
    scanf("%99s", expr);

    for (int i = 0; expr[i]; ) {
        if (isalnum((unsigned char)expr[i])) {
            int j = 0;

            while (isalnum((unsigned char)expr[i]))
                tokens[n][j++] = expr[i++];

            tokens[n++][j] = '\0';
        } else {
            tokens[n][0] = expr[i++];
            tokens[n++][1] = '\0';
        }
    }

    char *ops[] = {"*", "/", "+", "-"};

    for (int k = 0; k < 4; k++) {
        for (int i = 1; i < n - 1; i++) {
            if (strcmp(tokens[i], ops[k]) == 0) {
                char temp[20];
                sprintf(temp, "t%d", count++);

                printf("%s = %s %s %s\n",
                       temp, tokens[i-1],
                       tokens[i], tokens[i+1]);

                strcpy(tokens[i-1], temp);

                for (int j = i; j < n - 2; j++)
                    strcpy(tokens[j], tokens[j+2]);

                n -= 2;
                i = 0;
            }
        }
    }

    return 0;
}
