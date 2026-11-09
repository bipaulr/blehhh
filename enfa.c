#include <stdio.h>
int n, k, m, num_final;
int closure[16][16];
int move[16][9][16];
int is_final[16];
int main(void) {
    int i, j, p, r, s, a, from, to, x;
    char symbol;
    int result[16];
    if (scanf("%d %d %d", &n, &k, &m) != 3 ||
        n < 1 || n > 16 || k < 1 || k > 4 || m < 0) {
        fputs("Invalid counts (1-16 states, 1-4 symbols a-d)\n", stderr);
        return 1;
    }
    for (i = 0; i < m; i++) {
        if (scanf("%d %c %d", &from, &symbol, &to) != 3 ||
            from < 0 || from >= n || to < 0 || to >= n ||
            (symbol != 'e' && (symbol < 'a' || symbol >= 'a' + k))) {
            fputs("Invalid transition\n", stderr);
            return 1;
        }
        if (symbol == 'e') {
            closure[from][to] = 1;
        } else {
            move[from][symbol - 'a'][to] = 1;
        }
    }
    if (scanf("%d", &num_final) != 1 || num_final < 0 || num_final > n) {
        fputs("Invalid final-state count\n", stderr);
        return 1;
    }
    for (i = 0; i < num_final; i++) {
        if (scanf("%d", &x) != 1 || x < 0 || x >= n) {
            fputs("Invalid final state\n", stderr);
            return 1;
        }
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
