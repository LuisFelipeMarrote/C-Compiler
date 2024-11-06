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
            sdif, smenorig, smenor, smaiorig, smaior, sponto, sig, serro, sbase, snull};  

char* print_enum(enum tipos simbolo){
    char* tipo[] = {"smais", "smenos", "smult", "snúmero", "satribuicao", "sdoispontos", "svírgula",
            "sabre_parenteses", "sfecha_parenteses", "sponto_virgula", "sprograma", "sse", "sentao",
            "ssenao", "senquanto", "sfaca", "sinício", "sfim", "sescreva", "sleia", "svar", "sinteiro", "sbooleano", 
            "sverdadeiro", "sfalso", "sprocedimento", "sfuncao", "sdiv", "se", "sou", "snao", "sidentificador",
            "sdif", "smenorig", "smenor", "smaiorig", "smaior", "sponto", "sig", "serro", "sbase", "snull"}; 
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


/*typedef struct {
  char c;
  node_pilha_char* prev;
} node_pilha_char;*/
