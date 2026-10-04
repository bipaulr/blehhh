/* Convert an NFA with e moves into an NFA without e moves.
   New move:  q --a--> s  exists if we can go  q  =e=>  p  --a-->  r  =e=>  s
   (any number of e moves, then one real symbol a, then any number of e moves). */
#include <stdio.h>
int n, k, m, num_final;
int closure[16][16];     /* closure[i][j] = 1: state j is in the e-closure of state i     */
int move[16][9][16];     /* move[i][a][j] = 1: original NFA has i --a--> j (a=0 means 'a') */
int is_final[16];        /* is_final[i] = 1: state i is final in the original NFA          */
int main(void) {
    int i, j, p, r, s, a, from, to, x;
    char symbol;
    int result[16];
    /* e is reserved for epsilon; the ordinary alphabet is a through d. */
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
    /* step 1: e-closure of every state (same Warshall idea as program 2) */
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

    /* step 2: new moves */
    for (i = 0; i < n; i++) {
        for (a = 0; a < k; a++) {
            for (s = 0; s < n; s++) {
                result[s] = 0;
            }
            for (p = 0; p < n; p++) {
                if (closure[i][p]) {                       /* p reachable from i by e moves */
                    for (r = 0; r < n; r++) {
                        if (move[p][a][r]) {               /* read symbol a, land on r      */
                            for (s = 0; s < n; s++) {
                                if (closure[r][s]) {       /* then e moves from r           */
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
    /* step 3: q is final if its e-closure contains an old final state */
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
