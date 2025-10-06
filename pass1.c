#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
    char label[15], opcode[15], operand[30];
    char optab_opcode[15], optab_value[15];
    int locctr = 0, start = 0, length = 0;
    FILE *inp, *symtab, *optab, *inter, *lengthFile;

    inp = fopen("input.txt", "r");
    symtab = fopen("symtab.txt", "w");
    optab = fopen("optab.txt", "r");
    inter = fopen("intermediate.txt", "w");
    lengthFile = fopen("length.txt", "w");

    if (!inp || !symtab || !optab || !inter || !lengthFile) {
        printf("Error: Could not open one or more files.\n");
        return 1;
    }

    // Read first line
    if (fscanf(inp, "%s %s %s", label, opcode, operand) != 3) {
        printf("Error: Invalid input format in first line.\n");
        return 1;
    }

    // START
    if (strcmp(opcode, "START") == 0) {
        locctr = (int)strtol(operand, NULL, 16);
        start = locctr;
        fprintf(inter, "%X\t%s\t%s\t%s\n", locctr, label, opcode, operand);
        if (fscanf(inp, "%s %s %s", label, opcode, operand) != 3) {
            printf("Error: Unexpected end of file after START.\n");
            return 1;
        }
    }

    // Process till END
    while (strcmp(opcode, "END") != 0) {
        if (strcmp(label, "~") != 0) {
            fprintf(symtab, "%s\t%X\n", label, locctr);
        }

        fprintf(inter, "%X\t%s\t%s\t%s\n", locctr, label, opcode, operand);

        rewind(optab);
        int found = 0;

        while (fscanf(optab, "%s %s", optab_opcode, optab_value) == 2) {
            if (strcmp(opcode, optab_opcode) == 0) {
                locctr += 3;
                found = 1;
                break;
            }
        }

        if (!found) {
            if (strcmp(opcode, "WORD") == 0) {
                locctr += 3;
            } else if (strcmp(opcode, "RESW") == 0) {
                locctr += 3 * atoi(operand);
            } else if (strcmp(opcode, "RESB") == 0) {
                locctr += atoi(operand);
            } else if (strcmp(opcode, "BYTE") == 0) {
                if (operand[0] == 'C') {
                    locctr += strlen(operand) - 3;
                } else if (operand[0] == 'X') {
                    locctr += (strlen(operand) - 3) / 2;
                }
            } else {
                printf("Warning: Unknown opcode %s at LOCCTR %X\n", opcode, locctr);
            }
        }

        if (fscanf(inp, "%s %s %s", label, opcode, operand) != 3)
            break;
    }

    fprintf(inter, "%X\t%s\t%s\t%s\n", locctr, label, opcode, operand);

    length = locctr - start;
    printf("\nProgram size: %X\n", length);
    fprintf(lengthFile, "%X", length);

    fclose(lengthFile);
    fclose(inp);
    fclose(optab);
    fclose(symtab);
    fclose(inter);

    return 0;
}
