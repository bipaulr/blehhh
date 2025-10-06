#include <stdio.h>
#include <stdlib.h> // For abs()

int main() {
    int n, head;
    printf("Enter the number of disk requests: ");
    scanf("%d", &n);

    if (n <= 0) {
        printf("No disk requests entered.\n");
        return 0;
    }

    int requests[n];
    printf("Enter the disk request queue (track numbers):\n");
    for (int i = 0; i < n; i++)
        scanf("%d", &requests[i]);

    printf("Enter the initial position of the disk head: ");
    scanf("%d", &head);

    int total_head_movement = 0;
    int current_head = head;

    printf("\n--- FCFS Disk Scheduling ---\nSeek Sequence:\n");
    printf("%d", current_head);

    for (int i = 0; i < n; i++) {
        total_head_movement += abs(current_head - requests[i]);
        current_head = requests[i];
        printf(" -> %d", current_head);
    }

    printf("\n\nTotal Head Movement = %d cylinders\n", total_head_movement);

    return 0;
}
