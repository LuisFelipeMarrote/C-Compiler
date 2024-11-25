#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "definicoes.h"
#include "simbolos.h"

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
int pesquisa_declvar_tabela(token tk);
int pesquisa_declvarfunc_tabela(char* indent);
int qtde_variaveis_escopo();

//ponteiro para o topo da tabela
entrada_tab_simbolos* tabela = NULL; 

void popula_entrada(entrada_tab_simbolos* entrada, char string[], char escopo, enum tipos tipo, char rotulo[]){
    entrada->escopo = escopo;
    entrada->tipo = tipo;
    strcpy(entrada->nome_ident, string);
    strcpy(entrada->rotulo, rotulo);
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
entrada_tab_simbolos* busca_ident(char* identificador){
    entrada_tab_simbolos* entrada_atual = tabela;
    while(entrada_atual->tipo != sbase){
        char* ident_tabela = entrada_atual->nome_ident;

        if(strcmp(ident_tabela, identificador) == 0){
            return(entrada_atual);
        }
        entrada_atual = entrada_atual->prev;
    }
    return NULL;
}

///após declaração de variáveis ou funções, coloca os tipos na tabela baseado no parâmetro:
///- se 'ident' = "_var", troca o tipo de todas as entradas svar seguidas no tipo passado
///- se não, procura o ident na tabela e atribui fbool ou fint (passado no parametro)
void coloca_tipo_tabela(char* ident, enum tipos tipo){
    if(!strcmp("_var", ident)){
        entrada_tab_simbolos* temp = tabela;
        while(temp->tipo == svar){
            temp->tipo = tipo;
            temp = temp->prev;
        }
    }else{
        entrada_tab_simbolos* func = busca_ident(ident);
        if(func!= NULL){
            if(func->tipo == sfuncao){
                func->tipo = tipo;
            }else{     
                //semantic_error(0); // possivelmente nunca chegue aqui, ms sla
                printf("erro que ainda nao implementei. COMO QUE O CÓDIGO CHEGOU AQUI????");
            }
        }else{
            //semantic_error(0); // possivelmente nunca chegue aqui, ms sla
            printf("função nao encontrada ao inserir tipo. COMO QUE O CÓDIGO CHEGOU AQUI????");
        }
    }
}

/// 
void nova_tabela(){
    entrada_tab_simbolos* base = (entrada_tab_simbolos*) malloc(sizeof(entrada_tab_simbolos));
    popula_entrada(base, "base_da_pilha", 'L', sbase, "-");
    tabela = base;
    tabela->prev = NULL;    
}

void deleta_tabela(){
    while(tabela != NULL) {
        entrada_tab_simbolos* temp = tabela;
        tabela = tabela->prev;
        free(temp);
    }
}

void volta_nivel(){ ///possivelmente adicionar a quantidade de variaveis desalocadas
    while(tabela != NULL && tabela->escopo != 'L'){
        entrada_tab_simbolos* temp = tabela;
        tabela = tabela->prev;
        free(temp);
    }
    tabela->escopo = '-'; ///confirmar isso
}


///- Pesquisa se a variavel ja foi declarada no escopo 
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

/// @brief Pesquisa se a variavel ja foi declarada na parte visível da tabela (não limitado por escopo) - SÓ LÊ INTEIRO (confirmar se precisa dessa parte)
/// @param tk 
/// @return 1 se achou
int pesquisa_declvar_tabela(token tk){ // pesquisa se a variavel ja foi declarada
    //achou = 1;
    entrada_tab_simbolos* entrada_atual = tabela;
    while(entrada_atual->tipo != sbase){
        char* ident_tabela = entrada_atual->nome_ident;
        if(!strcmp(ident_tabela, tk.lexema)){
            if(entrada_atual->tipo != sinteiro){
                semantic_error(16, *linha); //achou, mas tipo errado
            }
            return 1; // encontrou 
        }
        entrada_atual = entrada_atual->prev;
    };
    return 0;  // nao encontrou
}

/// @brief durante a declaração verifica se a função/procedimento já foi declarada antes (tabela inteira)
/// @param indent nome da função
/// @return encontrou = false (0), não encontrou = true (1)
int pesquisa_declfunc_tabela(char* indent){ ///precisa ser inteiro ou booleano
    entrada_tab_simbolos* entrada_atual = tabela;
    while(entrada_atual->tipo != sbase){
        char* ident_tabela = entrada_atual->nome_ident;
        if(!strcmp(ident_tabela, indent)){
            if(entrada_atual->tipo != fint && entrada_atual->tipo !=fbool)
            return 0; // encontrou = false (0)
        }
        entrada_atual = entrada_atual->prev;
    };
    return 1; // não encontrou = verdadeiro (1)
}

// rever essa funçao acho que o return ta invertido, onde era para ser 1 é 0.
/// @brief Em analisa_escreva, procura por uma variável ou função(retorno) para escrever (tipo inteiro)
/// @param indent nome
/// @return achou(0) ou nao(1)
int pesquisa_declvarfunc_tabela(char* indent){ 
    entrada_tab_simbolos* entrada_atual = tabela;
    while(entrada_atual->tipo != sbase){
        char* ident_tabela = entrada_atual->nome_ident;
        if(!strcmp(ident_tabela, indent)){
            return 0; // encontrou = false (0)
        }
        entrada_atual = entrada_atual->prev;
    };
    return 1; // não encontrou = verdadeiro (1)
}

/// @brief conta quantas variáveis tem em um escopo para auxiliar na geração de instruções dalloc
/// @return quantidade de variáveis até a primeira marca de escopo
int qtde_variaveis_escopo(){
    entrada_tab_simbolos* entrada_atual = tabela;
    int count = 0;
    while(entrada_atual->escopo != 'L'){
        count++;
    }
    return count;
}

/// @brief função que verifica qual função ou procedimento está sendo analisada atualmente
/// @return a primeira entrada da tabela de símbolos com a marca de escopo
entrada_tab_simbolos* func_proc_atual(){
    entrada_tab_simbolos* atual = tabela;
    while (atual->tipo !=sbase){
        if(atual->escopo == 'L'){
            return atual;
        }
        atual = atual->prev;
    }
    return NULL;
}
