#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void convert(char hex_mask[], char bit_mask[]);

int main(void) {
    char add_str[10], length_str[10], input[10], bitmask_hex[13];
    char pn[10];
    char bitmask_bin[49];

    int start_addr, text_addr, text_len, i, opcode, addr, actual_addr;
    FILE *fp1, *fp2;

    printf("Enter the actual starting address (in hex): ");
    if (scanf("%x", &start_addr) != 1) {
        printf("Wrong starting address       \n");
        return 1;
    }

    fp1 = fopen("RLIN.txt", "r");
    if (fp1 == NULL) {
        printf("Error opening RLIN.txt\n");
        return 1;
    }

    fp2 = fopen("RLOUT.txt", "w");
    if (fp2 == NULL) {
        printf("Error opening RLOUT.txt\n");
        fclose(fp1);
        return 1;
    }

    fprintf(fp2, " ADDRESS   CONTENT\n");
    fprintf(fp2, "------------------\n");

    while (fscanf(fp1, "%9s", input) == 1 && strcmp(input, "E") != 0) {
        
        if (strcmp(input, "H") == 0) {
            if (fscanf(fp1, "%9s %9s %9s", pn, add_str, length_str) != 3) {
                printf("Error H\n");
                break;
            }
        } 
        else if (strcmp(input, "T") == 0) {
            if (fscanf(fp1, "%x %x %12s", &text_addr, &text_len, bitmask_hex) != 3) {
                 printf("Error T\n");
                 break;
            }

            text_addr += start_addr;

            convert(bitmask_hex, bitmask_bin);
            int num_instructions = text_len / 3;

            for (i = 0; i < num_instructions; i++) {
                if (fscanf(fp1, "%x %x", &opcode, &addr) != 2) {
                    printf("Error instruct in T\n");
                    break;
                }

                if (bitmask_bin[i] == '1') {
                    actual_addr = addr + start_addr;
                } else {
                    actual_addr = addr;
                }

                fprintf(fp2, "  %04X\t  %02X%04X\n", text_addr, opcode, actual_addr);
                text_addr += 3;
            }
            
            if (i != num_instructions) {
                break;
            }
        } 
        else {
            printf("Error: Unrecognized record type '%s'.\n", input);
            break;
        }
    }

    fclose(fp2);
    fclose(fp1);

    printf("\nRelocating loader finished. Output is in RLOUT.txt\n");

    return 0;
}

void convert(char hex_mask[], char bit_mask[]) {
    int i;
    bit_mask[0] = '\0'; 

    for (i = 0; i < strlen(hex_mask); i++) {
        switch (hex_mask[i]) {
            case '0': strcat(bit_mask, "0000"); break;
            case '1': strcat(bit_mask, "0001"); break;
            case '2': strcat(bit_mask, "0010"); break;
            case '3': strcat(bit_mask, "0011"); break;
            case '4': strcat(bit_mask, "0100"); break;
            case '5': strcat(bit_mask, "0101"); break;
            case '6': strcat(bit_mask, "0110"); break;
            case '7': strcat(bit_mask, "0111"); break;
            case '8': strcat(bit_mask, "1000"); break;
            case '9': strcat(bit_mask, "1001"); break;
            case 'A': strcat(bit_mask, "1010"); break;
            case 'B': strcat(bit_mask, "1011"); break;
            case 'C': strcat(bit_mask, "1100"); break;
            case 'D': strcat(bit_mask, "1101"); break;
            case 'E': strcat(bit_mask, "1110"); break;
            case 'F': strcat(bit_mask, "1111"); break;
            default: 
                printf("Warning: Invalid hex character in mask: %c\n", hex_mask[i]);
                break;
        }
    }
}