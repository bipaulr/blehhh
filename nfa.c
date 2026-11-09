#include <stdio.h>
int n, k, m, num_final;
int nfa_move[16][9];
int final_set;
int dfa_state[65536];
int dfa_next[65536][9];
int count;
void print_set(int set) {
    int i, first = 1;
    printf("{");
    for (i = 0; i < n; i++) {
        if (set & (1 << i)) {
            if (!first) {
                printf(",");
            }
            printf("%d", i);
            first = 0;
        }
    }
    printf("}");
}
int find(int set) {
    int d;
    for (d = 0; d < count; d++) {
        if (dfa_state[d] == set) {
            return d;
        }
    }
    return -1;
}

int main(void) {
    int i, a, d, from, to, x, target;
    char symbol;
    if (scanf("%d %d %d", &n, &k, &m) != 3 ||
        n < 1 || n > 16 || k < 1 || k > 9 || m < 0) {
        fputs("Invalid counts (1-16 states, 1-9 symbols)\n", stderr);
        return 1;
    }
    for (i = 0; i < m; i++) {
        if (scanf("%d %c %d", &from, &symbol, &to) != 3 ||
            from < 0 || from >= n || to < 0 || to >= n ||
            symbol < 'a' || symbol >= 'a' + k) {
            fputs("Invalid transition\n", stderr);
            return 1;
        }
        nfa_move[from][symbol - 'a'] |= 1 << to;
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
        final_set |= 1 << x;
    }
    dfa_state[0] = 1 << 0;
    count = 1;
    for (d = 0; d < count; d++) {
        for (a = 0; a < k; a++) {
            target = 0;
            for (i = 0; i < n; i++) {
                if (dfa_state[d] & (1 << i)) {
                    target |= nfa_move[i][a];
                }
            }
            if (find(target) == -1) {
                dfa_state[count++] = target;
            }
            dfa_next[d][a] = target;
        }
    }
    for (d = 0; d < count; d++) {
        printf("%s", (dfa_state[d] & final_set) ? "*" : " ");
        print_set(dfa_state[d]);
        for (a = 0; a < k; a++) {
            printf("  %c->", 'a' + a);
            print_set(dfa_next[d][a]);
        }
        printf("\n");
    }
    return 0;
}
