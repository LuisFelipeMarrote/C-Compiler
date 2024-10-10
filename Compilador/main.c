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
    AnalisadorSintatico(fp,&linha_main,&main_tk);
    //AnalisadorLexical(fp,&linha_main,&tk);
    //print_token(&main_tk);
    print_linhas();
    /*tabela_de_simbolos* tabela = nova_tabela();
    for(int i = 0; i < 5; i++){
        entrada_tab_simbolos* nova_entrada = (entrada_tab_simbolos*) malloc(sizeof(entrada_tab_simbolos));
        popula_entrada(nova_entrada, "entrada provisoria", 'x', i);
        tabela->insere(tabela, nova_entrada);
        printf("Nome: %s\t", tabela->topo->nome_ident);
        printf("Tipo: %s\t", print_enum(tabela->topo->tipo));
        printf("Escopo: %c\n", tabela->topo->escopo);
    }
    printf("===========================================================\n");
    for(int i = 0; i< 6; i++){
        printf("Nome: %s\t", tabela->topo->nome_ident);
        printf("Tipo: %s\t", print_enum(tabela->topo->tipo));
        printf("Escopo: %c\n", tabela->topo->escopo);

        tabela->pop(tabela);
    }*/
}