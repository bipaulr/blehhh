#include <stdio.h>
#include <stdlib.h> // Required for the abs() function

int main() {
    int n, head, disk_size;

    // 1. Get user inputs
    printf("Enter the number of disk requests: ");
    scanf("%d", &n);
    int requests[n];
    printf("Enter the disk request queue (track numbers):\n");
    for (int i = 0; i < n; i++) scanf("%d", &requests[i]);

    printf("Enter the initial position of the disk head: ");
    scanf("%d", &head);

    printf("Enter the total disk size (e.g., 200 for tracks 0-199): ");
    scanf("%d", &disk_size);

    // 2. Sort the request array using Bubble Sort
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (requests[j] > requests[j + 1]) {
                int temp = requests[j];
                requests[j] = requests[j + 1];
                requests[j + 1] = temp;
            }
        }
    }

    // 3. Find the position where the head should start servicing requests
    int start_pos = 0;
    while (start_pos < n && requests[start_pos] < head) {
        start_pos++;
    }

    int total_head_movement = 0;
    int current_head = head;

    printf("\n--- C-SCAN Seek Sequence ---\n");
    printf("%d", current_head);

    // 4. Execute the C-SCAN logic (always moves toward the high end first)
    // Service all requests to the right of the head
    for (int i = start_pos; i < n; i++) {
        total_head_movement += abs(current_head - requests[i]);
        current_head = requests[i];
        printf(" -> %d", current_head);
    }

    // Move from the last request to the very end of the disk
    total_head_movement += abs(current_head - (disk_size - 1));
    current_head = disk_size - 1;
    printf(" -> %d", current_head);

    // Jump from the end to the beginning of the disk
    total_head_movement += abs(current_head - 0);
    current_head = 0;
    printf(" -> %d", current_head);

    // Service the remaining requests from the beginning to the start position
    for (int i = 0; i < start_pos; i++) {
        total_head_movement += abs(current_head - requests[i]);
        current_head = requests[i];
        printf(" -> %d", current_head);
    }
    printf("\n");

    // 5. Display the final result
    printf("\nTotal head movement (seek time): %d cylinders\n", total_head_movement);

    return 0;
}