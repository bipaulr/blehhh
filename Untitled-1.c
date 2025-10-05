#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
    char opcode[15], operand[30], label[15];
    char optab_opcode[15], optab_value[15];
    int locctr, start, length;
    FILE *inp, *symtab, *optab, *inter, *leng;

    inp = fopen("input.txt", "r");
    symtab = fopen("symtab.txt", "w");
    optab = fopen("optab.txt", "r");
    inter = fopen("intermediate.txt", "w");
    leng = fopen("length.txt", "w");

    if (!inp || !symtab || !optab || !inter || !leng) {
        printf("Error: Could not open one or more files.\n");
        return 1;
    }

    fscanf(inp, "%s %s %s", label, opcode, operand);

    if (strcmp(opcode, "START") == 0) {
        locctr = (int)strtol(operand, NULL, 16);
        start = locctr;
        // Simplified output with tabs
        fprintf(inter, "%X\t%s\t%s\t%s\n", locctr, label, opcode, operand);
        fscanf(inp, "%s %s %s", label, opcode, operand);
    } else {
        locctr = 0;
        start = 0;
    }

    while (strcmp(opcode, "END") != 0) {
        if (strcmp(label, "~") != 0) {
            // Simplified output with tabs
            fprintf(symtab, "%s\t%X\n", label, locctr);
        }
        // Simplified output with tabs
        fprintf(inter, "%X\t%s\t%s\t%s\n", locctr, label, opcode, operand);

        rewind(optab);
        int found = 0;
        while (fscanf(optab, "%s%s ", optab_opcode, optab_value) == 2) {
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
            }
        }
        fscanf(inp, "%s %s %s", label, opcode, operand);
    }

    fprintf(inter, "%X\t%s\t%s\t%s\n", locctr, label, opcode, operand);

    length = locctr - start;
    printf("\nProgram size: %x\n", length);
    fprintf(leng, "%x", length);

    fclose(leng);
    fclose(inp);
    fclose(optab);
    fclose(symtab);
    fclose(inter);

    return 0;
}