#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "definicoes.h"

void popula_entrada(entrada_tab_simbolos* entrada, char string[], char escopo, enum tipos tipo, char rotulo[]);
void insere_tab_simbolos(char nome_ident[], enum tipos tipo, char escopo, char rotulo[]);
void pop_tab_simbolos();
entrada_tab_simbolos* busca_ident(char identificador[]);
int Pesquisa_duplicvar_tabela(char* indent);
void coloca_tipo_tabela(char* ident, enum tipos tipo);
void nova_tabela();
void deleta_tabela();
int pesquisa_declfunc_tabela(char* indent);
void volta_nivel();

//ponteiro para o topo da tabela
entrada_tab_simbolos* tabela = NULL; 

void popula_entrada(entrada_tab_simbolos* entrada, char string[], char escopo, enum tipos tipo, char rotulo[]){
    entrada->escopo = escopo;
    entrada->tipo = tipo;
    strcpy(entrada->nome_ident, string);
}

void insere_tab_simbolos(char nome_ident[], enum tipos tipo, char escopo, char rotulo[]){
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

//busca na tabela de simbolos por um identificador e retorna a entrada
entrada_tab_simbolos* busca_ident(char identificador[]){
    entrada_tab_simbolos* entrada_atual = tabela;
    while(entrada_atual->tipo != sbase){
        char* ident_tabela = entrada_atual->nome_ident;

        if(!strcmp(ident_tabela, identificador)){
            return(entrada_atual);
        }
        entrada_atual = entrada_atual->prev;
    }
    return NULL;
}

int Pesquisa_duplicvar_tabela(char* indent){
    entrada_tab_simbolos* entrada_atual = tabela;
    int fim_de_escopo = 1;
    while(entrada_atual->tipo != sbase && fim_de_escopo){
        char* ident_tabela = entrada_atual->nome_ident;
        if(!strcmp(ident_tabela, indent)){
            return 0; // encontrou = false (0)
        }
        if(entrada_atual->escopo == 'L')
            fim_de_escopo = 0;
        entrada_atual = entrada_atual->prev;
    };
    return 1; // não encontrou = true
}

///implementar
void coloca_tipo_tabela(char* ident, enum tipos tipo){
    entrada_tab_simbolos* temp = tabela;
    while(temp->tipo == svar || temp->tipo == sprocedimento || temp->tipo == sfuncao){
        temp->tipo = tipo;
        temp = temp->prev;
    }
}

void nova_tabela(){
    entrada_tab_simbolos* base = (entrada_tab_simbolos*) malloc(sizeof(entrada_tab_simbolos));
    popula_entrada(base, "base_da_pilha", 'L', sbase, "-");
    tabela = base;    
}

void deleta_tabela(){
    while(tabela != NULL) {
        entrada_tab_simbolos* temp = tabela;
        tabela = tabela->prev;
        free(temp);
    }
}

int pesquisa_declfunc_tabela(char* indent){
    entrada_tab_simbolos* entrada_atual = tabela;
    while(entrada_atual->tipo != sbase){
        char* ident_tabela = entrada_atual->nome_ident;
        if(!strcmp(ident_tabela, indent)){
            return 1; // encontrou = true (1)
        }
        entrada_atual = entrada_atual->prev;
    };
    return 0; // não encontrou = false
}


void volta_nivel(){
    while(tabela != NULL && tabela->escopo != 'L'){
        entrada_tab_simbolos* temp = tabela;
        tabela = tabela->prev;
        free(temp);
    }
}