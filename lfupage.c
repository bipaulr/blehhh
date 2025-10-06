#include <stdio.h>
#include <limits.h>

int main() {
    int n; // Number of pages in reference string
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
    int frequency[capacity]; // Frequency of each page
    int time[capacity];      // Timestamp for FIFO tie-breaking

    // Initialize all frames
    for (int i = 0; i < capacity; i++) {
        frames[i] = -1;
        frequency[i] = 0;
        time[i] = 0;
    }

    int page_faults = 0;
    int timer = 0;

    printf("\nIncoming Page\tFrames (Page(Freq))\tStatus\n");
    printf("--------------------------------------------------\n");

    for (int i = 0; i < n; i++) {
        int current_page = pages[i];
        int is_hit = 0;
        int hit_index = -1;
        timer++; // Increment simulated time

        // Check if page is already present (Hit)
        for (int j = 0; j < capacity; j++) {
            if (frames[j] == current_page) {
                is_hit = 1;
                hit_index = j;
                break;
            }
        }

        if (is_hit) {
            frequency[hit_index]++; // Increase usage frequency
        } else {
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
                // Place new page in an empty frame
                frames[empty_frame] = current_page;
                frequency[empty_frame] = 1;
                time[empty_frame] = timer;
            } else {
                // Find LFU page
                int lfu_index = 0;
                int min_freq = INT_MAX;

                for (int j = 0; j < capacity; j++) {
                    if (frequency[j] < min_freq) {
                        min_freq = frequency[j];
                        lfu_index = j;
                    } 
                    // Tie-breaker: older page replaced first (FIFO)
                    else if (frequency[j] == min_freq && time[j] < time[lfu_index]) {
                        lfu_index = j;
                    }
                }

                // Replace the LFU page
                frames[lfu_index] = current_page;
                frequency[lfu_index] = 1; // Reset frequency for the new page
                time[lfu_index] = timer;  // Update load time
            }
        }

        // Print frame state
        printf("%d\t\t", current_page);
        for (int j = 0; j < capacity; j++) {
            if (frames[j] != -1)
                printf("%d(%d) ", frames[j], frequency[j]);
            else
                printf("- ");
        }
        printf("\t%s\n", is_hit ? "Hit" : "Fault");
    }

    printf("\nTotal Page Faults: %d\n", page_faults);
    printf("Total Page Hits: %d\n", n - page_faults);
    printf("Page Fault Rate: %.2f%%\n", (page_faults * 100.0) / n);

    return 0;
}
