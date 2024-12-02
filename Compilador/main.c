#include "lexico.h"
#include "sintatico.h"
//#include "simbolos.h"
//#include "semantico.h"

int main(){
    atexit(limpa_memoria);

    FILE *fp; 
    char* filename = "../testes/Geracao/semantico-12.txt";
    token main_tk;
    int linha_main = 1;
    fp = fopen(filename, "r");
    caractere = fgetc(fp); 
    AnalisadorSintatico(fp,&linha_main,&main_tk);
    return 0;
}

