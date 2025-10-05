#include <stdio.h>

int main() {
    int n; // Number of pages in reference string
    printf("Enter the number of pages in the reference string: ");
    scanf("%d", &n);
    int pages[n];
    printf("Enter the reference string (e.g., 7 0 1 2 ...):\n");
    for (int i = 0; i < n; i++) scanf("%d", &pages[i]);

    int capacity; // Number of frames
    printf("Enter the number of frames: ");
    scanf("%d", &capacity);

    int frames[capacity];
    // Initialize all frames to -1 (representing empty)
    for (int i = 0; i < capacity; i++) frames[i] = -1;

    int page_faults = 0;
    int frame_index = 0; // Pointer to the oldest page (the one to be replaced)

    printf("\nIncoming Page\tFrames\n");
    // Process the reference string
    for (int i = 0; i < n; i++) {
        int current_page = pages[i];
        int is_hit = 0;

        // Check if the page is already in a frame (a "hit")
        for (int j = 0; j < capacity; j++) {
            if (frames[j] == current_page) {
                is_hit = 1;
                break;
            }
        }

        // If it's a "miss" (page fault)
        if (!is_hit) {
            page_faults++;
            frames[frame_index] = current_page;
            // Move the pointer to the next frame in a circular manner
            frame_index = (frame_index + 1) % capacity;
        }

        // Print the state of the frames
        printf("%d\t\t", current_page);
        for (int j = 0; j < capacity; j++) {
            if (frames[j] != -1) {
                printf("%d ", frames[j]);
            } else {
                printf("- ");
            }
        }
        printf(is_hit ? "(Hit)\n" : "(Fault)\n");
    }

    printf("\nTotal Page Faults: %d\n", page_faults);
    printf("Total Page Hits: %d\n", n - page_faults);

    return 0;
}