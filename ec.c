#include <stdio.h>
int num_states, num_transitions;
int reach[20][20];
int main(void) {
    int from, to, i, j, k;
    char symbol;
    scanf("%d %d", &num_states, &num_transitions);
    for (i = 0; i < num_transitions; i++) {
        scanf("%d %c %d", &from, &symbol, &to);
        if (symbol == 'e') {
            reach[from][to] = 1;
        }
    }
    for (i = 0; i < num_states; i++) {
        reach[i][i] = 1;
    }
    for (k = 0; k < num_states; k++) {
        for (i = 0; i < num_states; i++) {
            for (j = 0; j < num_states; j++) {
                if (reach[i][k] && reach[k][j]) {
                    reach[i][j] = 1;
                }
            }
        }
    }
    for (i = 0; i < num_states; i++) {
        printf("e-closure(q%d) = {", i);
        for (j = 0; j < num_states; j++) {
            if (reach[i][j]) {
                printf(" q%d", j);
            }
        }
        printf(" }\n");
    }
    return 0;
}
