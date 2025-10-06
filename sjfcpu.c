#include <stdio.h>
#include <limits.h>

typedef struct {
    int id;        // Process ID
    int at, bt;    // Arrival Time, Burst Time
    int ct, tat, wt; // Completion, Turnaround, Waiting Time
    int is_completed;
} Process;

int main() {
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);
    if (n <= 0) return 0;

    Process p[n];
    int execution_order[n];
    float total_tat = 0, total_wt = 0;

    // 1. Input
    for (int i = 0; i < n; i++) {
        printf("Enter Arrival Time and Burst Time for P%d: ", i);
        scanf("%d %d", &p[i].at, &p[i].bt);
        p[i].id = i;
        p[i].is_completed = 0;
    }

    int completed = 0;
    int current_time = 0;

    // 2. Scheduler loop
    while (completed < n) {
        int shortest_idx = -1;
        int min_bt = INT_MAX;

        for (int i = 0; i < n; i++) {
            if (!p[i].is_completed && p[i].at <= current_time) {
                if (p[i].bt < min_bt) {
                    min_bt = p[i].bt;
                    shortest_idx = i;
                } else if (p[i].bt == min_bt) {
                    // Tie-breaker: earlier arrival time
                    if (p[i].at < p[shortest_idx].at) {
                        shortest_idx = i;
                    }
                }
            }
        }

        if (shortest_idx == -1) {
            current_time++; // CPU idle
        } else {
            p[shortest_idx].ct = current_time + p[shortest_idx].bt;
            p[shortest_idx].tat = p[shortest_idx].ct - p[shortest_idx].at;
            p[shortest_idx].wt = p[shortest_idx].tat - p[shortest_idx].bt;
            p[shortest_idx].is_completed = 1;

            total_tat += p[shortest_idx].tat;
            total_wt += p[shortest_idx].wt;

            execution_order[completed] = shortest_idx;
            completed++;
            current_time = p[shortest_idx].ct;
        }
    }

    // 3. Display results
    printf("\n--- SJF Scheduling Results ---\n");
    printf("PID\tAT\tBT\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\n", p[i].id, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt);
    }
    printf("\nAverage Turnaround Time: %.2f\n", total_tat / n);
    printf("Average Waiting Time: %.2f\n", total_wt / n);

    // 4. Gantt Chart
    printf("\n--- Gantt Chart ---\n");
    int last_time = 0;
    printf("|");
    for (int i = 0; i < n; i++) {
        int idx = execution_order[i];
        if (p[idx].at > last_time) {
            printf(" IDLE |");
        }
        printf(" P%d |", p[idx].id);
        last_time = p[idx].ct;
    }
    printf("\n0");

    last_time = 0;
    for (int i = 0; i < n; i++) {
        int idx = execution_order[i];
        if (p[idx].at > last_time) printf("    %d", p[idx].at);
        printf("    %d", p[idx].ct);
        last_time = p[idx].ct;
    }
    printf("\n");

    return 0;
}
