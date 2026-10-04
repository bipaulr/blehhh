%{
#include <stdio.h>
#include <stdlib.h>
/* One node of the abstract syntax tree. */
typedef struct node {
    const char *label;           /* an operator like "+", or the text of a name or number */
    struct node *left, *right;   /* operands; both NULL for a leaf */
} Node;
Node *make_node(const char *label, Node *left, Node *right) {
    Node *p = malloc(sizeof(Node));
    if (p == NULL) {
        fputs("Out of memory\n", stderr);
        exit(EXIT_FAILURE);
    }
    p->label = label;
    p->left = left;
    p->right = right;
    return p;
}
/* Release leaves' copied text and every node after printing or on a parse error. */
void free_tree(Node *p) {
    if (p == NULL) return;
    free_tree(p->left);
    free_tree(p->right);
    if (p->left == NULL && p->right == NULL) free((void *)p->label);
    free(p);
}
/* Print the tree sideways: root at the left, right operand above, left operand below.
   Each level is indented by 4 spaces. */
void print_tree(Node *p, int depth) {
    if (p == NULL) {
        return;
    }
    print_tree(p->right, depth + 1);
    printf("%*s%s\n", depth * 4, "", p->label);
    print_tree(p->left, depth + 1);
}
int yylex(void);
void yyerror(const char *msg) {
    (void)msg;
    puts("syntax error");
}
%}
/* values passed around: text (from lex) or a tree node (built by the rules) */
%union {
    char *text;
    struct node *node;
}
%token <text> ID NUM
%type <node> expr term factor
%destructor { free($$); } <text>
%destructor { free_tree($$); } <node>
%%
/* The input is a list of lines, each holding one expression. */
lines : lines expr '\n'     { print_tree($2, 0); puts("-----"); free_tree($2); }
      | lines error '\n'    { yyerrok; }
      | /* empty */
      ;
/* BNF rule:  <expr> ::= <expr> + <term> | <expr> - <term> | <term>
   An operator rule builds a node with its two operands as children. */
expr   : expr '+' term      { $$ = make_node("+", $1, $3); }
       | expr '-' term      { $$ = make_node("-", $1, $3); }
       | term
       ;
/* BNF rule:  <term> ::= <term> * <factor> | <term> / <factor> | <factor> */
term   : term '*' factor    { $$ = make_node("*", $1, $3); }
       | term '/' factor    { $$ = make_node("/", $1, $3); }
       | factor
       ;
/* BNF rule:  <factor> ::= ( <expr> ) | ID | NUM
   A name or number becomes a leaf; brackets add no node of their own. */
factor : '(' expr ')'       { $$ = $2; }
       | ID                 { $$ = make_node($1, NULL, NULL); }
       | NUM                { $$ = make_node($1, NULL, NULL); }
       ;
%%
int main(void) {
    return yyparse();
}
