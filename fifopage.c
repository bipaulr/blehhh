#include <stdio.h>

int main() {
    int n; // Number of pages in the reference string
    printf("Enter the number of pages in the reference string: ");
    scanf("%d", &n);

    if (n <= 0) {
        printf("Invalid number of pages.\n");
        return 0;
    }

    int pages[n];
    printf("Enter the reference string (space-separated):\n");
    for (int i = 0; i < n; i++)
        scanf("%d", &pages[i]);

    int capacity; // Number of frames
    printf("Enter the number of frames: ");
    scanf("%d", &capacity);

    if (capacity <= 0) {
        printf("Invalid number of frames.\n");
        return 0;
    }

    int frames[capacity];
    for (int i = 0; i < capacity; i++)
        frames[i] = -1; // Initialize frames as empty

    int page_faults = 0;
    int frame_index = 0; // FIFO pointer

    printf("\nIncoming Page\tFrames\t\tStatus\n");
    printf("--------------------------------------\n");

    // Process each page in the reference string
    for (int i = 0; i < n; i++) {
        int current_page = pages[i];
        int is_hit = 0;

        // Check for page hit
        for (int j = 0; j < capacity; j++) {
            if (frames[j] == current_page) {
                is_hit = 1;
                break;
            }
        }

        // If it's a page fault (miss)
        if (!is_hit) {
            frames[frame_index] = current_page;
            frame_index = (frame_index + 1) % capacity; // FIFO circular replacement
            page_faults++;
        }

        // Print the frame contents
        printf("%d\t\t", current_page);
        for (int j = 0; j < capacity; j++) {
            if (frames[j] != -1)
                printf("%d ", frames[j]);
            else
                printf("- ");
        }
        printf("\t\t%s\n", is_hit ? "Hit" : "Fault");
    }

    printf("\nTotal Page Faults: %d\n", page_faults);
    printf("Total Page Hits: %d\n", n - page_faults);
    printf("Page Fault Rate: %.2f%%\n", (page_faults * 100.0) / n);

    return 0;
}
