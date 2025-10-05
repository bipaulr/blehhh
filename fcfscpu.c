#include <stdio.h>

// Using typedef for a shorter struct name
typedef struct {
    int id;         // Original Process ID
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

    // 1. Get user input for each process
    for (int i = 0; i < n; i++) {
        printf("Enter Arrival Time and Burst Time for P%d: ", i);
        scanf("%d %d", &p[i].at, &p[i].bt);
        p[i].id = i;
    }

    // 2. Sort processes by arrival time (Bubble Sort)
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (p[j].at > p[j + 1].at) {
                Process temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }

    // 3. Calculate completion, turnaround, and waiting times
    int current_time = 0;
    for (int i = 0; i < n; i++) {
        // Handle idle time if the CPU is free before this process arrives
        if (current_time < p[i].at) {
            current_time = p[i].at;
        }

        p[i].ct = current_time + p[i].bt;
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;

        total_tat += p[i].tat;
        total_wt += p[i].wt;
        current_time = p[i].ct; // Move time to the completion of the current process
    }

    // 4. Display the results table (in order of execution)
    printf("\n--- FCFS Scheduling Results ---\n");
    printf("PID\tAT\tBT\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n", p[i].id, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt);
    }
    printf("\nAverage Turnaround Time: %.2f\n", total_tat / n);
    printf("Average Waiting Time: %.2f\n", total_wt / n);

    // 5. Display the Gantt Chart
    printf("\n--- Gantt Chart ---\n");
    int last_ct = 0;
    printf("|");
    for (int i = 0; i < n; i++) {
        if (p[i].at > last_ct) {
            printf(" IDLE |"); // Show idle time if there's a gap
        }
        printf(" P%d |", p[i].id);
        last_ct = p[i].ct;
    }
    printf("\n0");

    last_ct = 0;
    for (int i = 0; i < n; i++) {
        if (p[i].at > last_ct) {
            printf("     %d", p[i].at); // Timestamp for end of idle time
        }
        printf("     %d", p[i].ct);     // Timestamp for completion of process
        last_ct = p[i].ct;
    }
    printf("\n");

    return 0;
}