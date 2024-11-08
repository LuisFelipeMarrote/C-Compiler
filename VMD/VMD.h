/*

LDC k (Carregar constante):

S:=s + 1 ; M [s]: = k


LDV n (Carregar valor):

S:=s+1 ; M[s]:=M[n]


ADD (Somar):

M[s-1]:=M[s-1]+M[s]; s:=s-1


SUB (Subtrair):

M[s-1]:=M[s-1]-M[s]; s:=s-1


MULT (Multiplicar):

M[s-1]:=M[s-1]*M[s]; s:=s-1


DIVI (Dividir):

M[s-1]:=M[s-1] div M[s]; s:=s-1


INV (Inverter sinal):

M[s]:=-M[s]


AND (Conjunção):

Se M [s-1]=1 e M[s]=1 então M[s-1]:=1 senão M[s-1]:=0; S:=s-1


OR (Disjunção):

Se M[s-1]=1 ou M[s]=1 então M[s-1]:=1 senão M[s-1]:=0; s:=s-1


NEG (Negação):

M[s]:=1-M[s]


CME (Comparar menor):

Se M[s-1]<M[s] então M[s-1]:=1 senão M[s-1]:=0; s:=s-1


CMA (Comparar maior):

Se M[s-1] >M[s] então M[s-1]:=1 senão M[s-1]:=0;s:=s-1


CEQ (comparar igual):

Se M[s-1]=M[s] então M[s-1]:=1 senão M[s-1]:=0;s:=s-1


CDIF (Comparar desigual):

Se M[s-1]  M[s] então M[s-1]:=1 senão M[s-1]:=0; s:=s-1


CMEQ (Comparar menor ou igual)

Se M[s-1]  M[s] então M[s-1]:=1 senão M[s-1]:=0;s:=s-1


CMAQ (Comparar maior ou igual):

Se M[s-1]  M[s] então M[s-1]:=1 senão M[s-1]:=0; s:=s-1

JMP p (Desviar sempre):
i:=p

JMPF p (Desviar se falso):
Se M[s]=0 então i:=p senão i:=i+1;
S:=s-1


STR n (Armazenar valor):
M[n]:=M[s]; s:=s-1

RD (Leitura):
S:=s+1; M[s]:= “próximo valor de entrada”

PRN (Impressão):
“Imprimir M[s]”; s:=s-1

START (Iniciar programa principal):
S:=-1

ALLOC m (Alocar memória*):
S:=s+m

DALLOC m (liberar memória):
S:=s-m

CALL p (Chamar procedimento ou função):
S:=s+1;
M[s]:=i+1;
i:=p

RETURN (Retornar de procedimento):
i:=M[s];
s:=s-1

*/


/*  Vai precisar de rótulo, instrução, atributo 1 e atributo 2

    Fazer um switch case, pegar os valores de cada instrução




*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_ROTULO_LEN 4
#define MAX_ATR_LEN 2
#define MAX_ROTULOS 16

int posicaoAtual = 0;
int i = 0;

struct Rotulo {
    char rot[MAX_ROTULO_LEN];
    int posicao;
};

struct Rotulo tabelaDeRotulos[MAX_ROTULOS];
int contadorDeRotulos = 0;

struct Pilha {
    int *M; //Memória da pilha
    int s; //Topo da pilha
    int capacidade;
};

void inicializarPilha(struct Pilha *p, int capacidadeInicial) {
    p->M = (int *)malloc(capacidadeInicial * sizeof(int));
    if (p->M == NULL) {
        printf("Erro ao alocar memória para a pilha.\n");
        exit(1);
    }
    p->s = -1;
    p->capacidade = capacidadeInicial;
}

int pilhaVazia(struct Pilha *p) {
    return p->s == -1;
}

int pilhaCheia(struct Pilha *p) {
    return p->s == p->capacidade - 1;
}

void redimensionarPilha(struct Pilha *p) {
    int novaCapacidade = p->capacidade * 2;
    p->M = (int *)realloc(p->M, novaCapacidade * sizeof(int));
    if (p->M == NULL) {
        printf("Erro ao redimensionar a pilha.\n");
        exit(1);
    }
    p->capacidade = novaCapacidade;
}

void empilhar(struct Pilha *p, int atributo) {
    if (pilhaCheia(p)) {
        printf("Pilha cheia! Redimensionando...\n");
        redimensionarPilha(p);
    }
    p->M[++(p->s)] = atributo;
    printf("Elemento %d empilhado.\n", atributo);
}

int desempilhar(struct Pilha *p) {
    if (pilhaVazia(p)) {
        printf("Erro: Pilha vazia!\n");
        return -1;
    } else {
        return p->M[(p->s)--];
    }
}

int topo(struct Pilha *p) {
    if (pilhaVazia(p)) {
        printf("Erro: Pilha vazia!\n");
        return -1;
    } else {
        return p->M[p->s];
    }
}

void liberarPilha(struct Pilha *p) {
    free(p->M);
    p->M = NULL;
    p->s = -1;
    p->capacidade = 0;
}

// Início da MVD
void MVD(struct Pilha *p, FILE *file) {
    char rot[MAX_ROTULO_LEN]; 
    char lerString[] = "";
    int posicao = 0;
    while(strcmp(lerString, "HLT") != 0){
        if (fscanf(file, "%s", lerString) != 1) {
            printf("Erro ao ler a instrução do arquivo.\n");
            return;
        }
        // Feito Ler rótulo
        resolveRot(rot, posicao);
        // como funciona ler rotulo, entradas possiveis: 
        // Feito Ler instrução
        resolveInst(lerString, p, file);
        // Fazer Ler atributo 1
        // Fazer Ler atributo 2
    }
}

void resolveRot(const char *rot, int posicao) {
    if (contadorDeRotulos < MAX_ROTULOS) {
        strcpy(tabelaDeRotulos[contadorDeRotulos].rot, rot);
        tabelaDeRotulos[contadorDeRotulos].posicao = posicao;
        contadorDeRotulos++;
    } else {
        printf("Erro: número máximo de rótulos atingido.\n");
    }
}

int encontrarPosicaoRotulo(const char *rot) {
    for (int i = 0; i < contadorDeRotulos; i++) {
        if (strcmp(tabelaDeRotulos[i].rot, rot) == 0) {
            return tabelaDeRotulos[i].posicao;
        }
    }
    printf("Erro: rótulo %s não encontrado.\n", rot);
    return -1;  // Retorna -1 se o rótulo não for encontrado
}

void resolveInst(char *inst, struct Pilha *p, FILE *file){
    char *atributo = inst;
    char list[MAX_ATR_LEN];
    for(int j = 0; j >= MAX_ATR_LEN; j++){
        if (fscanf(file, "%s", atributo) != 1){
        printf("Erro ao ler a instrução do arquivo.\n");
        }
        list[j] = *atributo;
    }
    switch (analisaInst(inst)) {
        case 0: // RETURN (Retornar de procedimento)
            i = desempilhar(p);
            break;
        case 1: // LDC k (Carregar constante)
            empilhar(p, p->M[p->s]);
            break;

        case 2: // LDV n (Carregar valor)
            empilhar(p, p->M[p->s]);
            break;

        case 3: // ADD (Somar)
            list[1] = desempilhar(p);
            list[0] = desempilhar(p);
            empilhar(p, list[0] + list[1]);
            break;

        case 4: // SUB (Subtrair)
            list[1] = desempilhar(p);
            list[0] = desempilhar(p);
            empilhar(p, list[0] - list[1]);
            break;

        case 5: // MULT (Multiplicar)
            list[1] = desempilhar(p);
            list[0] = desempilhar(p);
            empilhar(p, list[0] * list[1]);
            break;

        case 6: // DIVI (Dividir)
            list[1] = desempilhar(p);
            list[0] = desempilhar(p);
            if (list[1] != 0)
                empilhar(p, list[0] / list[1]);
            else
                printf("Erro: Divisão por zero!\n");
            break;

        case 7: // INV (Inverter sinal)
            p->M[p->s] = -p->M[p->s];
            break;

        case 8: // AND (Conjunção)
            list[1] = desempilhar(p);
            list[0] = desempilhar(p);
            empilhar(p, (list[0] == 1 && list[1] == 1) ? 1 : 0);
            break;

        case 9: // OR (Disjunção)
            list[1] = desempilhar(p);
            list[0] = desempilhar(p);
            empilhar(p, (list[0] == 1 || list[1] == 1) ? 1 : 0);
            break;

        case 10: // NEG (Negação)
            p->M[p->s] = 1 - p->M[p->s];
            break;

        case 11: // CME (Comparar menor)
            list[1] = desempilhar(p);
            list[0] = desempilhar(p);
            empilhar(p, (list[0] < list[1]) ? 1 : 0);
            break;

        case 12: // CMA (Comparar maior)
            list[1] = desempilhar(p);
            list[0] = desempilhar(p);
            empilhar(p, (list[0] > list[1]) ? 1 : 0);
            break;

        case 13: // CEQ (Comparar igual)
            list[1] = desempilhar(p);
            list[0] = desempilhar(p);
            empilhar(p, (list[0] == list[1]) ? 1 : 0);
            break;

        case 14: // CDIF (Comparar desigual)
            list[1] = desempilhar(p);
            list[0] = desempilhar(p);
            empilhar(p, (list[0] != list[1]) ? 1 : 0);
            break;

        case 15: // CMEQ (Comparar menor ou igual)
            list[1] = desempilhar(p);
            list[0] = desempilhar(p);
            empilhar(p, (list[0] <= list[1]) ? 1 : 0);
            break;

        case 16: // CMAQ (Comparar maior ou igual)
            list[1] = desempilhar(p);
            list[0] = desempilhar(p);
            empilhar(p, (list[0] >= list[1]) ? 1 : 0);
            break;
        
        case 17: // JMP p (Desviar sempre)
            int novaPosicao = encontrarPosicaoRotulo(atributo);  // usa atributo como rótulo
            if (novaPosicao != -1) {
                i = novaPosicao;
            }
            break;

        case 18: // JMPF p (Desviar se falso)
            if (desempilhar(p) == 0) {
                i = novaPosicao;
            } else {
                (i)++;
            }
            break;

        case 19: // STR n (Armazenar valor)
            p->M[atoi(atributo)] = p->M[p->s]; 
            p->s -= 1;
            break;

        case 20: // RD (Leitura)
            printf("Digite o próximo valor de entrada: ");
            int entrada;
            scanf("%d", &entrada);
            empilhar(p, entrada);
            break;

        case 21: // PRN (Impressão)
            printf("%d\n", desempilhar(p));
            break;

        case 22: // START (Iniciar programa principal)
            p->s = -1;
            break;

        case 23: // ALLOC m (Alocar memória)
            p->s += operando;
            break;

        case 24: // DALLOC m (Liberar memória)
            scanf("%d", &operando); // Lê o valor de m
            p->s -= operando;
            break;

        case 25: // CALL p (Chamar procedimento ou função)
            scanf("%d", &operando); // Lê o endereço p
            empilhar(p, *i + 1);
            *i = operando;
            break;
        
        case 27: // NULL (Nada)
            // Não faz nada
            break;

        default:
            printf("Instrução inválida: %s\n", inst);
            break;
    }
}

int analisaInst(char *inst) {
    if (strcmp(inst, "NULL") == 0)
        return 0;
    if (strcmp(inst, "LDC") == 0)
        return 1;
    else if (strcmp(inst, "LDV") == 0)
        return 2;
    else if (strcmp(inst, "ADD") == 0)
        return 3;
    else if (strcmp(inst, "SUB") == 0)
        return 4;
    else if (strcmp(inst, "MULT") == 0)
        return 5;
    else if (strcmp(inst, "DIVI") == 0)
        return 6;
    else if (strcmp(inst, "INV") == 0)
        return 7;
    else if (strcmp(inst, "AND") == 0)
        return 8;
    else if (strcmp(inst, "OR") == 0)
        return 9;
    else if (strcmp(inst, "NEG") == 0)
        return 10;
    else if (strcmp(inst, "CME") == 0)
        return 11;
    else if (strcmp(inst, "CMA") == 0)
        return 12;
    else if (strcmp(inst, "CEQ") == 0)
        return 13;
    else if (strcmp(inst, "CDIF") == 0)
        return 14;
    else if (strcmp(inst, "CMEQ") == 0)
        return 15;
    else if (strcmp(inst, "CMAQ") == 0)
        return 16;
    else if (strcmp(inst, "JMP") == 0)
        return 17;
    else if (strcmp(inst, "JMPF") == 0)
        return 18;
    else if (strcmp(inst, "STR") == 0)
        return 19;
    else if (strcmp(inst, "RD") == 0)
        return 20;
    else if (strcmp(inst, "PRN") == 0)
        return 21;
    else if (strcmp(inst, "START") == 0)
        return 22;
    else if (strcmp(inst, "ALLOC") == 0)
        return 23;
    else if (strcmp(inst, "DALLOC") == 0)
        return 24;
    else if (strcmp(inst, "CALL") == 0)
        return 25;
    else if (strcmp(inst, "RETURN") == 0)
        return 26;
}

int main() {
    struct Pilha p;
    inicializarPilha(&p, 8);
    FILE *file = fopen("teste.txt", "r");
    MVD(&p, file);
    fclose(file);
    liberarPilha(&p);
    /* Exemplo de uso
    empilhar(&p, 10);
    empilhar(&p, 20);
    desempilhar(&p);
    liberarPilha(&p);
*/
    return 0;
}
