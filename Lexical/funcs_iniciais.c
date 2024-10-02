#include "funcs_iniciais_lib.h"

FILE *fp; 
char* filename = "new.txt";
token tk;
int linha_main = 0;

int main(){
    fp = fopen(filename, "r");
    caractere = fgetc(fp); 
    while ( feof(fp) != 1)
    { 
        AnalisadorLexical(fp,&linha_main,&tk);
        print_token(&tk);
    }
    print_linhas();
}