/*
FALTANDO COLOCAR OS RETURNS NOS PROCS E FUNCS 
NOS FUNCS TENHO QUE COLOCAR O RESULTADO NO ROTULO 0

*/
#pragma once
#include <stdlib.h> 
#include <stdio.h> 
#include <string.h> 
#include <ctype.h> 
#include "definicoes.h"

FILE *new_fp; 
char nome_arquivo[30];
int qntd_var = 0;
int qntd_rotulo = 0;
char str_aux_atr1[4], str_aux_atr2[4], str_aux_rot[4], str_aux_inst[8];

void Seta_string(char *str, int size, int filled);
void Gera_operador(char op[]);
void Gera_load_variavel(entrada_tab_simbolos *entrada);
void Gera_load_const(char num[]);
void Gera_start_programn();
void Gera_end_programn();
void Gera_jmp(int rotulo);
void Gera_jmpf(int rotulo);
void Gera_rotulo(int rotulo);
char* new_rotulo (int num_var);
void Gera_alloc(int num_var);
void Gera_dalloc(int num_var);
void Gera_leia(char rotulo[4]);
void Gera_escreva(char rotulo[4]);
void Gera_call(char rotulo[4]);
void Gera_return();
void Gera_str(char rotulo[4]);
void Gera(char rotulo[4], char instrucao[8], char atr1[4], char atr2[4]);
void Cria_arquivo();

//essa função vou implementar dps, mas ela é so para formatar
void Seta_string(char *str, int size, int filled){
    memset(str,' ', size-filled-1);
}

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
    Gera_dalloc(1);
    Gera("    ","HLT","    ","    ");
}

void Gera_jmp(int rotulo)
{
    Gera("    ","JMP",itoa(rotulo,str_aux_atr1,10),"    ");
}

void Gera_jmpf(int rotulo)
{
    Gera("    ","JMPF",itoa(rotulo,str_aux_atr1,10),"    ");
}

void Gera_rotulo(int rotulo)
{
    Gera(itoa(rotulo,str_aux_rot,10),"NULL","    ","    ");    
}

char* new_rotulo_var (int num_var)
{
    itoa(qntd_var + num_var,str_aux_atr1,10);
    Seta_string(&(str_aux_atr1[strlen(str_aux_atr1)]), sizeof(str_aux_atr1) / sizeof(str_aux_atr1[0]), strlen(str_aux_atr1));
    return str_aux_atr1; 
}

char* new_rotulo (int rotulo)
{
    itoa(rotulo,str_aux_atr1,10);
    Seta_string(&(str_aux_atr1[strlen(str_aux_atr1)]), sizeof(str_aux_atr1) / sizeof(str_aux_atr1[0]), strlen(str_aux_atr1));
    return str_aux_atr1; 
}

void Gera_alloc(int num_var)
{
    Gera("    ","ALLOC",itoa(qntd_var,str_aux_atr1,10),itoa(num_var,str_aux_atr2,10));
    qntd_var = qntd_var + num_var;
}

void Gera_dalloc(int num_var)
{
    Gera("    ","DALLOC",itoa(qntd_var - 1,str_aux_atr1,10),itoa(num_var,str_aux_atr2,10));
    qntd_var = qntd_var - num_var;
}

void Gera_leia(char rotulo[4])
{
    Gera("    ","RD","    ","    ");
    //retorno da tabela de simbolos
    Gera("    ","STR",rotulo,"    ");        
}

void Gera_escreva(char rotulo[4])
{
    //retorno da tabela de simbolos pela busca do token.lexema
    Gera("    ","LDV",rotulo,"    ");   
    Gera("    ","PRN","    ","    ");
}

void Gera_call(char rotulo[4])
{
    Gera("    ","CALL",rotulo,"    ");
}

void Gera_return()
{
    Gera("    ","RETURN","    ","    ");
}

void Gera_str(char rotulo[4])
{
    Gera("    ","STR",rotulo,"    ");
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
    if (strcmp(atr2, "    ") != 0)
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