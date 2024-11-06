#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "definicoes.h"
#include "lexico.h"
#include "semantico.h"

typedef struct entrada_tab_simbolos entrada_tab_simbolos;
typedef struct entrada_tab_simbolos{
    char nome_ident[lexema_size_max];
    char escopo; //rever tipo
    enum tipos tipo;
    //adicionar end memoria (ver qual tipo usar)
    entrada_tab_simbolos* prev;
}entrada_tab_simbolos;

void popula_entrada(entrada_tab_simbolos* entrada, char string[], char escopo, enum tipos tipo, char rotulo){
    entrada->escopo = escopo;
    entrada->tipo = tipo;
    memcpy(entrada->nome_ident, string, strlen(string) + 1);
}



//ponteiro para o topo da tabela
entrada_tab_simbolos* tabela; 

void insere_tab_simbolos(char nome_ident[], char escopo, enum tipos tipo, char rotulo){
    entrada_tab_simbolos* novo = (entrada_tab_simbolos*) malloc(sizeof(entrada_tab_simbolos));
    popula_entrada(novo, nome_ident, escopo, tipo, rotulo);

    novo->prev = tabela;
    tabela = novo;
}

void pop_tab_simbolos(){
    if(tabela->tipo != sbase){
        entrada_tab_simbolos* topo_anterior = tabela;
        tabela = topo_anterior->prev;
        free(topo_anterior);
    }else{
        printf("não há mais entradas!\n");
    }
}

///implementar
int Pesquisa_duplicvar_tabela(char* indent){
    printf("Nao implementei ainda(Pesquisa_duplicvar_tabela)");
    //encontrou = false
    return 1; // não encontrou = true
}

///implementar
void coloca_tipo_tabela(char* ident){
    printf("Nao implementei ainda(coloca_tipo_tabela)");
    
}

void nova_tabela(){
    //cria a tabela e primeira entrada (base)
    entrada_tab_simbolos* base = (entrada_tab_simbolos*) malloc(sizeof(entrada_tab_simbolos));
    
    //popula a primeira entrada
    char* nome_base = "base_da_pilha";
    base->escopo = '-';
    memcpy(base->nome_ident, nome_base, strlen(nome_base) + 1);
    base->tipo = sbase;
    tabela = base;    
}

void deleta_tabela(){
    free(tabela);
}

