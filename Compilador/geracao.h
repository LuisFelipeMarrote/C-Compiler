#pragma once
#include <stdlib.h> 
#include <stdio.h> 
#include <string.h> 
#include <ctype.h> 
#include "definicoes.h"

FILE *new_fp; 
char nome_arquivo[30];

void Gera(char rotulo[4], char instrucao[8], char atr1[4], char atr2[4]);
void Gera_operador(char op[]);
void Cria_arquivo();

// essa função vou implementar dps, mas ela é so para formatar
// void Seta_string(char str[], int size){
//     char ajuda[size];
//     memset(ajuda,' ', size);
//     str[0] = 'a';
// }

void Gera(char rotulo[4], char instrucao[8], char atr1[4], char atr2[4]){
    //redeclara isso, pois ele ta passando como string literal, ou seja, so daa para ler
    //entao eu so preciso declarar variaveis aux e copiar a informaçao paraa laa
    char linha[20] = {}; //ver como inicializa a lista vazia 
    //Seta_string(rotulo, 4);
    strcat(linha, rotulo);
    //Seta_string(instrucao, sizeof(instrucao));
    strcat(linha, instrucao);
    if (strcmp(atr1, "    ") != 0)
    {
        //Seta_string(atr1, sizeof(atr1));
        strcat(linha, atr1);
    }
    else if (strcmp(atr2, "    ") != 0)
    {
        //Seta_string(atr2, sizeof(atr2));
        strcat(linha, atr2);
    }
    strcat(linha, "\n");
    printf("\n%d ", new_fp);
    fputs(linha, new_fp);
}

void Cria_arquivo(token *tk){

    strcat(nome_arquivo, tk->lexema);
    strcat(nome_arquivo, ".txt");

    new_fp = fopen(nome_arquivo, "r");

    if (new_fp != NULL)
    {
        remove(nome_arquivo);
        printf("O arquivo '%s' já existe e foi bÃÃÃnidu.\n", nome_arquivo);
    }

    new_fp = fopen(nome_arquivo, "w");

    // Verifica se o arquivo foi aberto com sucesso
    if (new_fp == NULL) {
        printf("Erro ao criar o arquivo.\n");
    }

    printf("%d", new_fp);

}

void Gera_operador(char op[])
{
    Gera("    ",op,"    ","    ");
}
