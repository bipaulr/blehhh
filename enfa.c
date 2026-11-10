#include <stdio.h>
int n, k, m, num_final;
int closure[16][16];
int move[16][9][16];
int is_final[16];
int main(void) {
    int i, j, p, r, s, a, from, to, x;
    char symbol;
    int result[16];
    scanf("%d %d %d", &n, &k, &m);
    for (i = 0; i < m; i++) {
        scanf("%d %c %d", &from, &symbol, &to);
        if (symbol == 'e') {
            closure[from][to] = 1;
        } else {
            move[from][symbol - 'a'][to] = 1;
        }
    }
    scanf("%d", &num_final);
    for (i = 0; i < num_final; i++) {
        scanf("%d", &x);
        is_final[x] = 1;
    }
    for (i = 0; i < n; i++) {
        closure[i][i] = 1;
    }
    for (x = 0; x < n; x++) {
        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {
                if (closure[i][x] && closure[x][j]) {
                    closure[i][j] = 1;
                }
            }
        }
    }

    for (i = 0; i < n; i++) {
        for (a = 0; a < k; a++) {
            for (s = 0; s < n; s++) {
                result[s] = 0;
            }
            for (p = 0; p < n; p++) {
                if (closure[i][p]) {
                    for (r = 0; r < n; r++) {
                        if (move[p][a][r]) {
                            for (s = 0; s < n; s++) {
                                if (closure[r][s]) {
                                    result[s] = 1;
                                }
                            }
                        }
                    }
                }
            }
            printf("d'(q%d,%c) = {", i, 'a' + a);
            for (s = 0; s < n; s++) {
                if (result[s]) {
                    printf(" q%d", s);
                }
            }
            printf(" }\n");
        }
    }
    printf("Final states:");
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (closure[i][j] && is_final[j]) {
                printf(" q%d", i);
                break;
            }
        }
    }
    printf("\n");
    return 0;
}
