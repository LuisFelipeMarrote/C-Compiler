/*
 FALTANDO COLOCAR OS RETURNS NOS PROCS E FUNCS.
 COLOCAR UM ROTULO CASO ENTRE EM FUNCS
 NOS FUNCS TENHO QUE COLOCAR O RESULTADO NO ROTULO 0.
 CHECAR O TAMANHO DA CONSTANTE QUE CHEGA NA FUNÇAO Gera_load_const e tem que passar para o formato. - TA FEITO ACHO, CHECAR A SOLUÇÃO, POIS ELA É ESQUISITA
 O LIMITE PARA LOAD CONSTANTE É 23 POR ALGUM MOTIVO, DESCOBRIR.
 ANALISAR SOBRE A NECESSIDADE DE MANTER AAQUI E LA AS VARIAVEIS QUE CONTAM OS ROTULO E ROTULOS DE VAR. EXCLUIR EM ALGUM DO LUGARES.
 PARA SOLUCIONAR O PROBLEMA DA FORMATAÇÃO TAMBÉM DA PARA SETAR AS VARIAAVEIS AAUX COM ESPAÇO E DEPOIS DAR MEMCPY DO CONTEUDO NELAS,
AI EU SEMPRE USO AS VARIÁVEIS GLOBAIS E AS FUNÇOES SO SETAM E QUANDO VOLTAR SO LIMPAM ONDE USARAM. SOLUÇÃO BOA ESSA.  
 CRIAR UM FUNÇÃO PARA RETORNAR O NUMERO DE VARIAVEIS (INT) ATÉ A MARCA, PARA FAZER O DALLOC. 
 FAZER UMA FUNÇÃO QUE RETORNE (BOOL) CASO ESTEJA SENDO FEITA UMA ATRIBUIÇÃO PARA O UMA FUNÇÃO, ELA SO É PERMITIDA CASO ESTEJA DENTRO DELA E POSSUA O MESMO NOME.
CASO ISSO FOR VERDADE EU VOU FAZER UM STR 0. 

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
char str_aux_atr1[5], str_aux_atr2[5], str_aux_rot[5], str_aux_inst[9];

void Seta_string(char *str, int size, int filled);
void Gera_operador(enum tipos op);
void Gera_load_variavel(char rotulo[5]);
void Gera_load_const(char num[]);
void Gera_start_programn();
void Gera_end_programn();
void Gera_jmp(char rotulo[5]);
void Gera_jmpf(char rotulo[5]);
void Gera_rotulo(char rotulo[5]);
char* str_rotulo_var (int rotulo);
char* str_rotulo (int num_var);
void Gera_alloc(int num_var);
void Gera_dalloc(int num_var);
void Gera_leia(char rotulo[5]);
void Gera_escreva(char rotulo[5]);
void Gera_call(char rotulo[5]);
void Gera_return();
void Gera_str(char rotulo[5]);
void Gera(char rotulo[5], char instrucao[9], char atr1[5], char atr2[5]);
void Gera_final(char rotulo[5], char instrucao[9], char atr1[5], char atr2[5]);
void Cria_arquivo();

//essa função vou implementar dps, mas ela é so para formatar
void Seta_string(char *str, int size, int filled){
    int tam = size-filled-1; // -1 POIS TEM QUE CONTAR COM O \0 NO FINAL.
    memset(str,' ', tam);
}

void Gera_operador(enum tipos op)
{
    if (op == smais) {
        Gera("    ","ADD     ","    ","    ");
    } else if (op == smenos) {
        Gera("    ","SUB     ","    ","    ");
    }else if (op == smult) {
        Gera("    ","MULT    ","    ","    "); 
    } else if (op == sdiv) {
        Gera("    ","DIVI    ","    ","    ");
    } else if (op == ssinalu) {
        Gera("    ","INV     ","    ","    ");
    } else if (op == se) {
        Gera("    ","AND     ","    ","    ");
    } else if (op == sou) {
        Gera("    ","OR      ","    ","    ");
    } else if (op == snao) { 
        Gera("    ","NEG     ","    ","    ");
    }else if (op == smaior) {
        Gera("    ","CMA     ","    ","    ");
    }else if (op == smenor) {
        Gera("    ","CME     ","    ","    ");
    }else if (op == sig) {
        Gera("    ","CEQ     ","    ","    ");
    }else if (op == sdif) {
        Gera("    ","CDIF    ","    ","    ");
    }else if (op == smenorig) {
        Gera("    ","CMEQ    ","    ","    ");
    }else if (op == smaiorig) {
        Gera("    ","CMAQ    ","    ","    ");
    }
}

void Gera_load_variavel(char rotulo[5])
{
    Gera("    ","LDV     ",rotulo,"    ");
}

void Gera_load_const(char num[5])
{
    printf("\n oi%d\n", strlen(num));
    printf("\n AQUI%d\n", sizeof(str_aux_atr1) / sizeof(str_aux_atr1[0]));
    if(strlen(num) > 4){
        memcpy(str_aux_atr1, num, 4);
        Gera("    ","LDC     ",str_aux_atr1,"    ");
    } else {    
        Seta_string(&(num[strlen(num)]), 5, strlen(num));
        Gera("    ","LDC     ",num,"    ");
    }
}

void Gera_start_programn()
{
    Gera("    ","START   ","    ","    ");
    Gera_alloc(1);
}

void Gera_end_programn()
{
    Gera_dalloc(1);
    Gera_final("    ","HLT     ","    ","    ");
}

void Gera_jmp(char rotulo[5])
{
    Gera("    ","JMP     ",rotulo,"    ");
}

void Gera_jmpf(char rotulo[5])
{
    Gera("    ","JMPF    ",rotulo,"    ");
}

void Gera_rotulo(char rotulo[5])
{
    Gera(rotulo,"NULL    ","    ","    ");    
}

char* str_rotulo_var (int rotulo)
{    
    char *str = str_aux_atr1;
    itoa(rotulo,str_aux_atr1,10);
    Seta_string(&(str_aux_atr1[strlen(str_aux_atr1)]), sizeof(str_aux_atr1) / sizeof(str_aux_atr1[0]), strlen(str_aux_atr1));
    return str_aux_atr1; 
}

char* str_rotulo (int rotulo)
{    
    itoa(rotulo,str_aux_atr1,10);
    Seta_string(&(str_aux_atr1[strlen(str_aux_atr1)]), sizeof(str_aux_atr1) / sizeof(str_aux_atr1[0]), strlen(str_aux_atr1));
    return str_aux_atr1;     
}

//A função str_rotulo2 precisa existir, pois o alloc precisaa de 2 parametros e a funçao str_rotulo estava devolvendo a mesma posição de memoria
char* str_rotulo2 (int rotulo)
{    
    itoa(rotulo,str_aux_atr2,10);
    Seta_string(&(str_aux_atr2[strlen(str_aux_atr2)]), sizeof(str_aux_atr2) / sizeof(str_aux_atr2[0]), strlen(str_aux_atr2));
    return str_aux_atr2;     
}

void Gera_alloc(int num_var)
{
    Gera("    ","ALLOC   ",str_rotulo(qntd_var),str_rotulo2(num_var));
    qntd_var = qntd_var + num_var;
}

// vou tirar daqui a variavel qntd_var e passar como parametro dps.
void Gera_dalloc(int num_var)
{
    Gera("    ","DALLOC  ",str_rotulo(qntd_var - num_var),str_rotulo2(num_var));
    qntd_var = qntd_var - num_var;
}

void Gera_leia(char rotulo[5])
{
    Gera("    ","RD      ","    ","    ");
    Gera("    ","STR     ",rotulo,"    ");        
}

void Gera_escreva(char rotulo[5])
{
    Gera("    ","LDV     ",rotulo,"    ");   
    Gera("    ","PRN     ","    ","    ");
}

void Gera_call(char rotulo[5])
{
    Gera("    ","CALL    ",rotulo,"    ");
}

void Gera_return()
{
    Gera("    ","RETURN  ","    ","    ");
}

void Gera_str(char rotulo[5])
{
    Gera("    ","STR     ",rotulo,"    ");
}

 
void Gera(char rotulo[5], char instrucao[9], char atr1[5], char atr2[5]){

    //Seta_string(&(instrucao[strlen(instrucao)]), 9, strlen(instrucao));
    // Isso daqui so nao da certo, pois eu nao posso alterar essa regiao de memoria que foraam passados por parametros, pois passei como string literal,
    // ou seja, so da para ler.
    // Um jeito de consetar é copiar para outra string auxiliar. 

    char linha[22] = {}; //ver como inicializa a lista vazia 

    strcat(linha, rotulo);
    strcat(linha, instrucao);
    strcat(linha, atr1);
    strcat(linha, atr2);    

    strcat(linha, "\n");
    fputs(linha, new_fp);
}

//feio, mas resolve.
void Gera_final(char rotulo[5], char instrucao[9], char atr1[5], char atr2[5]){

    //Seta_string(&(instrucao[strlen(instrucao)]), 9, strlen(instrucao));
    // Isso daqui so nao da certo, pois eu nao posso alterar essa regiao de memoria que foraam passados por parametros, pois passei como string literal,
    // ou seja, so da para ler.
    // Um jeito de consetar é copiar para outra string auxiliar. 

    char linha[22] = {}; //ver como inicializa a lista vazia 

    strcat(linha, rotulo);
    strcat(linha, instrucao);
    strcat(linha, atr1);
    strcat(linha, atr2);    
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