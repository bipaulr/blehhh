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
    int frequency[capacity]; // To store frequency counts
    int time[capacity];      // To track load time for FIFO tie-breaking
    // Initialize all frames to -1 (empty)
    for (int i = 0; i < capacity; i++) {
        frames[i] = -1;
        frequency[i] = 0;
        time[i] = 0;
    }

    int page_faults = 0;
    int timer = 0; // A counter to simulate time

    printf("\nIncoming Page\tFrames\n");
    for (int i = 0; i < n; i++) {
        int current_page = pages[i];
        int is_hit = 0;
        int hit_index = -1;
        timer++;

        // Check for a "hit"
        for (int j = 0; j < capacity; j++) {
            if (frames[j] == current_page) {
                is_hit = 1;
                hit_index = j;
                break;
            }
        }

        if (is_hit) {
            // If hit, just increment the frequency
            frequency[hit_index]++;
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
                frequency[empty_frame] = 1;
                time[empty_frame] = timer;
            } else {
                // If no empty frames, find the least frequently used page
                int lfu_index = 0;
                int min_freq = INT_MAX;

                for (int j = 0; j < capacity; j++) {
                    if (frequency[j] < min_freq) {
                        min_freq = frequency[j];
                        lfu_index = j;
                    } 
                    // Tie-breaker: If frequencies are equal, choose the oldest one (FIFO)
                    else if (frequency[j] == min_freq && time[j] < time[lfu_index]) {
                        lfu_index = j;
                    }
                }
                // Replace the LFU page
                frames[lfu_index] = current_page;
                frequency[lfu_index] = 1;
                time[lfu_index] = timer;
            }
        }

        // Print the state of the frames
        printf("%d\t\t", current_page);
        for (int j = 0; j < capacity; j++) {
            if (frames[j] != -1) printf("%d(%d) ", frames[j], frequency[j]);
            else printf("- ");
        }
        printf(is_hit ? "(Hit)\n" : "(Fault)\n");
    }

    printf("\nTotal Page Faults: %d\n", page_faults);
    printf("Total Page Hits: %d\n", n - page_faults);

    return 0;
}