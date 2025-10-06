#include <stdio.h>

// Define Process structure
typedef struct {
    int id;         // Process ID
    int at, bt;     // Arrival Time, Burst Time
    int ct, tat, wt;// Completion, Turnaround, Waiting Time
} Process;

int main() {
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    if (n <= 0) return 0;

    Process p[n];
    float total_tat = 0, total_wt = 0;

    // 1. Input arrival and burst times
    for (int i = 0; i < n; i++) {
        printf("Enter Arrival Time and Burst Time for P%d: ", i);
        scanf("%d %d", &p[i].at, &p[i].bt);
        p[i].id = i;
    }

    // 2. Sort processes by arrival time
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (p[j].at > p[j + 1].at) {
                Process temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }

    // 3. Compute CT, TAT, WT
    int current_time = 0;
    for (int i = 0; i < n; i++) {
        if (current_time < p[i].at)
            current_time = p[i].at;  // Handle idle time

        p[i].ct = current_time + p[i].bt;
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;

        total_tat += p[i].tat;
        total_wt += p[i].wt;
        current_time = p[i].ct;
    }

    // 4. Display Table
    printf("\n--- FCFS Scheduling Results ---\n");
    printf("PID\tAT\tBT\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].id, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt);
    }

    printf("\nAverage Turnaround Time: %.2f\n", total_tat / n);
    printf("Average Waiting Time: %.2f\n", total_wt / n);

    // 5. Gantt Chart
    printf("\n--- Gantt Chart ---\n");
    int last_ct = 0;
    printf("|");
    for (int i = 0; i < n; i++) {
        if (p[i].at > last_ct)
            printf(" IDLE |");
        printf(" P%d |", p[i].id);
        last_ct = p[i].ct;
    }

    printf("\n0");
    last_ct = 0;
    for (int i = 0; i < n; i++) {
        if (p[i].at > last_ct)
            printf("     %d", p[i].at);
        printf("     %d", p[i].ct);
        last_ct = p[i].ct;
    }
    printf("\n");

    return 0;
}
