%{
    #include<stdio.h>
    #include<stdlib.h>
    int yylex();
    void yyerror(const char *s){
        void(s);
        prinft("Invalid Variable /n");
    }
%}

%token L D

%%
variable : L rest '\n'  {printf("Valid Variable \n"); YYACCEPT;}
rest : rest L
     | rest D 
     | 
     ;
%%

int main(){
    return yyparse(); 
}

