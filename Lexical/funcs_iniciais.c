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


//falta
void trata_digito(token *tk){ 
    /* Falta:
        confirmar concatenação de caracteres
    */ 

    char num[lexema_size_max]; 
    while(caractere > 47 && caractere < 58){ 
        strncat(num, &caractere, 1);  // ver como fazer isso para string
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
    char caract_aux[lexema_size_max] = "";
    strncat(caract_aux, &caractere, 1);  
    ler();
    
    while((47 < caractere && caractere < 58 )|| (64 < caractere && caractere < 91) ||
          (96 < caractere && caractere < 123) || (caractere == 95)){
        strncat(caract_aux, &caractere, 1); 
        ler();
    }
    char ao = caractere;
    set_token_s(tk, caract_aux);

    if (strcmp(caract_aux, "programa") == 0) {
        tk->simbolo = sprograma;
    } else if (strcmp(caract_aux, "se") == 0) {
        tk->simbolo = sse;
    }else if (strcmp(caract_aux, "entao") == 0) {
        tk->simbolo = sentao;
    }else if (strcmp(caract_aux, "senao") == 0) {
        tk->simbolo = ssenao;
    }else if (strcmp(caract_aux, "enquanto") == 0) {
        tk->simbolo = senquanto;
    }else if (strcmp(caract_aux, "faca") == 0) {
        tk->simbolo = sfaca;
    }else if (strcmp(caract_aux, "inicio") == 0) {
        tk->simbolo = sinício;
    }else if (strcmp(caract_aux, "fim") == 0) {
        tk->simbolo = sfim;
    }else if (strcmp(caract_aux, "escreva") == 0) {
        tk->simbolo = sescreva;
    }else if (strcmp(caract_aux, "leia") == 0) {
        tk->simbolo = sleia;
    }else if (strcmp(caract_aux, "var") == 0) {
        tk->simbolo = svar;
    }else if (strcmp(caract_aux, "inteiro") == 0) {
        tk->simbolo = sinteiro;
    }else if (strcmp(caract_aux, "booleano") == 0) {
        tk->simbolo = sbooleano;
    }else if (strcmp(caract_aux, "verdadeiro") == 0) {
        tk->simbolo = sverdadeiro;
    }else if (strcmp(caract_aux, "falso") == 0) {
        tk->simbolo = sfalso;
    }else if (strcmp(caract_aux, "procedimento") == 0) {
        tk->simbolo = sprocedimento;
    }else if (strcmp(caract_aux, "funcao") == 0) {
        tk->simbolo = sfuncao;
    }else if (strcmp(caract_aux, "div") == 0) {
        tk->simbolo = sdiv;
    }else if (strcmp(caract_aux, "e") == 0) {
        tk->simbolo = se;
    }else if (strcmp(caract_aux, "ou") == 0) {
        tk->simbolo = sou;
    }else if (strcmp(caract_aux, "nao") == 0) {
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

void printList()
{
    Node *no = atual;
    while (no->prev != NULL)
    {
        printf("%s", no->tk.lexema);
        no = no->prev;
    }
    printf("%s", no->tk.lexema);
}

void desibimbador()
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
    /*falta:  
        - caractere é letra (se atentar a exceções como 'ç') 
        - erro (descobrir o que retornar) 
    */  
   char aux = caractere;
    if(47 < caractere && caractere < 58){ 
        trata_digito(tk);  
    }else{ 
        if((64 < caractere && caractere < 91) || (96 < caractere && caractere < 123)){ 
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
    desibimbador();
    fclose(fp); 
}

int main(){
    AnalisadorLexicalN1();
}