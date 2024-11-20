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

void sintax_error(int n, int linha){
    ///rever todos os rotulos de erro abaixo (placeholders)
    char* erros[] = {"falta definir",
        "O código não começa com 'programa'",
        "O código deve ter um nome para o programa após 'programa'",
        "Não foi encontrado ';' após declaração de nome do programa", 
        "Não foi encontrado '.' ao final do código",
        "O código deve terminar no '.' (não pode ter mais código após o ponto)",
        "Não foi encontrado ';' após declaração de variáveis",
        "O código deveria ter um nome de variável entre ',' e ':' (remova ',' caso não queira adicionar mais variaveis)",
        "Declaração de variável com tipo inválido (deve ser booleano ou inteiro)",
        "Não foi encontrado 'Inicio' antes dos comandos",
        "Não foi encontrado '(' após 'leia'",
        "Esperado identificador para leitura",
        "Parentesis aberto não é fechado em 'leia('",
        "Não foi encontrado '(' após 'escreva'",
        "Parentesis aberto não é fechado em 'escreva('",
        "Esperado identificador para escrita",
        "16: Esperado identificador de processo",
        "17: Esperado '(' após processo",
        "18: Esperado ')' após processo",
        "Não foi encontrado ';' após um comando",
        "Não foi encontrado 'faca' após a condição do comando 'enquanto'",
        "Esperado identificador para declaração de procedimento (seu procedimento precisa ter nome)",
        "Não foi encontrado ';' após a declaração de procedimento",
        "Esperado identificador para declaração de função (sua função precisa ter nome)",
        "Não foi encontrado ':' para declaração de tipo da função (formato esperado: 'nome_da_funcao : tipo_da_funcao;')",
        "Tipo inválido para a função (deve ser 'inteiro' ou 'booleano')",
        "Não foi encontrado ';' após a declaração de função",
        "Esperado 'entao' após a condição do comando 'se'",
        "Parêntesis não foi fechado corretamente (era esperado ')')",
        "Foi Encontrado um simbolo inesperado dentro da expressao",
        "Era Esperado ';' após o 'fim' de uma subrotina (procedimento ou função)",
        };
    
    printf("Erro na linha %d: %s", linha, erros[n]);
    printf(" %s ", erros[n]);
    printf("(Código de erro - Sint%d) \n", n);

}

void semantic_error(int n, int linha){
    //rever todos os rotulos de erro abaixo (placeholders)
    char* erros[] = {"falta definir",
        "1: ",
        };
    
    printf("Erro semantico");
    printf(" %s ", erros[n]);
    printf("na linha %d\n", linha);

}