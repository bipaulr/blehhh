#include <stdio.h>

typedef struct {
    int id;           // Process ID
    int at, bt;       // Arrival, Burst Time
    int rem_bt;       // Remaining Burst Time
    int ct, tat, wt;  // Completion, Turnaround, Waiting Time
} Process;

int main() {
    int n, tq;
    printf("Enter the number of processes: ");
    scanf("%d", &n);
    printf("Enter the Time Quantum: ");
    scanf("%d", &tq);

    if (n <= 0 || tq <= 0) return 0;

    Process p[n];
    for (int i = 0; i < n; i++) {
        printf("Enter Arrival Time and Burst Time for P%d: ", i);
        scanf("%d %d", &p[i].at, &p[i].bt);
        p[i].id = i;
        p[i].rem_bt = p[i].bt;
    }

    int ready_queue[100];
    int front = 0, rear = 0;
    int current_time = 0, completed = 0;
    float total_tat = 0, total_wt = 0;

    int gantt_p[200], gantt_t[200];
    int gantt_count = 0;

    // Initially, add any processes that arrive at time 0
    int visited[n];
    for (int i = 0; i < n; i++) visited[i] = 0;
    for (int i = 0; i < n; i++) {
        if (p[i].at == 0) {
            ready_queue[rear++] = i;
            visited[i] = 1;
        }
    }

    while (completed < n) {
        if (front == rear) { // No process ready, CPU idle
            gantt_p[gantt_count] = -1; // IDLE
            current_time++;
            gantt_t[gantt_count] = current_time;
            gantt_count++;

            // Check for any new arrivals during idle
            for (int i = 0; i < n; i++) {
                if (!visited[i] && p[i].at <= current_time) {
                    ready_queue[rear++] = i;
                    visited[i] = 1;
                }
            }
            continue;
        }

        int idx = ready_queue[front++];
        int run_for = (p[idx].rem_bt < tq) ? p[idx].rem_bt : tq;

        p[idx].rem_bt -= run_for;
        current_time += run_for;

        // Record Gantt chart entry
        gantt_p[gantt_count] = p[idx].id;
        gantt_t[gantt_count] = current_time;
        gantt_count++;

        // Add any newly arrived processes up to current_time
        for (int i = 0; i < n; i++) {
            if (!visited[i] && p[i].at <= current_time) {
                ready_queue[rear++] = i;
                visited[i] = 1;
            }
        }

        if (p[idx].rem_bt > 0) {
            ready_queue[rear++] = idx; // Re-add to end of queue
        } else {
            p[idx].ct = current_time;
            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt = p[idx].tat - p[idx].bt;
            total_tat += p[idx].tat;
            total_wt += p[idx].wt;
            completed++;
        }
    }

    // Results table
    printf("\n--- Round Robin Scheduling Results ---\n");
    printf("PID\tAT\tBT\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].id, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt);
    }

    printf("\nAverage Turnaround Time: %.2f\n", total_tat / n);
    printf("Average Waiting Time: %.2f\n", total_wt / n);

    // Gantt Chart
    printf("\n--- Gantt Chart ---\n");
    printf("|");
    for (int i = 0; i < gantt_count; i++) {
        if (gantt_p[i] == -1)
            printf(" IDLE |");
        else
            printf(" P%d |", gantt_p[i]);
    }
    printf("\n0");
    for (int i = 0; i < gantt_count; i++) {
        printf("    %d", gantt_t[i]);
    }
    printf("\n");

    return 0;
}
