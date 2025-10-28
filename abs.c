#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void main()
{
FILE *fp;
int i, j, staddr1;
char progName[10], line[100], fileName[10], staddr[10];
printf("Enter program name: ");
scanf("%s", progName);
fp = fopen("objectcode.txt", "r");
if (fp == NULL) {
printf("Error: Cannot open object code file.\n");
exit(1);
}
fscanf(fp, "%s", line);
if (line[0] != 'H') {
printf("Invalid object program format.\n");
fclose(fp);
return;
}
for (i = 2, j = 0; i < 8 && j < 6; i++, j++)
fileName[j] = line[i];
fileName[j] = '\0';
printf("Program name from object file: %s\n", fileName);
if (strcmp(progName, fileName) == 0) {
while (fscanf(fp, "%s", line) != EOF) {
if (line[0] == 'T') {
for (i = 2, j = 0; i < 8 && j < 6; i++, j++)
staddr[j] = line[i];
staddr[j] = '\0';
staddr1 = (int)strtol(staddr, NULL, 16);
i = 12;
while (line[i] != '$' && line[i] != '\0') {
if (line[i] != '^') {
printf("%04X\t%c%c\n", staddr1, line[i],
line[i + 1]);
staddr1++;
i += 2;
} else {
i++;
}
}
} else if (line[0] == 'E') {
printf("\nEnd of program.\n");
break;
}
}
} else {
printf("Program name does not match.\n");
}
fclose(fp);
}
