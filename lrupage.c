#include <stdio.h>
#include <limits.h>

int main() {
    int n; // Number of pages in reference string
    printf("Enter the number of pages in the reference string: ");
    scanf("%d", &n);

    int pages[n];
    printf("Enter the reference string (e.g., 7 0 1 2 ...):\n");
    for (int i = 0; i < n; i++)
        scanf("%d", &pages[i]);

    int capacity; // Number of frames
    printf("Enter the number of frames: ");
    scanf("%d", &capacity);

    // ✅ Declare arrays *after* capacity is known
    int frames[capacity];
    int time[capacity]; // To store last used time of each frame

    // Initialize frames as empty
    for (int i = 0; i < capacity; i++) {
        frames[i] = -1;
        time[i] = 0;
    }

    int page_faults = 0;
    int timer = 0; // To simulate time for LRU

    printf("\nIncoming Page\tFrames\n");

    for (int i = 0; i < n; i++) {
        int current_page = pages[i];
        int is_hit = 0;

        // Check if page already in a frame (HIT)
        for (int j = 0; j < capacity; j++) {
            if (frames[j] == current_page) {
                is_hit = 1;
                time[j] = ++timer; // Update usage time
                break;
            }
        }

        if (!is_hit) {
            // Page fault occurred
            page_faults++;

            // Find an empty frame (if available)
            int empty_index = -1;
            for (int j = 0; j < capacity; j++) {
                if (frames[j] == -1) {
                    empty_index = j;
                    break;
                }
            }

            if (empty_index != -1) {
                // Use the empty frame
                frames[empty_index] = current_page;
                time[empty_index] = ++timer;
            } else {
                // Replace least recently used page
                int lru_index = 0;
                int min_time = INT_MAX;

                for (int j = 0; j < capacity; j++) {
                    if (time[j] < min_time) {
                        min_time = time[j];
                        lru_index = j;
                    }
                }

                frames[lru_index] = current_page;
                time[lru_index] = ++timer;
            }
        }

        // Print frame status
        printf("%d\t\t", current_page);
        for (int j = 0; j < capacity; j++) {
            if (frames[j] != -1)
                printf("%d ", frames[j]);
            else
                printf("- ");
        }
        printf(is_hit ? "(Hit)\n" : "(Fault)\n");
    }

    printf("\nTotal Page Faults: %d\n", page_faults);
    printf("Total Page Hits: %d\n", n - page_faults);

    return 0;
}
