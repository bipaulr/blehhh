#include <stdio.h>
#include <limits.h> // Required for INT_MAX

// Using typedef for a shorter struct name
typedef struct {
    int id;             // Process ID
    int at, bt;         // Arrival Time, Burst Time
    int ct, tat, wt;    // Completion, Turnaround, Waiting Time
    int is_completed;   // Flag to check if process is done (0 for no, 1 for yes)
} Process;

int main() {
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    if (n <= 0) return 0;

    Process p[n];
    int execution_order[n]; // To store the sequence of executed processes for the Gantt Chart
    float total_tat = 0, total_wt = 0;

    // 1. Get user input for each process
    for (int i = 0; i < n; i++) {
        printf("Enter Arrival Time and Burst Time for P%d: ", i);
        scanf("%d %d", &p[i].at, &p[i].bt);
        p[i].id = i;
        p[i].is_completed = 0;
    }

    int completed_count = 0;
    int current_time = 0;

    // 2. Main scheduler loop
    while (completed_count < n) {
        int shortest_idx = -1;
        int min_burst = INT_MAX;

        // Find the available process with the shortest burst time
        for (int i = 0; i < n; i++) {
            if (p[i].at <= current_time && !p[i].is_completed) {
                if (p[i].bt < min_burst) {
                    min_burst = p[i].bt;
                    shortest_idx = i;
                }
            }
        }

        if (shortest_idx == -1) {
            // No process is available, CPU is idle
            current_time++;
        } else {
            // Execute the shortest available process
            p[shortest_idx].ct = current_time + p[shortest_idx].bt;
            p[shortest_idx].tat = p[shortest_idx].ct - p[shortest_idx].at;
            p[shortest_idx].wt = p[shortest_idx].tat - p[shortest_idx].bt;
            p[shortest_idx].is_completed = 1;

            total_tat += p[shortest_idx].tat;
            total_wt += p[shortest_idx].wt;

            execution_order[completed_count] = shortest_idx;
            completed_count++;
            current_time = p[shortest_idx].ct; // Move time to the completion of this process
        }
    }

    // 3. Display the results table
    printf("\n--- SJF Scheduling Results ---\n");
    printf("PID\tAT\tBT\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\n", p[i].id, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt);
    }
    printf("\nAverage Turnaround Time: %.2f\n", total_tat / n);
    printf("Average Waiting Time: %.2f\n", total_wt / n);

    // 4. Display the Gantt Chart
    printf("\n--- Gantt Chart ---\n");
    int last_ct = 0;
    printf("|");
    for (int i = 0; i < n; i++) {
        int current_p_idx = execution_order[i];
        if (p[current_p_idx].at > last_ct) {
            printf(" IDLE |"); // Show idle time if there's a gap
        }
        printf(" P%d |", p[current_p_idx].id);
        last_ct = p[current_p_idx].ct;
    }
    printf("\n0");

    last_ct = 0;
    for (int i = 0; i < n; i++) {
        int current_p_idx = execution_order[i];
        if (p[current_p_idx].at > last_ct) {
            printf("     %d", p[current_p_idx].at); // Timestamp for end of idle time
        }
        printf("     %d", p[current_p_idx].ct); // Timestamp for completion of process
        last_ct = p[current_p_idx].ct;
    }
    printf("\n");

    return 0;
}