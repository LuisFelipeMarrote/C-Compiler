#include "lexico.h"
#include "sintatico.h"
#include "simbolos.h"


int main(){
    FILE *fp; 
    char* filename = "new.txt";
    token main_tk;
    int linha_main = 1;
    fp = fopen(filename, "r");
    caractere = fgetc(fp); 
    //AnalisadorSintatico(fp,&linha_main,&main_tk);
    //AnalisadorLexical(fp,&linha_main,&tk);
    //print_token(&main_tk);
    //print_linhas();
    
    nova_tabela();

    
    insere_tab_simbolos("sla", 'x', snull, ' ');
    for(int i = 0; i < 5; i++){
        insere_tab_simbolos("entrada provisoria", 'x', i, ' ');
        printf("Nome: %s\t", tabela->nome_ident);
        printf("Tipo: %s\t", print_enum(tabela->tipo));
        printf("Escopo: %c\n", tabela->escopo);
    }
    insere_tab_simbolos("sla2", 'x', snull, ' ');
    insere_tab_simbolos("oi", 'x', snull, ' ');
    
    printf("===========================================================\n");
    for(int i = 0; i< 7; i++){
        printf("Nome: %s\t", tabela->nome_ident);
        printf("Tipo: %s\t", print_enum(tabela->tipo));
        printf("Escopo: %c\n", tabela->escopo);

        pop_tab_simbolos();
    }
}