#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    char mnemonic[10];
    char code[10];
} OptabEntry;

typedef struct {
    char symbol[10];
    char address[10];
} SymtabEntry;

int main() {
    char addr[10], label[10], opcode[10], operand[10], object_code[20];
    int start_addr = 0, length = 0;

    OptabEntry OPTAB[50];
    SymtabEntry SYMTAB[50];
    int optab_count = 0, symtab_count = 0;

    FILE *inter = fopen("intermediate.txt", "r");
    FILE *optab_file = fopen("optab.txt", "r");
    FILE *symtab_file = fopen("symtab.txt", "r");
    FILE *asml = fopen("asml.txt", "w");
    FILE *objc = fopen("objc.txt", "w");
    FILE *leng = fopen("length.txt", "r");

    if (!inter || !optab_file || !symtab_file || !asml || !objc || !leng) {
        printf("Error: Could not open one or more files.\n");
        return 1;
    }

    // --- Load OPTAB and SYMTAB into memory ---
    while (fscanf(optab_file, "%s %s", OPTAB[optab_count].mnemonic, OPTAB[optab_count].code) == 2)
        optab_count++;
    while (fscanf(symtab_file, "%s %s", SYMTAB[symtab_count].symbol, SYMTAB[symtab_count].address) == 2)
        symtab_count++;
    fscanf(leng, "%X", &length);

    // --- Process START Directive ---
    if (fscanf(inter, "%s %s %s %s", addr, label, opcode, operand) != 4) {
        printf("Error: Invalid intermediate file.\n");
        return 1;
    }

    if (strcmp(opcode, "START") == 0) {
        start_addr = (int)strtol(operand, NULL, 16);
        fprintf(asml, "~\t%s\t%s\t%s\t~\n", label, opcode, operand);
        fprintf(objc, "H^%-6s^%06X^%06X\n", label, start_addr, length);
        fscanf(inter, "%s %s %s %s", addr, label, opcode, operand);
    }

    // --- Start Text record ---
    fprintf(objc, "T^%06X^", (int)strtol(addr, NULL, 16));

    // --- Main Loop ---
    while (strcmp(opcode, "END") != 0) {
        int op_found = 0;

        // Search OPTAB
        for (int i = 0; i < optab_count; i++) {
            if (strcmp(opcode, OPTAB[i].mnemonic) == 0) {
                int sym_found = 0;
                for (int j = 0; j < symtab_count; j++) {
                    if (strcmp(operand, SYMTAB[j].symbol) == 0) {
                        sprintf(object_code, "%s%s", OPTAB[i].code, SYMTAB[j].address);
                        sym_found = 1;
                        break;
                    }
                }
                if (!sym_found)
                    sprintf(object_code, "%s0000", OPTAB[i].code);

                fprintf(objc, "%s^", object_code);
                fprintf(asml, "%s\t%s\t%s\t%s\t%s\n", addr, label, opcode, operand, object_code);
                op_found = 1;
                break;
            }
        }

        // Handle directives
        if (!op_found) {
            if (strcmp(opcode, "WORD") == 0) {
                int val = atoi(operand);
                fprintf(objc, "%06X^", val);
                fprintf(asml, "%s\t%s\t%s\t%s\t%06X\n", addr, label, opcode, operand, val);
            } else if (strcmp(opcode, "BYTE") == 0) {
                fprintf(asml, "%s\t%s\t%s\t%s\t", addr, label, opcode, operand);
                if (operand[0] == 'C') {
                    for (int i = 2; i < strlen(operand) - 1; i++) {
                        fprintf(objc, "%02X", operand[i]);
                        fprintf(asml, "%02X", operand[i]);
                    }
                    fprintf(objc, "^");
                    fprintf(asml, "\n");
                } else if (operand[0] == 'X') {
                    for (int i = 2; i < strlen(operand) - 1; i++) {
                        fprintf(objc, "%c", operand[i]);
                        fprintf(asml, "%c", operand[i]);
                    }
                    fprintf(objc, "^");
                    fprintf(asml, "\n");
                }
            } else {
                // RESW or RESB
                fprintf(asml, "%s\t%s\t%s\t%s\t~\n", addr, label, opcode, operand);
            }
        }

        if (fscanf(inter, "%s %s %s %s", addr, label, opcode, operand) != 4)
            break;
    }

    // --- END Directive ---
    fprintf(objc, "\nE^%06X\n", start_addr);
    fprintf(asml, "~\t%s\t%s\t%s\t~\n", label, opcode, operand);

    fclose(inter);
    fclose(optab_file);
    fclose(symtab_file);
    fclose(asml);
    fclose(objc);
    fclose(leng);

    printf("Pass 2 completed successfully.\n");
    return 0;
}
