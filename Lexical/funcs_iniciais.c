#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "definicoes.h"


typedef struct Node Node;

struct Node{ 
  token tk;          // ver ponteiro / string 
  Node *prev;
}; 

char caractere;
char *filename; 
FILE *fp; 
Node *atual;

void printList();

void ler()
{ 
    caractere = fgetc(fp); 
} 
/// Cria um token baseado num array de char passado
void set_token_s(token *tk, char string[])
{ 
    memset (tk->lexema, 0, lexema_size_max);
    memcpy ( tk->lexema, string, strlen(string)+1 ); 
} 

/// Cria um token baseado no caractere atual
void set_token_c(token *tk)
{ 
    memset (tk->lexema, 0, lexema_size_max);
    memcpy ( tk->lexema, &caractere, sizeof(caractere) ); 
} 


void trata_digito(token *tk){ 
    char num[lexema_size_max]; 
    while(isdigit(caractere)){ 
        strncat(num, &caractere, 1);  
        ler(); 
    }
    set_token_s (tk, num);
    tk->simbolo = snum;
} 

void trata_atribuicao(token *tk){ 
    ler(); 
    if(caractere == '='){ 
        set_token_s (tk, ":="); 
        tk->simbolo = satribuicao; 
        ler(); 
    }else{  
        set_token_s (tk, ":"); 
        tk->simbolo = sdoispontos; 
    }
}

void trata_aritmetico(token *tk){ 
    set_token_c (tk);  
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
    set_token_c (tk);  
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
    ler();
} 

void insere_lista(token *tk)
{
    Node *no = (Node*) malloc(sizeof(Node));
    memcpy ( &(no->tk), tk, sizeof(token) );

    if(atual != NULL)
    {
        no->prev = atual;
    }    
    else
    {
        no->prev = NULL;
    }

    atual = no;
}

void trata_ident_reserv(token *tk){
    char id[lexema_size_max] = "";
    strncat(id, &caractere, 1);  
    ler();
    
    while(isalnum(caractere) || (caractere == 95)){
        strncat(id, &caractere, 1); 
        ler();
    }
    char ao = caractere;
    set_token_s(tk, id);

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
    }else if (strcmp(id, "inicio") == 0) {
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

void trata_relacional(token *tk){
    char switchcaractere = caractere;
    ler();
    switch(switchcaractere){
        case '!':
            if(caractere == '='){
                set_token_s(tk, "!=");
                tk->simbolo = sdif;
                ler();
            }
            else{
                //erro
            }
            break;
        case '<':
            if(caractere == '='){
                set_token_s(tk, "<=");
                tk->simbolo = smenorig;
                ler();
            }
            else{
                set_token_s(tk, "<");
                tk->simbolo = smenor;
            }
            break;
        case '>':
            if(caractere == '='){
                set_token_s(tk, ">=");
                tk->simbolo = smaiorig;
                ler();
            }
            else{
                set_token_s(tk, ">");
                tk->simbolo = smaior;
            }
            break;
    }
}

void print_token(token *tk){
    printf("==================================================================================\n");
    printf("Lexema: %s\tSimbolo:%s\n", tk->lexema, print_enum(tk->simbolo));
    printf("==================================================================================\n");
}

void printList()
{
    Node *no = atual;
    while (no->prev != NULL)
    {
        print_token(no);
        no = no->prev;
    }
    print_token(no);
}

void desalocador()
{
    Node *no = atual;
    while (no->prev != NULL)
    {
        atual = no->prev;
        free(no);
        no = atual;
    }    
    free(no);
}

void pega_token(token *tk){ 
   char aux = caractere;
    if(isdigit(caractere)){ 
        trata_digito(tk);  
    }else{ 
        if(isalpha(caractere)){ 
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
        while(caractere == '{') 
        {
                while(caractere != '}' && caractere != EOF) 
                { 
                    ler(); 
                }            
                if(caractere == EOF) 
                { 
                    ///error()
                } 
                ler(); 
        } 

        while(caractere == ' ' || caractere == '\n')
        {
            ler();
        }

        if(caractere != EOF) 
        { 
            ///printf("%c", caractere);
            token tk;
            pega_token(&tk);
            insere_lista(&tk);
        }
    } 
    printList();
    desalocador();
    fclose(fp); 
}

int main(){
    AnalisadorLexicalN1();
}