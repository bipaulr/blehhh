#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BLOCKS 100
#define MAX_FILES 10

struct File {
    char filename[50];
    int start_block;
    int num_blocks;
};

void allocateContiguous(struct File* file, char* filename, int startBlock, int numBlocks) {
    strcpy(file->filename, filename);
    file->start_block = startBlock;
    file->num_blocks = numBlocks;
}

int main() {
    struct File files[MAX_FILES];
    int num_files;

    printf("Enter number of files to allocate (max %d): ", MAX_FILES);
    scanf("%d", &num_files);
    if (num_files > MAX_FILES) {
        printf("Maximum number of files exceeded.\n");
        return 1;
    }

    for (int i = 0; i < num_files; i++) {
        char filename[50];
        int startBlock, numBlocks;

        printf("\nFile %d:\n", i + 1);
        printf("Enter filename: ");
        scanf("%s", filename);

        printf("Enter start block (0-%d): ", MAX_BLOCKS - 1);
        scanf("%d", &startBlock);

        printf("Enter number of blocks: ");
        scanf("%d", &numBlocks);

        if (startBlock < 0 || startBlock >= MAX_BLOCKS || numBlocks <= 0 || (startBlock + numBlocks) > MAX_BLOCKS) {
            printf("Invalid block allocation for file %s. Try again.\n", filename);
            i--; 
            continue;
        }

        allocateContiguous(&files[i], filename, startBlock, numBlocks);
    }

printf("\nFile Allocation Table:\n");
printf("%-15s %-15s %-15s\n", "File Name", "Start Block", "Number of Blocks");

for (int i = 0; i < num_files; i++) {
    printf("%-15s %-15d %-15d\n",files[i].filename,files[i].start_block,files[i].num_blocks);
}

    return 0;
}