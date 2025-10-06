#include <stdio.h>
#include <limits.h> // For INT_MAX

// Define a process structure
typedef struct {
    int id;              // Process ID
    int at, bt, priority; // Arrival, Burst, Priority
    int ct, tat, wt;     // Completion, Turnaround, Waiting times
    int is_completed;    // Completion flag
} Process;

int main() {
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    if (n <= 0) return 0;

    Process p[n];
    int execution_order[n]; // To store execution sequence
    float total_tat = 0, total_wt = 0;

    // 1. Input process details
    for (int i = 0; i < n; i++) {
        printf("Enter AT, BT, and Priority for P%d: ", i);
        scanf("%d %d %d", &p[i].at, &p[i].bt, &p[i].priority);
        p[i].id = i;
        p[i].is_completed = 0;
    }

    int completed_count = 0;
    int current_time = 0;
    int exec_idx = 0; // For Gantt chart order

    // 2. Scheduling loop
    while (completed_count < n) {
        int selected_idx = -1;
        int highest_priority = INT_MAX;

        // Select the ready process with the highest priority (lowest number)
        for (int i = 0; i < n; i++) {
            if (p[i].at <= current_time && !p[i].is_completed) {
                if (selected_idx == -1 || p[i].priority < highest_priority ||
                    (p[i].priority == highest_priority && p[i].at < p[selected_idx].at)) {
                    highest_priority = p[i].priority;
                    selected_idx = i;
                }
            }
        }

        if (selected_idx == -1) {
            // CPU idle
            current_time++;
        } else {
            // Execute the selected process
            p[selected_idx].ct = current_time + p[selected_idx].bt;
            p[selected_idx].tat = p[selected_idx].ct - p[selected_idx].at;
            p[selected_idx].wt = p[selected_idx].tat - p[selected_idx].bt;
            p[selected_idx].is_completed = 1;

            total_tat += p[selected_idx].tat;
            total_wt += p[selected_idx].wt;

            execution_order[exec_idx++] = selected_idx;
            completed_count++;
            current_time = p[selected_idx].ct;
        }
    }

    // 3. Display process table
    printf("\n--- Priority Scheduling (Non-Preemptive) Results ---\n");
    printf("PID\tAT\tBT\tPri\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].id, p[i].at, p[i].bt, p[i].priority,
               p[i].ct, p[i].tat, p[i].wt);
    }

    printf("\nAverage Turnaround Time: %.2f\n", total_tat / n);
    printf("Average Waiting Time: %.2f\n", total_wt / n);

    // 4. Gantt Chart
    printf("\n--- Gantt Chart ---\n");
    printf("|");

    int last_ct = 0;
    for (int i = 0; i < exec_idx; i++) {
        int idx = execution_order[i];

        // Handle idle time
        if (p[idx].at > last_ct) {
            printf(" IDLE |");
            last_ct = p[idx].at;
        }

        printf(" P%d |", p[idx].id);
        last_ct = p[idx].ct;
    }

    printf("\n0");
    last_ct = 0;

    for (int i = 0; i < exec_idx; i++) {
        int idx = execution_order[i];
        if (p[idx].at > last_ct) {
            printf("     %d", p[idx].at);
            last_ct = p[idx].at;
        }
        printf("     %d", p[idx].ct);
        last_ct = p[idx].ct;
    }
    printf("\n");

    return 0;
}
