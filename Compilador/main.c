#include "lexico.h"
#include "sintatico.h"
//#include "simbolos.h"
//#include "semantico.h"

int main(){
    atexit(limpa_memoria);

    FILE *fp; 
    //char* filename = "../testes/Sintatico/sint1.txt";
    char* filename = "../testes/Geracao/gera_Final.txt";
    //char* filename = "../testes/pessoal.txt";
    token main_tk;
    int linha_main = 1;
    fp = fopen(filename, "r");
    caractere = fgetc(fp); 
    AnalisadorSintatico(fp,&linha_main,&main_tk);
    //AnalisadorLexical(fp,&linha_main,&tk);
    //print_token(&main_tk);
    //print_linhas();

    return 0;
}

