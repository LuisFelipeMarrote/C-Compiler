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
FILE *fp; 
token *tk_main;
int *linha;

void ler();
void trata_digito(token *tk);
void trata_atribuicao(token *tk);
void trata_aritmetico(token *tk);
void trata_pontuacao(token *tk);
void trata_ident_reserv(token *tk);
void trata_relacional(token *tk);
void print_token(token *tk);
void pega_token(token *tk);
void AnalisadorLexicalN1();
void lexical_error(int n, token *tk);
void AnalisadorLexical(FILE *fp_main, int *linha_main, token *token);
void print_linhas();

void ler()
{ 
    caractere = fgetc(fp); 
} 

void print_linhas()
{
    printf("linhas: %d", *linha); // fazer uma função para printar as linhas
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
    tk->simbolo = snúmero;
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
            tk->simbolo = smult ;
            break;
    }
    ler(); 
} 

void trata_pontuacao(token *tk){
    set_token_c (tk);  
    switch (caractere)
    {
    case ',':
        tk->simbolo = svírgula;
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
        tk->simbolo = sponto;
        break;    
    default:
        break;
    }
    ler();
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
                ungetc(caractere, fp);
                lexical_error(4, tk);
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
        case '=':
            set_token_s(tk, "=");
            tk->simbolo = sig;
            break;
    }
}

void print_token(token *tk){
    printf("Lexema: %s\tSimbolo: %s\n", tk->lexema, print_enum(tk->simbolo));
    printf("==================================================================================\n");
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
                            lexical_error(3, tk); 
                        } 
                    } 
                } 
            } 
        } 
    }
} 

void lexical_error(int n, token *tk){
    char* erros[] = {"",
        "1: Chaves Abertas ('{') sem chaves fechadas depois (era esperado '}')",
        "2: Chaves Fechadas ('}') sem chaves abertas antes (era esperado '{' antes)",
        "3: Caractere Inválido",
        "4: Ocorrencia de Exclamação ('!') sem Sinal de Igual ('=') depois (símbolo inválido)" 
    };
    
    printf("Erro lexical");
    printf(" %s ", erros[n]);
    printf("na linha %d\n", *linha);

    
    set_token_s(tk, "Erro");
    tk->simbolo = serro;

    ler();
}

void AnalisadorLexicalN1()
{
    if (fp == NULL) 
    { 
        printf("Error: could not open file"); 
        return; 
    } 


    while (caractere != EOF)
    {
        while ((caractere == '{' || caractere == ' ' || caractere == '\n' || caractere == '}') && caractere != EOF) 
        {        
            if(caractere == '{') 
            {
                int abre = 0;
                    while((caractere != '}' || abre > 1) && caractere != EOF) 
                    { 
                        if(caractere == '{') 
                            abre++;
                        if(caractere == '}') 
                            abre--;
                        if(caractere == '\n')
                            (*linha)++;
                        ler(); 
                    }            
                    if(caractere == EOF) 
                    { 
                        lexical_error(1, tk_main);
                        return;
                    } 
                    ler(); 
            } 

            if(caractere == '}'){
                lexical_error(2, tk_main);
                return;
            }

            while(caractere == ' ' || caractere == '\n')
            {
                if(caractere == '\n'){
                    (*linha)++;
                }
                ler();
            }
        }             
        
        if(caractere != EOF) 
        { 
            //printf("\n%c\n", caractere);
            pega_token(tk_main);
            return;
        }
    }
}

void AnalisadorLexical(FILE *fp_main, int *linha_main, token *token){
    linha = linha_main;
    fp = fp_main;
    tk_main = token;
    AnalisadorLexicalN1();    
}