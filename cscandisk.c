#include <stdio.h>
#include <stdlib.h> // for abs()

int main() {
    int n, head, disk_size;

    // 1. Get inputs
    printf("Enter the number of disk requests: ");
    scanf("%d", &n);
    int requests[n];

    printf("Enter the disk request queue (track numbers):\n");
    for (int i = 0; i < n; i++)
        scanf("%d", &requests[i]);

    printf("Enter the initial head position: ");
    scanf("%d", &head);

    printf("Enter the total disk size (e.g., 200 for tracks 0-199): ");
    scanf("%d", &disk_size);

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

    // 3. Find where the head is in sorted order
    int start_pos = 0;
    while (start_pos < n && requests[start_pos] < head)
        start_pos++;

    int total_movement = 0;
    int current = head;

    printf("\n--- C-SCAN Seek Sequence ---\n");
    printf("%d", current);

    // 4. Move toward higher end
    for (int i = start_pos; i < n; i++) {
        total_movement += abs(current - requests[i]);
        current = requests[i];
        printf(" -> %d", current);
    }

    // 5. Move from last request to end (disk_size - 1)
    if (current != disk_size - 1) {
        total_movement += abs(current - (disk_size - 1));
        current = disk_size - 1;
        printf(" -> %d", current);
    }

    // 6. Jump to beginning (NOT counted in seek time)
    if (start_pos > 0) {
        current = 0;
        printf(" -> %d", current);
    }

    // 7. Service remaining requests on left side
    for (int i = 0; i < start_pos; i++) {
        total_movement += abs(current - requests[i]);
        current = requests[i];
        printf(" -> %d", current);
    }

    printf("\n\nTotal head movement (seek time): %d cylinders\n", total_movement);
    return 0;
}
