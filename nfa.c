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
    scanf("%d %d %d", &n, &k, &m);
    for (i = 0; i < m; i++) {
        scanf("%d %c %d", &from, &symbol, &to);
        nfa_move[from][symbol - 'a'] |= 1 << to;
    }
    scanf("%d", &num_final);
    for (i = 0; i < num_final; i++) {
        scanf("%d", &x);
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
