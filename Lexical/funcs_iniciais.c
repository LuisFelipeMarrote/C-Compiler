#include "funcs_iniciais_lib.h"
#include "sintaxico.h"


int main(){
    FILE *fp; 
    char* filename = "ExtraS.txt";
    token main_tk;
    int linha_main = 1;
    fp = fopen(filename, "r");
    caractere = fgetc(fp); 
    AnalisadorSintatico(fp,&linha_main,&main_tk);
    //AnalisadorLexical(fp,&linha_main,&tk);
    //print_token(&main_tk);
    print_linhas();
}