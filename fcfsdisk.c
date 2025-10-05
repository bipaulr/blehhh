#include <stdio.h>
#include <stdlib.h> // Required for the abs() function

int main() {
    int n, head;
    printf("Enter the number of disk requests: ");
    scanf("%d", &n);

    if (n <= 0) {
        printf("No requests to process.\n");
        return 0;
    }

    int requests[n]; // Array to store the track numbers of disk requests

    // 1. Get user input for the request queue and initial head position
    printf("Enter the disk request queue (track numbers):\n");
    for (int i = 0; i < n; i++) {
        scanf("%d", &requests[i]);
    }

    printf("Enter the initial position of the disk head: ");
    scanf("%d", &head);

    int total_head_movement = 0;
    int current_head = head;

    // 2. Calculate total head movement and print the seek sequence
    printf("\n--- FCFS Seek Sequence ---\n");
    printf("%d", current_head); // Start from the initial head position

    for (int i = 0; i < n; i++) {
        // Calculate the distance to the next request
        total_head_movement += abs(current_head - requests[i]);
        // Move the head to the new position
        current_head = requests[i];
        // Print the new position in the sequence
        printf(" -> %d", current_head);
    }
    printf("\n");

    // 3. Display the final result
    printf("\nTotal head movement (seek time): %d cylinders\n", total_head_movement);

    return 0;
}