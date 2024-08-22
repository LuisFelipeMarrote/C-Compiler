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

#include "definicoes.h"

char caractere;

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
            tk->simbolo = "smais";
            break;
        case '-': 
            tk->simbolo = "smenos" ;
            break;
        case '*':  
            tk->simbolo = "smul" ;
            break;
    }
    ler(); 
} 