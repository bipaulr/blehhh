#include <stdio.h>

// Using typedef for a shorter struct name
typedef struct {
    int id;           // Original Process ID
    int at, bt;       // Arrival Time, Burst Time
    int rem_bt;       // Remaining Burst Time
    int ct, tat, wt;  // Completion, Turnaround, Waiting Time
} Process;

int main() {
    int n, tq; // n = number of processes, tq = time quantum
    printf("Enter the number of processes: ");
    scanf("%d", &n);
    printf("Enter the Time Quantum: ");
    scanf("%d", &tq);

    if (n <= 0 || tq <= 0) return 0;

    Process p[n];
    int ready_queue[100]; // A simple queue to hold process indices
    int front = 0, rear = 0;
    float total_tat = 0, total_wt = 0;
    int completed = 0, current_time = 0;

    // Gantt chart tracking arrays
    int gantt_p[100], gantt_t[100];
    int gantt_count = 0;

    // 1. Get user input for each process
    for (int i = 0; i < n; i++) {
        printf("Enter Arrival Time and Burst Time for P%d: ", i);
        scanf("%d %d", &p[i].at, &p[i].bt);
        p[i].id = i;
        p[i].rem_bt = p[i].bt; // Initially, remaining time is the full burst time
    }

    // 2. Main scheduler loop
    int visited[n]; // To track if a process has been added to the queue
    for(int i = 0; i < n; i++) visited[i] = 0;

    while (completed != n) {
        // Add all processes that have arrived by the current_time to the ready queue
        for (int i = 0; i < n; i++) {
            if (p[i].at <= current_time && p[i].rem_bt > 0 && !visited[i]) {
                ready_queue[rear++] = i;
                visited[i] = 1;
            }
        }

        if (front == rear) { // If ready queue is empty, CPU is idle
            current_time++;
            continue;
        }

        // Get the next process from the ready queue
        int idx = ready_queue[front++];
        int run_for = (p[idx].rem_bt < tq) ? p[idx].rem_bt : tq;

        // Execute the process for run_for time
        p[idx].rem_bt -= run_for;
        current_time += run_for;

        // Record this execution slice for the Gantt Chart
        gantt_p[gantt_count] = p[idx].id;
        gantt_t[gantt_count] = current_time;
        gantt_count++;

        // After execution, add newly arrived processes to the queue
         for (int i = 0; i < n; i++) {
            if (p[i].at <= current_time && p[i].rem_bt > 0 && !visited[i]) {
                ready_queue[rear++] = i;
                visited[i] = 1;
            }
        }

        if (p[idx].rem_bt > 0) {
            // If process is not finished, add it back to the end of the queue
            ready_queue[rear++] = idx;
        } else {
            // If process is finished
            p[idx].ct = current_time;
            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt = p[idx].tat - p[idx].bt;
            total_tat += p[idx].tat;
            total_wt += p[idx].wt;
            completed++;
        }
    }

    // 3. Display the results table
    printf("\n--- Round Robin Scheduling Results ---\n");
    printf("PID\tAT\tBT\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n", p[i].id, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt);
    }
    printf("\nAverage Turnaround Time: %.2f\n", total_tat / n);
    printf("Average Waiting Time: %.2f\n", total_wt / n);

    // 4. Display the Gantt Chart
    printf("\n--- Gantt Chart ---\n");
    printf("|");
    for (int i = 0; i < gantt_count; i++) {
        printf(" P%d |", gantt_p[i]);
    }
    printf("\n0");
    for (int i = 0; i < gantt_count; i++) {
        printf("    %d", gantt_t[i]);
    }
    printf("\n");

    return 0;
}