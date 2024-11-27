#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_ROT_LEN 4
#define MAX_ATR_LEN 4
#define MAX_STR_LEN 8
#define MAX_NUM_ATR 2
#define MAX_ROTULOS 16
#define MAX_INST 100

int i = 0;

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


Pilha* inicializarPilha(int capacidadeInicial);
int pilhaVazia(Pilha *p); 
int pilhaCheia(Pilha *p);
void empilhar(Pilha *p, int atributo);
int desempilhar(Pilha *p);
int topo(Pilha *p);
void redimensionarPilha(Pilha *p);
void liberarPilha(Pilha *p);
int analisaInst(struct Inst *lista);


Pilha* inicializarPilha(int capacidadeInicial) {
    Pilha *p = (Pilha*)malloc(sizeof(Pilha));
    if (p == NULL) {
        printf("Erro: Falha na alocação da pilha\n");
        return NULL;
    }
    
    p->M = (int*)malloc(capacidadeInicial * sizeof(int));
    if (p->M == NULL) {
        free(p);
        printf("Erro ao alocar memória para a pilha.\n");
        exit(1);
    }
    p->s = -1;
    p->capacidade = capacidadeInicial;
    return p;
}

int pilhaVazia(Pilha *p) {
    return p->s == -1;
}

int pilhaCheia(Pilha *p) {
    return p->s == p->s - 1;
}

/// @brief s = s+1; M[s] = atributo 
void empilhar(Pilha *p, int atributo) {
    if (pilhaCheia(p)) {
        printf("Pilha cheia! Redimensionando...\n");
        redimensionarPilha(p);
    }

    p->M[++(p->s)] = atributo;
    printf("Elemento %d empilhado.\n", atributo);
    printf("Estado atual da pilha: ");
    for (int i = 0; i <= p->s; i++) {
        printf("%d ", p->M[i]);
    }
    printf("\n");
}
 

/// @brief  retorna M[s] ; s = s-1
int desempilhar(Pilha *p) {
    if (pilhaVazia(p)) {
        printf("Erro: Pilha vazia!\n");
        exit(1);
    } else {
        return p->M[(p->s)--];
    }
}

/// @brief retorna M[s] 
int topo(Pilha *p) {
    if (pilhaVazia(p)) {
        printf("Erro: Pilha vazia!\n");
        return -1;
    } else {
        return p->M[p->s];
    }
}

void redimensionarPilha(Pilha *p) {
    int novaCapacidade = (p->capacidade) + 1;
    int *novo_array = (int*)realloc(p->M, novaCapacidade * sizeof(int));
   
    if (novo_array == NULL) {
        printf("Erro ao redimensionar a pilha.\n");
        exit(1);
    }

    p->M = novo_array;
    p->capacidade = novaCapacidade;
}

void liberarPilha(Pilha *p) {
    if(p == NULL){
        return;
    }
    free(p->M);
    free(p);
}

int analisaInst(struct Inst *lista) {
    if (strcmp(lista->instrucao, "NULL") == 0)
        return 0;
    if (strcmp(lista->instrucao, "LDC     ") == 0)
        return 1;
    else if (strcmp(lista->instrucao, "LDV     ") == 0)
        return 2;
    else if (strcmp(lista->instrucao, "ADD     ") == 0)
        return 3;
    else if (strcmp(lista->instrucao, "SUB     ") == 0)
        return 4;
    else if (strcmp(lista->instrucao, "MULT    ") == 0)
        return 5;
    else if (strcmp(lista->instrucao, "DIVI    ") == 0)
        return 6;
    else if (strcmp(lista->instrucao, "INV     ") == 0)
        return 7;
    else if (strcmp(lista->instrucao, "AND     ") == 0)
        return 8;
    else if (strcmp(lista->instrucao, "OR      ") == 0)
        return 9;
    else if (strcmp(lista->instrucao, "NEG     ") == 0)
        return 10;
    else if (strcmp(lista->instrucao, "CME     ") == 0)
        return 11;
    else if (strcmp(lista->instrucao, "CMA     ") == 0)
        return 12;
    else if (strcmp(lista->instrucao, "CEQ     ") == 0)
        return 13;
    else if (strcmp(lista->instrucao, "CDIF    ") == 0)
        return 14;
    else if (strcmp(lista->instrucao, "CMEQ    ") == 0)
        return 15;
    else if (strcmp(lista->instrucao, "CMAQ    ") == 0)
        return 16;
    else if (strcmp(lista->instrucao, "JMP     ") == 0)
        return 17;
    else if (strcmp(lista->instrucao, "JMPF    ") == 0)
        return 18;
    else if (strcmp(lista->instrucao, "STR     ") == 0)
        return 19;
    else if (strcmp(lista->instrucao, "RD      ") == 0)
        return 20;
    else if (strcmp(lista->instrucao, "PRN     ") == 0)
        return 21;
    else if (strcmp(lista->instrucao, "START   ") == 0)
        return 22;
    else if (strcmp(lista->instrucao, "ALLOC   ") == 0)
        return 23;
    else if (strcmp(lista->instrucao, "DALLOC  ") == 0)
        return 24;
    else if (strcmp(lista->instrucao, "CALL    ") == 0)
        return 25;
    else if (strcmp(lista->instrucao, "RETURN  ") == 0)
        return 26;
}

void lerInstrucoes(FILE *file, struct Inst lista[MAX_INST]) {
    int count = -1;
    while (strcmp(lista[count].instrucao, "HLT     ") != 0){
        (count)++;

        // Analisa e preenche os campos da estrutura
        fgets(lista[count].rotulo, sizeof(lista[count].rotulo), file);
        fgets(lista[count].instrucao, sizeof(lista[count].instrucao), file);
        fgets(lista[count].atr1, sizeof(lista[count].atr1), file);
        fgets(lista[count].atr2, sizeof(lista[count].atr2), file);
 
        // Imprime as informações lidas (opcional)
        printf("%4s %8s %4s %4s\n",
               lista[count].rotulo,
               lista[count].instrucao,
               lista[count].atr1,
               lista[count].atr2);
    }
}

void resolveInst(Pilha *p, FILE *file, int* count, struct Inst lista[MAX_INST]){
    printf("%8s", lista[*count].instrucao);
    int m = 0;
    int n = 0;
    int resultado = 0;
    switch (analisaInst(&lista[*count])) {
        case 0: // NULL
            // Nada
            break;
        case 1: // LDC k (Carregar constante)
            empilhar(p, atoi(lista[*count].atr1));
            break;

        case 2: // LDV n (Carregar variavel)
            empilhar(p, atoi(lista[*count].atr1));
            break;

        case 3: // ADD (Somar)
            m = desempilhar(p); //m[s]
            n = desempilhar(p); //m[s-1]
            resultado = n + m; // m[s-1] + m[s]
            p->M[(p->s)--] = resultado; //m[s-1] = res; s = s-1
            break;

        case 4: // SUB (Subtrair)
            m = desempilhar(p); //m[s]
            n = desempilhar(p); //m[s-1]
            resultado = n - m; // m[s-1] - m[s]
            p->M[(p->s)--] = resultado; //m[s-1] = res; s = s-1
            break;

        case 5: // MULT (Multiplicar)
            m = desempilhar(p); //m[s]
            n = desempilhar(p); //m[s-1]
            resultado = n * m; // m[s-1] * m[s]
            p->M[(p->s)--] = resultado; //m[s-1] = res; s = s-1
            break;

        case 6: // DIVI (Dividir)
            m = desempilhar(p); //m[s]
            n = desempilhar(p); //m[s-1]
            if (m != 0){
                resultado = n / m; // m[s-1] div m[s]
                p->M[(p->s)--] = resultado; //m[s-1] = res; s = s-1
            }
            else
                printf("Erro: Divisão por zero!\n");
            break;

        case 7: // INV (Inverter sinal)
            p->M[p->s] = -(p->M[p->s]);
            break;

        case 8: // AND (Conjunção)
            m = desempilhar(p); //m[s]
            n = desempilhar(p); //m[s-1]
            resultado = n && m; // m[s-1] and m[s]
            p->M[(p->s)--] = resultado; //m[s-1] = res; s = s-1
            break;

        case 9: // OR (Disjunção)
            m = desempilhar(p); //m[s]
            n = desempilhar(p); //m[s-1]
            resultado = n || m; // m[s-1] OU m[s]
            p->M[(p->s)--] = resultado; //m[s-1] = res; s = s-1
            break;
            
        case 10: // NEG (Negação)
            p->M[p->s] = 1 - p->M[p->s];
            break;

        case 11: // CME (Comparar menor)
            m = desempilhar(p); //m[s]
            n = desempilhar(p); //m[s-1]
            resultado = n < m; // m[s-1] < m[s]
            p->M[(p->s)--] = resultado; //m[s-1] = res; s = s-1
            break;

        case 12: // CMA (Comparar maior)
            m = desempilhar(p); //m[s]
            n = desempilhar(p); //m[s-1]
            resultado = n > m; // m[s-1] > m[s]
            p->M[(p->s)--] = resultado; //m[s-1] = res; s = s-1
            break;

        case 13: // CEQ (Comparar igual)
            m = desempilhar(p); //m[s]
            n = desempilhar(p); //m[s-1]
            resultado = n == m; // m[s-1] = m[s]
            p->M[(p->s)--] = resultado; //m[s-1] = res; s = s-1
            break;

        case 14: // CDIF (Comparar desigual)
            m = desempilhar(p); //m[s]
            n = desempilhar(p); //m[s-1]
            resultado = n != m; // m[s-1] != m[s]
            p->M[(p->s)--] = resultado; //m[s-1] = res; s = s-1
            break;

        case 15: // CMEQ (Comparar menor ou igual)
            m = desempilhar(p); //m[s]
            n = desempilhar(p); //m[s-1]
            resultado = n <= m; // m[s-1] <= m[s]
            p->M[(p->s)--] = resultado; //m[s-1] = res; s = s-1
            break;

        case 16: // CMAQ (Comparar maior ou igual)
            m = desempilhar(p); //m[s]
            n = desempilhar(p); //m[s-1]
            resultado = n >= m; // m[s-1] >= m[s]
            p->M[(p->s)--] = resultado; //m[s-1] = res; s = s-1
            break;
        
        case 17: // JMP p (Desviar sempre)
            for (int i = -1; i < MAX_INST; i++) {
                // Verifique se o rótulo da linha atual é igual a lista[count].atr1
                if (strcmp(lista[i].rotulo, lista[*count].atr1) == 0) {
                    printf("\n rotulo = %s, instrucao = %s, atr1 = %s",lista[i].rotulo, lista[*count].instrucao, lista[*count].atr1);
                    // Se for igual, você pode executar a ação desejada
                    *count = i;
                    return;
                    // Faça o que for necessário quando o rótulo for encontrado
                }
            };
            break;

        case 18: // JMPF p (Desviar se falso)
            if(desempilhar(p) == 0){
                for (int i = -1; i < MAX_INST; i++) {
                    // Verifique se o rótulo da linha atual é igual a lista[count].atr1
                    if (strcmp(lista[i].rotulo, lista[*count].atr1) == 0) {
                        // Se for igual, você pode executar a ação desejada
                        *count = i;
                        // Faça o que for necessário quando o rótulo for encontrado
                    }
                }
            };
            break;

        case 19: // STR n (Armazenar valor)
            p->M[atoi(lista[*count].atr1)] = p->M[p->s];
            (p->s)--;
            break;
 
        case 20: // RD (Leitura)
            printf("Digite o próximo valor de entrada: ");
            int entrada;
            fflush(stdin);
            scanf("%d", &entrada);
            printf("\n Entrada = %d", entrada);
            empilhar(p, entrada);
            break;

        case 21: // PRN (Impressão)
            printf("%d\n", desempilhar(p));
            break;

        case 23: // ALLOC (Alocar memória)
            n = atoi(lista[*count].atr2);
            for (int k = 0; k < n; k++){
                m = atoi(lista[*count].atr1);
                empilhar(p, p->M[m+k]);
            }
            break;

        case 24: // DALLOC m (Liberar memória)
            n = atoi(lista[*count].atr2);
            for (int k = n - 1; k >= 0; k--) {
                int m = atoi(lista[*count].atr1);
                p->M[m+k] = p->M[p->s];
                p->s = p->s - 1;                 // Decrementa o topo da pilha
            }
            break;

        case 25: // CALL p (Chamar procedimento ou função)
            empilhar(p, ((*count)+1));
            for (int i = 0; i < MAX_INST; i++) {
                
                // Verifique se o rótulo da linha atual é igual a lista[count].atr1
                if (strcmp(lista[i].rotulo, lista[*count].atr1) == 0) {
                    // Se for igual, você pode executar a ação desejada
                    *count = i;
                    return;
                    // Faça o que for necessário quando o rótulo for encontrado
                }
            }
            break;

        case 26: // Return
            *count = desempilhar(p);

        default:
            printf("Instrução inválida: \n");
            break;
    }
}

// Início da MVD
void MVD(Pilha *p, struct Inst lista[], int countres, FILE *file) {
    if (strcmp(lista[0].instrucao, "START   ") != 0) {
        printf("Erro: A primeira instrução deve ser 'START'.\n");
        return;
    }
    while (strcmp(lista[countres].instrucao, "HLT     ") != 0) {
            countres++;
            resolveInst(p, file, &countres, lista);
    }
    printf("\nHLT - FIM DO PROGRAMA");
    
    liberarPilha(p);
}
