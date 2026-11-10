#include <stdio.h>
#include <string.h>
#include <ctype.h>

char p[50][100];
int first[26][128], follow[26][128], nt[26], n;

int add(int set[], char c) {
    if (!set[(int)c]) {
        set[(int)c] = 1;
        return 1;
    }
    return 0;
}

int main() {
    int i, j, k, x, change, nullable;
    char A, B, c;

    printf("Enter number of productions: ");
    scanf("%d", &n);

    printf("Enter productions (use # for epsilon):\n");
    for (i = 0; i < n; i++) {
        scanf("%s", p[i]);
        A = p[i][0];
        nt[A - 'A'] = 1;
    }

    // Identify non-terminals
    for (i = 0; i < n; i++)
        for (j = 2; p[i][j]; j++)
            if (isupper(p[i][j]))
                nt[p[i][j] - 'A'] = 1;

    // FIRST
    do {
        change = 0;

        for (i = 0; i < n; i++) {
            A = p[i][0];
            nullable = 1;

            for (j = 2; p[i][j]; j++) {
                c = p[i][j];

                if (c == '#') {
                    change |= add(first[A-'A'], '#');
                    break;
                }

                if (!isupper(c)) {
                    change |= add(first[A-'A'], c);
                    nullable = 0;
                    break;
                }

                x = c - 'A';

                for (k = 0; k < 128; k++)
                    if (k != '#' && first[x][k])
                        change |= add(first[A-'A'], k);

                if (!first[x]['#']) {
                    nullable = 0;
                    break;
                }
            }

            if (nullable)
                change |= add(first[A-'A'], '#');
        }
    } while (change);

    // FOLLOW of start symbol
    add(follow[p[0][0]-'A'], '$');

    // FOLLOW
    do {
        change = 0;

        for (i = 0; i < n; i++) {
            A = p[i][0];

            for (j = 2; p[i][j]; j++) {
                B = p[i][j];

                if (!isupper(B))
                    continue;

                nullable = 1;

                for (k = j+1; p[i][k]; k++) {
                    c = p[i][k];

                    if (!isupper(c)) {
                        change |= add(follow[B-'A'], c);
                        nullable = 0;
                        break;
                    }

                    x = c - 'A';

                    for (int t = 0; t < 128; t++)
                        if (t != '#' && first[x][t])
                            change |= add(follow[B-'A'], t);

                    if (!first[x]['#']) {
                        nullable = 0;
                        break;
                    }
                }

                if (nullable)
                    for (k = 0; k < 128; k++)
                        if (follow[A-'A'][k])
                            change |= add(follow[B-'A'], k);
            }
        }
    } while (change);

    // Print FIRST
    printf("\nFIRST sets:\n");
    for (i = 0; i < 26; i++)
        if (nt[i]) {
            printf("FIRST(%c) = { ", 'A'+i);
            for (j = 0; j < 128; j++)
                if (first[i][j])
                    printf("%c ", j);
            printf("}\n");
        }

    // Print FOLLOW
    printf("\nFOLLOW sets:\n");
    for (i = 0; i < 26; i++)
        if (nt[i]) {
            printf("FOLLOW(%c) = { ", 'A'+i);
            for (j = 0; j < 128; j++)
                if (follow[i][j])
                    printf("%c ", j);
            printf("}\n");
        }

    return 0;
}