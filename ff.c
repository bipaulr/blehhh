/* FIRST and FOLLOW sets of a grammar.
   Productions are typed like S=AB (uppercase = non-terminal, # = epsilon).
   The first production's left side is the start symbol.
   Rules are applied again and again until no set grows (a fixed point). */
#include <stdio.h>
#include <ctype.h>

int n;                            /* number of productions */
char prod[20][20];                /* prod[p] = "S=AB" */
char first[128][128];             /* first[X][c] = 1 means c is in FIRST(X) */
char follow[128][128];            /* follow[X][c] = 1 means c is in FOLLOW(X) */

/* add c to a set; return 1 only if it was new */
int add(char *set, int c) {
    if (!set[c]) {
        set[c] = 1;
        return 1;
    }
    return 0;
}

void compute_first(void) {
    int changed = 1, p, k, c, all_nullable;
    char a, x, *rhs;
    while (changed) {
        changed = 0;
        for (p = 0; p < n; p++) {
            a = prod[p][0];
            rhs = prod[p] + 2;                         /* text after '=' */
            if (rhs[0] == '#') {
                changed |= add(first[(int)a], '#');
                continue;
            }
            all_nullable = 1;                          /* can the whole right side vanish? */
            for (k = 0; rhs[k]; k++) {
                x = rhs[k];
                if (!isupper((unsigned char)x)) {      /* terminal: it starts the string */
                    changed |= add(first[(int)a], x);
                    all_nullable = 0;
                    break;
                }
                for (c = 0; c < 128; c++) {            /* non-terminal: copy its FIRST minus # */
                    if (c != '#' && first[(int)x][c]) {
                        changed |= add(first[(int)a], c);
                    }
                }
                if (!first[(int)x]['#']) {             /* x cannot vanish: stop */
                    all_nullable = 0;
                    break;
                }
            }
            if (all_nullable) {
                changed |= add(first[(int)a], '#');
            }
        }
    }
}

void compute_follow(char start) {
    int changed = 1, p, k, m, c, rest_nullable;
    char a, b, y, *rhs;
    follow[(int)start]['$'] = 1;                       /* rule 1: $ follows the start symbol */
    while (changed) {
        changed = 0;
        for (p = 0; p < n; p++) {
            a = prod[p][0];
            rhs = prod[p] + 2;
            for (k = 0; rhs[k]; k++) {
                b = rhs[k];
                if (!isupper((unsigned char)b)) {      /* only non-terminals have FOLLOW */
                    continue;
                }
                rest_nullable = 1;                     /* can everything after b vanish? */
                for (m = k + 1; rhs[m]; m++) {
                    y = rhs[m];
                    if (!isupper((unsigned char)y)) {  /* rule 2: terminal right after b */
                        changed |= add(follow[(int)b], y);
                        rest_nullable = 0;
                        break;
                    }
                    for (c = 0; c < 128; c++) {        /* rule 2: FIRST of what follows b */
                        if (c != '#' && first[(int)y][c]) {
                            changed |= add(follow[(int)b], c);
                        }
                    }
                    if (!first[(int)y]['#']) {
                        rest_nullable = 0;
                        break;
                    }
                }
                if (rest_nullable) {                   /* rule 3: b can end the production */
                    for (c = 0; c < 128; c++) {
                        if (follow[(int)a][c]) {
                            changed |= add(follow[(int)b], c);
                        }
                    }
                }
            }
        }
    }
}

void print_set(const char *name, char x, char set[128][128]) {
    int c, first_item = 1;
    printf("%s(%c) = { ", name, x);
    for (c = 0; c < 128; c++) {
        if (set[(int)x][c]) {
            if (!first_item) {
                printf(", ");
            }
            if (c == '#') {
                printf("epsilon");
            } else {
                printf("%c", c);
            }
            first_item = 0;
        }
    }
    printf(" }\n");
}

int main(void) {
    int i, p;
    char seen[128] = {0};
    printf("Enter number of productions: ");
    if (scanf("%d", &n) != 1 || n < 1 || n > 20) {
        fputs("Invalid number of productions (1-20)\n", stderr);
        return 1;
    }
    printf("Enter productions (e.g. S=AB, use # for epsilon):\n");
    for (i = 0; i < n; i++) {
        if (scanf("%19s", prod[i]) != 1 || !isupper((unsigned char)prod[i][0]) ||
            prod[i][1] != '=' || prod[i][2] == '\0') {
            fputs("Invalid production (use the form A=xyz)\n", stderr);
            return 1;
        }
    }
    compute_first();
    compute_follow(prod[0][0]);                        /* first production = start symbol */

    printf("\n");
    for (p = 0; p < n; p++) {                          /* print each non-terminal once */
        if (!seen[(int)prod[p][0]]) {
            seen[(int)prod[p][0]] = 1;
            print_set("FIRST ", prod[p][0], first);
        }
    }
    printf("\n");
    for (i = 0; i < 128; i++) {
        seen[i] = 0;
    }
    for (p = 0; p < n; p++) {
        if (!seen[(int)prod[p][0]]) {
            seen[(int)prod[p][0]] = 1;
            print_set("FOLLOW", prod[p][0], follow);
        }
    }
    return 0;
}
