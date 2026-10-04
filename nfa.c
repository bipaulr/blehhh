/* Convert an NFA (no e moves) into a DFA: subset construction.
   Each DFA state is a SET of NFA states. A set is stored in one integer
   ("bitmask"): bit i is 1 when NFA state i is in the set.
       set | (1 << i)       adds state i to the set
       set & (1 << i)       is non-zero when state i is in the set */
#include <stdio.h>
int n, k, m, num_final;
int nfa_move[16][9];           /* nfa_move[i][a] = set of states reached from i on symbol a */
int final_set;                 /* set of NFA final states                                   */
int dfa_state[65536];          /* dfa_state[d] = the set of NFA states that DFA state d is  */
int dfa_next[65536][9];        /* dfa_next[d][a] = set reached from d on symbol a           */
int count;                     /* number of DFA states found so far                         */
/* print a set like {0,2}; empty set prints {} */
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
/* return the DFA state number of this set, or -1 if it is new */
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
    /* the DFA start state is the set {0} */
    dfa_state[0] = 1 << 0;
    count = 1;
    /* process DFA states one by one; new ones are added to the end of the list */
    for (d = 0; d < count; d++) {
        for (a = 0; a < k; a++) {
            /* target = union of the moves of every NFA state inside this DFA state */
            target = 0;
            for (i = 0; i < n; i++) {
                if (dfa_state[d] & (1 << i)) {
                    target |= nfa_move[i][a];
                }
            }
            /* Include the empty set as a dead state, with self-loops. */
            if (find(target) == -1) {
                dfa_state[count++] = target;
            }
            dfa_next[d][a] = target;
        }
    }
    /* print the DFA table; * marks a final state */
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
