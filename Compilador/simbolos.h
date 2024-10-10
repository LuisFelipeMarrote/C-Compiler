#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "definicoes.h"
#include "lexico.h"
#include "semantico.h"

typedef struct entrada_tab_simbolos{
    char nome_ident[lexema_size_max];
    char escopo; //rever tipo
    enum tipos tipo;
    //adicionar end memoria (ver qual tipo usar)
    entrada_tab_simbolos* prev;
}entrada_tab_simbolos;

//"Classe" da tabela de simbolos começa----------------------------------------------
typedef struct tabela_de_simbolos{
    entrada_tab_simbolos* topo;

    //Insere e remove na Tabela:
    void (*insere)(struct tabela_de_simbolos* self, entrada_tab_simbolos* novo); 
    void (*pop)(struct tabela_de_simbolos* self); 
    //Consulta a Tabela: percorre a Tabela procurando por um identificador. Devolve todos os campos do registro.
    //Coloca Tipo nas Variáveis: percorre a tabela do final para o começo substituindo todos os campos tipo que possuem o valor variável pelo tipo agora localizado.

}tabela_de_simbolos;

void insere_tab_simbolos(struct tabela_de_simbolos* self, entrada_tab_simbolos* novo){
    novo->prev = self->topo;
    self->topo = novo;
}

void pop_tab_simbolos(struct tabela_de_simbolos* self){
    entrada_tab_simbolos* topo_anterior;
    topo_anterior = self->topo;
    self->topo = self->topo->prev;
    free(topo_anterior);
}

tabela_de_simbolos* nova_tabela(){
    tabela_de_simbolos* tabela = (tabela_de_simbolos*)malloc(sizeof(tabela_de_simbolos));
    entrada_tab_simbolos base;
    
    char* nome_base = "base_da_pilha";
    base.escopo = '-';
    memcpy(base.nome_ident, nome_base, strlen(nome_base) + 1);
    base.tipo = sbase;
    tabela->topo = &base;

    tabela->insere = insere_tab_simbolos;
    tabela->pop = pop_tab_simbolos;
}

void deleta_tabela(tabela_de_simbolos* tabela){
    free(tabela);
}

//"Classe" da tabela de simbolos acaba----------------------------------------------
