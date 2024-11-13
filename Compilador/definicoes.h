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
            fbool, snomeprog, ssinalu};  

char* print_enum(enum tipos simbolo){
    char* tipo[] = {"smais", "smenos", "smult", "snúmero", "satribuicao", "sdoispontos", "svírgula",
            "sabre_parenteses", "sfecha_parenteses", "sponto_virgula", "sprograma", "sse", "sentao",
            "ssenao", "senquanto", "sfaca", "sinício", "sfim", "sescreva", "sleia", "svar", "sinteiro", "sbooleano", 
            "sverdadeiro", "sfalso", "sprocedimento", "sfuncao", "sdiv", "se", "sou", "snao", "sidentificador",
            "sdif", "smenorig", "smenor", "smaiorig", "smaior", "sponto", "sig", "serro", "sbase", "snull", "fint", 
            "fbool", "snomeprog", "ssinalu"}; 
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

void sintax_error(int n){
    ///rever todos os rotulos de erro abaixo (placeholders)
    char* erros[] = {"falta definir",
        "1: Esperado 'programa'",
        "2: Esperado identificador para escrever",
        "3: Esperado ';'", 
        "4: Esperado ponto final",
        "5: Encontrado ponto final antes do fim do arquivo",
        "6: Esperado ';' após declaração de variáveis",
        "7: Esperado um identificador para 'var'",
        "8: Declaração de variável com tipo inválido",
        "9: Esperado inicio",
        "10: Esperado '(' após 'leia'",
        "11: Esperado identificador para leitura",
        "12: Esperado ')' para 'leia'",
        "13: Esperado '(' após 'escreva'",
        "14: Esperado ')' para 'escreva'",
        "15: Esperado identificador para escrita",
        "16: Esperado identificador de processo",
        "17: Esperado '(' após processo",
        "18: Esperado ')' após processo",
        "19: Esperado ';' após comando",
        "20: Esperado 'faca' após enquanto",
        "21: Esperado identificador para declaração de procedimento",
        "22: Esperado ';' após a declaração de procedimento",
        "23: Esperado identificador para declaração de função",
        "24: Esperado ':' para declaração de função",
        "25: Tipo inválido para a função",
        "26: Esperado ';' após a declaração de função",
        "27: Esperado 'entao' após 'se'",
        "28: Esperado ')'",
        "29: Encontrado simbolo inesperado para um fator",
        "30: Esperado ';' após subrotina",
        };
    
    printf("Erro na linha %d: %s", *linha);
    printf(" %s ", erros[n]);
    printf("(Código de erro - Sint%d) \n", n);

}

void semantic_error(int n){
    //rever todos os rotulos de erro abaixo (placeholders)
    char* erros[] = {"falta definir",
        "1: ",
        };
    
    printf("Erro semantico");
    printf(" %s ", erros[n]);
    printf("na linha %d\n", *linha);

}