#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//pilha de dados
typedef struct {
    int* M; //Memória da pilha
    int s; //Topo da pilha
    int capacidade; // capacidade atual da pilha
} Pilha;

struct Inst {
    char rotulo[5];
    char instrucao[9];
    char atr1[5];
    char atr2[6];
};

Pilha *p = NULL;
int i = 0;
struct Inst lista[100];
int countres = -1;

//prototipos:
Pilha* inicializarPilha(int capacidadeInicial);
void liberarPilha();
int pilhaVazia();
int pilhaCheia();
void empilhar(int atributo);
int desempilhar();
int topo(); 
void redimensionarPilha(); 

Pilha* inicializarPilha(int capacidadeInicial) {
    Pilha *p = (Pilha*)malloc(sizeof(Pilha));
    if (p == NULL) {
        //printf("Erro: Falha na alocação da pilha\n");
        return NULL;
    }

    p->M = (int *)calloc(capacidadeInicial, sizeof(int));
    if (p->M == NULL) {
        free(p);
        //printf("Erro ao alocar memória para a pilha.\n");
        return NULL;
    }
    p->s = -1;
    p->capacidade = capacidadeInicial;
    return p;
}

void liberarPilha() {
    if(p == NULL){
        return;
    }
    if(p->M != NULL){
        free(p->M);
        p->M = NULL;
    }
    free(p);
    p = NULL;
}

int pilhaVazia() {
    if (p == NULL) {
        return 1;  
    }
    return p->s == -1;
}

int pilhaCheia() {
    if (p == NULL) {
        return 1;  
    }
    return p->s == p->capacidade - 1;
}

/// @brief s = s+1; M[s] = atributo 
void empilhar(int atributo) {
    if (p == NULL) {  // Falta verificação de ponteiro nulo
        return;
    }

    if (pilhaCheia()) {
        //printf("Pilha cheia! Redimensionando...\n");
        redimensionarPilha();
    }

    p->M[++(p->s)] = atributo;
    /*printf("Elemento %d empilhado.\n", atributo);
    printf("Estado atual da pilha: ");
    for (int i = 0; i <= p->s; i++) {
        printf("%d ", p->M[i]);
    }
    printf("\n");*/
    //AtualizarListViewMemoria();
}

/// @brief  retorna M[s] ; s = s-1
int desempilhar() {
    if (p == NULL) {  // Falta verificação de ponteiro nulo
        return 0;  // Melhor retornar erro que encerrar programa
    }

    if (pilhaVazia()) {
        //printf("Erro: Pilha vazia!\n");
        return 0;
    } else {
        return p->M[(p->s)--];
    }
}

/// @brief retorna M[s] 
int topo() {
    if (p == NULL) {  
        return -1;  
    }

    if (pilhaVazia()) {
        //printf("Erro: Pilha vazia!\n");
        return -1;
    } else {
        return p->M[p->s];
    }
}

void redimensionarPilha() {
    if (p == NULL) {
        return;
    }

    int novaCapacidade = (p->capacidade) + 10;
    int *novo_array = (int*)realloc(p->M, novaCapacidade * sizeof(int));
   
    if (novo_array == NULL) {
        //printf("Erro ao redimensionar a pilha.\n");
        return;
    }

    p->M = novo_array;
    p->capacidade = novaCapacidade;
}


