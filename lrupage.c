#include <stdio.h>
#include <limits.h>

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
    int time[capacity]; // To store the time of last use for each frame
    // Initialize all frames to -1 (empty)
    for (int i = 0; i < capacity; i++) frames[i] = -1;

    int page_faults = 0;
    int timer = 0; // A counter to simulate time

    printf("\nIncoming Page\tFrames\n");
    for (int i = 0; i < n; i++) {
        int current_page = pages[i];
        int is_hit = 0;
        int hit_index = -1;

        // Check for a "hit"
        for (int j = 0; j < capacity; j++) {
            if (frames[j] == current_page) {
                is_hit = 1;
                hit_index = j;
                break;
            }
        }

        if (is_hit) {
            // If hit, update the time for that page
            time[hit_index] = ++timer;
        } else {
            // If miss (page fault)
            page_faults++;
            int empty_frame = -1;
            // Check for an empty frame
            for (int j = 0; j < capacity; j++) {
                if (frames[j] == -1) {
                    empty_frame = j;
                    break;
                }
            }

            if (empty_frame != -1) {
                // If an empty frame is found, use it
                frames[empty_frame] = current_page;
                time[empty_frame] = ++timer;
            } else {
                // If no empty frames, find the least recently used page
                int lru_index = 0;
                int min_time = INT_MAX;
                for (int j = 0; j < capacity; j++) {
                    if (time[j] < min_time) {
                        min_time = time[j];
                        lru_index = j;
                    }
                }
                // Replace the LRU page
                frames[lru_index] = current_page;
                time[lru_index] = ++timer;
            }
        }
        
        // Print the state of the frames
        printf("%d\t\t", current_page);
        for (int j = 0; j < capacity; j++) {
            if (frames[j] != -1) printf("%d ", frames[j]);
            else printf("- ");
        }
        printf(is_hit ? "(Hit)\n" : "(Fault)\n");
    }

    printf("\nTotal Page Faults: %d\n", page_faults);
    printf("Total Page Hits: %d\n", n - page_faults);

    return 0;
}