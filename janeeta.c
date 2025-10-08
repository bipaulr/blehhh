#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
    char addr[10], label[10], opcode[10], operand[10];
    char opt_opcode[10], opt_code[10];
    char sym_label[10], sym_addr[10];
    int start_addr = 0, length = 0;
    FILE *inter = fopen("intermediate.txt", "r");
    FILE *optab = fopen("optab.txt", "r");
    FILE *symtab = fopen("symtab.txt", "r");
    FILE *objc = fopen("objc.txt", "w");
    FILE *leng = fopen("length.txt", "r");

    if (!inter || !optab || !symtab || !objc || !leng) {
        printf("Error: Could not open one or more files.\n");
        return 1;
    }
    fscanf(leng, "%X", &length);
    // Read first line
    fscanf(inter, "%s %s %s %s", addr, label, opcode, operand);
    // START directive
    if (strcmp(opcode, "START") == 0) {
        start_addr = (int)strtol(operand, NULL, 16);
        fprintf(objc, "H^%-6s^%06X^%06X\n", label, start_addr, length);
        fscanf(inter, "%s %s %s %s", addr, label, opcode, operand);
    }
    fprintf(objc, "T^%06X^", (int)strtol(addr, NULL, 16));
    // Main loop
    while (strcmp(opcode, "END") != 0) {
        int found = 0;
        rewind(optab);
        // Search in OPTAB
        while (fscanf(optab, "%s %s", opt_opcode, opt_code) == 2) {
            if (strcmp(opcode, opt_opcode) == 0) {
                found = 1;
                int sym_found = 0;
                rewind(symtab);
                // Search operand in SYMTAB
                while (fscanf(symtab, "%s %s", sym_label, sym_addr) == 2) {
                    if (strcmp(operand, sym_label) == 0) {
                        fprintf(objc, "%s%s^", opt_code, sym_addr);
                        sym_found = 1;
                        break;
                    }
                }
                if (!sym_found) {
                    fprintf(objc, "%s0000^", opt_code);
                }
                break;
            }
        }
        // Not in OPTAB → assembler directive
        if (!found) {
            if (strcmp(opcode, "WORD") == 0) {
                int val = atoi(operand);
                fprintf(objc, "%06X^", val);
            } 
            else if (strcmp(opcode, "BYTE") == 0) {
                if (operand[0] == 'C') {
                    for (int i = 2; i < strlen(operand) - 1; i++)
                        fprintf(objc, "%02X", operand[i]);
                    fprintf(objc, "^");
                } 
                else if (operand[0] == 'X') {
                    for (int i = 2; i < strlen(operand) - 1; i++)
                        fprintf(objc, "%c", operand[i]);
                    fprintf(objc, "^");
                }
            }
            // RESW, RESB → no object code
        }
        fscanf(inter, "%s %s %s %s", addr, label, opcode, operand);
    }
    fprintf(objc, "\nE^%06X\n", start_addr);
    fclose(inter);
    fclose(optab);
    fclose(symtab);
    fclose(objc);
    fclose(leng);
    printf("Pass 2 completed successfully.\n");
    return 0;
}