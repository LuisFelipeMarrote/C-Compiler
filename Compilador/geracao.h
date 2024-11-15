#pragma once
#include <stdlib.h> 
#include <stdio.h> 
#include <string.h> 
#include <ctype.h> 
#include "definicoes.h"

FILE *new_fp; 
char nome_arquivo[30];
int qntd_atual = 0;

void Gera_operador(char op[]);
void Gera_load_variavel(entrada_tab_simbolos *entrada);
void Gera_load_const(char num[]);
void Gera_start_programn();
void Gera_end_programn();
void Gera_jmp(char rotulo[]);
void Gera_jmpf(char rotulo[]);
void Gera_rotulo(char rotulo[]);
void Gera_alloc(int num_var);
void Gera_dalloc(int num_var);
void Gera_call(char rotulo[]);
void Gera_return();
void Gera(char rotulo[4], char instrucao[8], char atr1[4], char atr2[4]);
void Cria_arquivo();

// essa função vou implementar dps, mas ela é so para formatar
// void Seta_string(char str[], int size){
//     char ajuda[size];
//     memset(ajuda,' ', size);
//     str[0] = 'a';
// }

void Gera_operador(char op[])
{
    if (strcmp(op, "+") == 0) {
        Gera("    ","ADD","    ","    ");
    } else if (strcmp(op, "-") == 0) {
        Gera("    ","SUB","    ","    ");
    }else if (strcmp(op, "*") == 0) {
        Gera("    ","MULT","    ","    ");
    } else if (strcmp(op, "/") == 0) {
        Gera("    ","DIVI","    ","    ");
    } else if (strcmp(op, "-") == 0) { // conferir qual é o operador
        Gera("    ","INV","    ","    ");
    } else if (strcmp(op, "e") == 0) {
        Gera("    ","AND","    ","    ");
    } else if (strcmp(op, "ou") == 0) {
        Gera("    ","OR","    ","    ");
    } else if (strcmp(op, "nao") == 0) {
        Gera("    ","NEG","    ","    ");
    }else if (strcmp(op, "<") == 0) {
        Gera("    ","CME","    ","    ");
    }else if (strcmp(op, ">") == 0) {
        Gera("    ","CMA","    ","    ");
    }else if (strcmp(op, "=") == 0) {
        Gera("    ","CEQ","    ","    ");
    }else if (strcmp(op, "!=") == 0) {
        Gera("    ","CDIF","    ","    ");
    }else if (strcmp(op, "<=") == 0) {
        Gera("    ","CMEQ","    ","    ");
    }else if (strcmp(op, ">=") == 0) {
        Gera("    ","CMAQ","    ","    ");
    }
}

/* falta mudar a tabela de simbolos
void Gera_load_variavel(entrada_tab_simbolos *entrada)
{
    Gera("    ","LDV",entrada->rotulo,"    ");
}
*/

void Gera_load_const(char num[])
{
    Gera("    ","LDC",num,"    ");
}

void Gera_start_programn()
{
    Gera("    ","START","    ","    ");
    Gera_alloc(1);
}

void Gera_end_programn()
{
    Gera("    ","HLT","    ","    ");
}

void Gera_jmp(char rotulo[])
{
    Gera("    ","JMP",rotulo,"    ");
}

void Gera_jmpf(char rotulo[])
{
    Gera("    ","JMPF",rotulo,"    ");
}

void Gera_rotulo(char rotulo[])
{
    Gera(rotulo,"NULL","    ","    ");    
}

void Gera_alloc(int num_var)
{
    char str_aux1[4], str_aux2[4];
    Gera("    ","ALLOC",itoa(qntd_atual,str_aux1,10),itoa(num_var,str_aux2,10));
    qntd_atual = qntd_atual + num_var;
}

void Gera_dalloc(int num_var)
{
    char str_aux1[4], str_aux2[4];
    Gera("    ","DALLOC",itoa(qntd_atual - 1,str_aux1,10),itoa(num_var,str_aux2,10));
    qntd_atual = qntd_atual - num_var;
}

void Gera_leia(char rotulo[])
{
    Gera("    ","RD","    ","    ");
    //retorno da tabela de simbolos
    //Gera("    ","STR",rotulo,"    ");        
}

void Gera_escreva(char rotulo[])
{
    //retorno da tabela de simbolos pela busca do token.lexema
    //Gera("    ","LDV",rotulo,"    ");   
    Gera("    ","PRN","    ","    ");
}

void Gera_call(char rotulo[])
{
    Gera("    ","CALL",rotulo,"    ");
}

void Gera_return()
{
    Gera("    ","RETURN","    ","    ");
}

void Gera(char rotulo[4], char instrucao[8], char atr1[4], char atr2[4]){
    //redeclara isso, pois ele ta passando como string literal, ou seja, so da para ler
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

    printf("Arquivo criado com sucesso!\n");

}