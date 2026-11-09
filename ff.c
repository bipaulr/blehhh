#include <stdio.h>
#include <ctype.h>

int n;
char prod[20][20];
char first[128][128];
char follow[128][128];

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
            rhs = prod[p] + 2;
            if (rhs[0] == '#') {
                changed |= add(first[(int)a], '#');
                continue;
            }
            all_nullable = 1;
            for (k = 0; rhs[k]; k++) {
                x = rhs[k];
                if (!isupper((unsigned char)x)) {
                    changed |= add(first[(int)a], x);
                    all_nullable = 0;
                    break;
                }
                for (c = 0; c < 128; c++) {
                    if (c != '#' && first[(int)x][c]) {
                        changed |= add(first[(int)a], c);
                    }
                }
                if (!first[(int)x]['#']) {
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
    follow[(int)start]['$'] = 1;
    while (changed) {
        changed = 0;
        for (p = 0; p < n; p++) {
            a = prod[p][0];
            rhs = prod[p] + 2;
            for (k = 0; rhs[k]; k++) {
                b = rhs[k];
                if (!isupper((unsigned char)b)) {
                    continue;
                }
                rest_nullable = 1;
                for (m = k + 1; rhs[m]; m++) {
                    y = rhs[m];
                    if (!isupper((unsigned char)y)) {
                        changed |= add(follow[(int)b], y);
                        rest_nullable = 0;
                        break;
                    }
                    for (c = 0; c < 128; c++) {
                        if (c != '#' && first[(int)y][c]) {
                            changed |= add(follow[(int)b], c);
                        }
                    }
                    if (!first[(int)y]['#']) {
                        rest_nullable = 0;
                        break;
                    }
                }
                if (rest_nullable) {
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
    compute_follow(prod[0][0]);

    printf("\n");
    for (p = 0; p < n; p++) {
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
