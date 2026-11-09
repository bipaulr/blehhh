#include <stdio.h>
int n, k, num_final;
int next_state[20][9];
int is_final[20];
int different[20][20];
int rep(int x) {
    int j = 0;
    while (different[x][j]) {
        j++;
    }
    return j;
}
int main(void) {
    int i, j, a, x, y, changed = 1;
    if (scanf("%d %d", &n, &k) != 2 ||
        n < 1 || n > 20 || k < 1 || k > 9) {
        fputs("Invalid counts (1-20 states, 1-9 symbols)\n", stderr);
        return 1;
    }
    for (i = 0; i < n; i++) {
        for (a = 0; a < k; a++) {
            if (scanf("%d", &next_state[i][a]) != 1 ||
                next_state[i][a] < 0 || next_state[i][a] >= n) {
                fputs("Invalid DFA transition\n", stderr);
                return 1;
            }
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
        for (j = 0; j < n; j++) {
            different[i][j] = (is_final[i] != is_final[j]);
        }
    }

    while (changed) {
        changed = 0;
        for (i = 0; i < n; i++) {
            for (j = 0; j < i; j++) {
                if (!different[i][j]) {
                    for (a = 0; a < k; a++) {
                        x = next_state[i][a];
                        y = next_state[j][a];
                        if (different[x][y]) {
                            different[i][j] = different[j][i] = 1;
                            changed = 1;
                            break;
                        }
                    }
                }
            }
        }
    }
    for (i = 0; i < n; i++) {
        if (rep(i) == i) {
            printf("%sq%d = {", is_final[i] ? "*" : " ", i);
            for (j = i; j < n; j++) {
                if (!different[i][j]) {
                    printf(" q%d", j);
                }
            }
            printf(" }");
            for (a = 0; a < k; a++) {
                printf("  %c->q%d", 'a' + a, rep(next_state[i][a]));
            }
            printf("\n");
        }
    }
    return 0;
}
