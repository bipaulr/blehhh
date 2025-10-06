#include <stdio.h>
#include <stdlib.h> // For abs()

int main() {
    int n, head, disk_size, direction;

    // 1. User inputs
    printf("Enter the number of disk requests: ");
    scanf("%d", &n);
    if (n <= 0) return 0;

    int requests[n];
    printf("Enter the disk request queue (track numbers):\n");
    for (int i = 0; i < n; i++) scanf("%d", &requests[i]);

    printf("Enter the initial position of the disk head: ");
    scanf("%d", &head);

    printf("Enter the total disk size (e.g., 200 for tracks 0-199): ");
    scanf("%d", &disk_size);

    printf("Enter the initial direction (1 = right/high, 0 = left/low): ");
    scanf("%d", &direction);

    // 2. Sort the request array
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (requests[j] > requests[j + 1]) {
                int temp = requests[j];
                requests[j] = requests[j + 1];
                requests[j + 1] = temp;
            }
        }
    }

    // 3. Find start position
    int start_pos = 0;
    while (start_pos < n && requests[start_pos] < head) start_pos++;

    int total_head_movement = 0;
    int current_head = head;

    printf("\n--- SCAN Seek Sequence ---\n");
    printf("%d", current_head);

    if (direction == 1) { // Moving right
        // Service requests to the right
        for (int i = start_pos; i < n; i++) {
            total_head_movement += abs(current_head - requests[i]);
            current_head = requests[i];
            printf(" -> %d", current_head);
        }
        // Move to end if last request not at end
        if (current_head != disk_size - 1) {
            total_head_movement += abs(current_head - (disk_size - 1));
            current_head = disk_size - 1;
            printf(" -> %d", current_head);
        }
        // Service requests to the left
        for (int i = start_pos - 1; i >= 0; i--) {
            total_head_movement += abs(current_head - requests[i]);
            current_head = requests[i];
            printf(" -> %d", current_head);
        }
    } else { // Moving left
        // Service requests to the left
        for (int i = start_pos - 1; i >= 0; i--) {
            total_head_movement += abs(current_head - requests[i]);
            current_head = requests[i];
            printf(" -> %d", current_head);
        }
        // Move to start if last request not at 0
        if (current_head != 0) {
            total_head_movement += abs(current_head - 0);
            current_head = 0;
            printf(" -> %d", current_head);
        }
        // Service requests to the right
        for (int i = start_pos; i < n; i++) {
            total_head_movement += abs(current_head - requests[i]);
            current_head = requests[i];
            printf(" -> %d", current_head);
        }
    }
    printf("\n");

    printf("\nTotal head movement (seek time): %d cylinders\n", total_head_movement);

    return 0;
}
