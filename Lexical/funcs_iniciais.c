/*
==============================================================================================

Ainda não tenho certeza da melhor forma de colocar todas as funções (um aqeuivo ou vários),
mas vou colocar todos aqui por enquanto só para arrumar coisas como identação


Faltas atuais:
    -> pega_token:
        - caractere é letra (se atentar a exceções como 'ç') 
        - erro (descobrir o que retornar) 
    -> trata_digito:
        - confirmar concatenação de caracteres

==============================================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include "definicoes.h"

char caractere;
char *filename; 
FILE *fp; 
Node *primeiro = NULL;
Node *atual = NULL;

void ler()
{ 
    caractere = fgetc(fp); 
} 

typedef struct{ 
  token tk;          // ver ponteiro / string 
  Node *next;
} Node; 

//falta:
void pega_token(token *tk){ 
    /*falta:  
        - caractere é letra (se atentar a exceções como 'ç') 
        - erro (descobrir o que retornar) 
    */  
    if(47 < caractere && caractere < 58){ 
        trata_digito(tk);  
    }else{ 
        if(/*caractere é letra*/1){ 
            trata_ident_reserv(tk); 
        }else{ 
            if(caractere == ':'){ 
                trata_atribuicao(tk); 
            }else{ 
                if(caractere == '+' || caractere == '-' || caractere == '*'){ 
                    trata_aritmetico(tk); 
                }else{ 
                    if(caractere == '!' || caractere == '<' || caractere == '>' || caractere == '='){ 
                        trata_relacional(tk); 
                    }else{ 
                        if(caractere == ';' || caractere == ','|| caractere == '('|| caractere == ')'|| caractere == '.'){ 
                            trata_pontuacao(tk); 
                        }else{ 
                            /*erro*/ 
                        } 
                    } 
                } 
            } 
        } 
    }
} 

//falta
void trata_digito(token *tk){ 
    /* Falta:
        confirmar concatenação de caracteres
    */
    char num = caractere; 
    ler(); 
    while(caractere > 47 && caractere < 58){ 
       num = num + caractere; // ver como fazer isso para string 
        ler(); 
    } 
    tk-> simbolo = snum; 
    tk->lexema = num; 
} 

void trata_atribuicao(token *tk){ 
    ler(); 
    if(caractere == '='){ 
        tk->lexema = ":="; 
        tk->simbolo = satribuicao; 
        ler(); 
    }else{  
        tk->lexema = ':'; 
        tk->simbolo = sdoispontos; 
    } 
} 

void trata_aritmetico(token *tk){ 
    tk->lexema = caractere; 
    switch(caractere){
        case '+': 
            tk->simbolo = smais;
            break;
        case '-': 
            tk->simbolo = smenos ;
            break;
        case '*':  
            tk->simbolo = smul ;
            break;
    }
    ler(); 
} 


void trata_pontuacao(token *tk){ 
    tk->lexema = caractere;
    switch (caractere)
    {
    case ',':
        tk->simbolo = svirgula;
        break;
    case '(':
        tk->simbolo = sabre_parenteses;
        break;
    case ')':
        tk->simbolo = sfecha_parenteses;
        break;
    case ';':
        tk->simbolo = sponto_virgula;
        break;    
    case '.':
        tk->simbolo = sfim;
        break;    
    default:
        break;
    }
} 

void insere_lista(token *tk)
{
    Node no;
    no.tk = *tk;
    no.next = NULL;

    if(primeiro != NULL)
    {
        atual->next = no;
        atual = no;
    }
    else
    {
        primeiro = &no;
        atual = primeiro;
    }
}

void AnalisadorLexicalN1()
{
    filename = "new.txt";
    fp = fopen(filename, "r");

    if (fp == NULL) 
    { 
        printf("Error: could not open file %s", filename); 
        return; 
    } 
    ler(); 

    while (caractere != EOF) 
    {
        while((caractere == '{' || caractere == ' ' || caractere == '\n') && caractere != EOF) 
        {
            if(caractere == '{') 
            { 
                while(caractere != '}' && caractere != EOF) 
                { 
                    ler(); 
                }
                ler(); 
            }                 

            while((caractere == ' ' || caractere == '\n') && caractere != EOF) 
            { 
                ler(); 
            } 
        } 

        if(caractere != EOF) 
        { 
            printf("%c", caractere);
            ler();
            token tk;
            pega_token(&tk);
            insere_lista(&tk);
        }
    } 

    fclose(fp); 
}

void trata_ident_reserv(token *tk){
    /*Falta:
        - caractere/letra/'_'
        - id = id + caractere
    */
    char* id = caractere;
    ler();
    while(1/* caractere é letra ou dígito ou "_' */){
        id = id + caractere; //rever
        ler();
    }
    tk->lexema = id;

    if (strcmp(id, "programa") == 0) {
        tk->simbolo = sprograma;
    } else if (strcmp(id, "se") == 0) {
        tk->simbolo = sse;
    }else if (strcmp(id, "entao") == 0) {
        tk->simbolo = sentao;
    }else if (strcmp(id, "senao") == 0) {
        tk->simbolo = ssenao;
    }else if (strcmp(id, "enquanto") == 0) {
        tk->simbolo = senquanto;
    }else if (strcmp(id, "faca") == 0) {
        tk->simbolo = sfaca;
    }else if (strcmp(id, "início") == 0) {
        tk->simbolo = sinício;
    }else if (strcmp(id, "fim") == 0) {
        tk->simbolo = sfim;
    }else if (strcmp(id, "escreva") == 0) {
        tk->simbolo = sescreva;
    }else if (strcmp(id, "leia") == 0) {
        tk->simbolo = sleia;
    }else if (strcmp(id, "var") == 0) {
        tk->simbolo = svar;
    }else if (strcmp(id, "inteiro") == 0) {
        tk->simbolo = sinteiro;
    }else if (strcmp(id, "booleano") == 0) {
        tk->simbolo = sbooleano;
    }else if (strcmp(id, "verdadeiro") == 0) {
        tk->simbolo = sverdadeiro;
    }else if (strcmp(id, "falso") == 0) {
        tk->simbolo = sfalso;
    }else if (strcmp(id, "procedimento") == 0) {
        tk->simbolo = sprocedimento;
    }else if (strcmp(id, "funcao") == 0) {
        tk->simbolo = sfuncao;
    }else if (strcmp(id, "div") == 0) {
        tk->simbolo = sdiv;
    }else if (strcmp(id, "e") == 0) {
        tk->simbolo = se;
    }else if (strcmp(id, "ou") == 0) {
        tk->simbolo = sou;
    }else if (strcmp(id, "nao") == 0) {
        tk->simbolo = snao;
    }else{
        tk-> simbolo = sidentificador;
    }

}

