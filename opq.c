#include <stdio.h>
#include <string.h>

char stack[50], input[50];
int top = -1, ip = 0;

char table[8][8] = {
    {'>','>','<','<','<','>','<','>'},
    {'>','>','<','<','<','>','<','>'},
    {'>','>','>','>','<','>','<','>'},
    {'>','>','>','>','<','>','<','>'},
    {'<','<','<','<','<','=','<',' '},
    {'>','>','>','>',' ','>',' ','>'},
    {'>','>','>','>',' ','>',' ','>'},
    {'<','<','<','<','<',' ','<',' '}
};

int findex(char c)
{   
    switch(c)
    {
        case '+': return 0;
        case '-': return 1;
        case '*': return 2;
        case '/': return 3;
        case '(': return 4;
        case ')': return 5;
        case 'i': return 6;
        case '$': return 7;
    }
    return -1;
}

void display(char action[])
{
    int i;

    for(i = 0; i <= top; i++)
        printf("%c", stack[i]);

    printf("\t\t%s\t\t%s\n", input + ip, action);
}

int reduce(char action[])
{
    /* E -> i */
    if(top >= 0 && stack[top] == 'i')
    {
        stack[top] = 'E';
        strcpy(action, "REDUCE E->id");
        return 1;
    }

    /* E -> (E) */
    if(top >= 2 &&
       stack[top] == ')' &&
       stack[top-1] == 'E' &&
       stack[top-2] == '(')
    {
        top -= 2;
        stack[top] = 'E';

        strcpy(action, "REDUCE E->(E)");
        return 1;
    }

    /* E -> E+E | E-E | E*E | E/E */
    if(top >= 2 &&
       stack[top] == 'E' &&
       stack[top-2] == 'E')
    {
        if(stack[top-1] == '+' ||
           stack[top-1] == '-' ||
           stack[top-1] == '*' ||
           stack[top-1] == '/')
        {
            char op = stack[top-1];

            top -= 2;
            stack[top] = 'E';

            sprintf(action, "REDUCE E->E%cE", op);
            return 1;
        }
    }

    return 0;
}

int main()
{
    char a, b;
    char action[30];
    int i;

    printf("Enter expression (use i for id): ");
    scanf("%48s", input);

    strcat(input, "$");

    stack[++top] = '$';

    printf("\n%-16s %-16s %s\n", "STACK", "INPUT", "ACTION");
    printf("------------------------------------------------------------\n");

    while(1)
    {
        i = top;

        /* Ignore E while finding the top terminal */
        while(i >= 0 && stack[i] == 'E')
            i--;

        a = stack[i];
        b = input[ip];

        /* Acceptance */
        if(top == 1 &&
           stack[0] == '$' &&
           stack[1] == 'E' &&
           b == '$')
        {
            display("ACCEPTED");
            break;
        }

        /* Invalid symbol */
        if(findex(a) == -1 || findex(b) == -1)
        {
            display("REJECTED");
            break;
        }

        /* SHIFT */
        if(table[findex(a)][findex(b)] == '<' ||
           table[findex(a)][findex(b)] == '=')
        {
            stack[++top] = b;
            ip++;

            display("SHIFT");
        }

        /* REDUCE */
        else if(table[findex(a)][findex(b)] == '>')
        {
            if(!reduce(action))
            {
                display("REJECTED");
                break;
            }

            display(action);
        }

        /* Error */
        else
        {
            display("REJECTED");
            break;
        }
    }

    return 0;
}
