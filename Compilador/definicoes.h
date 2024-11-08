#pragma once
#include <stdlib.h> 
#include <stdio.h> 
#include <string.h> 
#include <ctype.h> 
#define lexema_size_max 30

enum tipos {smais, smenos, smult, snúmero, satribuicao, sdoispontos,svírgula, 
            sabre_parenteses, sfecha_parenteses, sponto_virgula, sprograma, sse, sentao,
            ssenao, senquanto, sfaca, sinício, sfim, sescreva, sleia, svar, sinteiro, sbooleano, 
            sverdadeiro, sfalso, sprocedimento, sfuncao, sdiv, se, sou, snao, sidentificador, 
            sdif, smenorig, smenor, smaiorig, smaior, sponto, sig, serro, sbase, snull, fint, 
            fbool};  

char* print_enum(enum tipos simbolo){
    char* tipo[] = {"smais", "smenos", "smult", "snúmero", "satribuicao", "sdoispontos", "svírgula",
            "sabre_parenteses", "sfecha_parenteses", "sponto_virgula", "sprograma", "sse", "sentao",
            "ssenao", "senquanto", "sfaca", "sinício", "sfim", "sescreva", "sleia", "svar", "sinteiro", "sbooleano", 
            "sverdadeiro", "sfalso", "sprocedimento", "sfuncao", "sdiv", "se", "sou", "snao", "sidentificador",
            "sdif", "smenorig", "smenor", "smaiorig", "smaior", "sponto", "sig", "serro", "sbase", "snull", "fint", 
            "fbool"}; 
    return tipo[simbolo];       
}

typedef struct{ 
  char lexema[lexema_size_max]; 
  enum tipos simbolo; 
}token;

typedef struct node_lista_token node_lista_token;
typedef struct node_lista_token{
    token tk;
    node_lista_token* prox;
}node_lista_token;

//cria novo node de lista de tokens
node_lista_token* novo_no_token(token tk) {
    node_lista_token* novo_no = (node_lista_token*)malloc(sizeof(node_lista_token));
    if (novo_no) {
        novo_no->tk = tk;
        novo_no->prox = NULL;
    }
    return novo_no;
}

//adiciona um token a uma lista
node_lista_token* adicionar_token(node_lista_token* lista, token tk) {
    node_lista_token* novo_no = novo_no_token(tk);
    if (!lista) {
        return novo_no;
    }
    node_lista_token* atual = lista;
    while (atual->prox != NULL) {
        atual = atual->prox;
    }
    atual->prox = novo_no;
    return lista;
}

typedef struct entrada_tab_simbolos entrada_tab_simbolos;
struct entrada_tab_simbolos{
    char nome_ident[lexema_size_max];
    char escopo;
    enum tipos tipo;
    char rotulo[4]; //end de memoria
    entrada_tab_simbolos* prev;
};