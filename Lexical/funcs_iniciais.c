#include "definicoes.h"

/*
==============================================================================================

Ainda não tenho certeza da melhor forma de colocar todas as funções (um aqeuivo ou vários),
mas vou colocar todos aqui por enquanto só para arrumar coisas como identação

==============================================================================================
*/

char caractere;

void pega_token(token token_inst){ 
    /*falta:  
        - confirmar caractere é dígito (tirar tabela ascii e 0 <= caractere <= 9) 
        - caractere é letra (se atentar a exceções como 'ç') 
        - erro (descobrir o que retornar) 
    */  
    if(47 < caractere && caractere < 58){ 
        trata_digito(token_inst);  
    }else{ 
        if(/*caractere é letra*/1){ 
            trata_ident_reserv(token_inst); 
        }else{ 
            if(caractere == ':'){ 
                trata_atribuicao(token_inst); 
            }else{ 
                if(caractere == '+' || caractere == '-' || caractere == '*'){ 
                    trata_aritmetico(token_inst); 
                }else{ 
                    if(caractere == '!' || caractere == '<' || caractere == '>' || caractere == '='){ 
                        trata_relacional(token_inst); 
                    }else{ 
                        if(caractere == ';' || caractere == ','|| caractere == '('|| caractere == ')'|| caractere == '.'){ 
                            trata_pontuacao(token_inst); 
                        }else{ 
                            /*erro*/ 
                        } 
                    } 
                } 
            } 
        } 
    }
} 

token trataDigito(token *token_inst){ 
    char num = caractere; 
    ler(); 
    while(caractere > 47 && caractere < 58){ 
       num = num + caractere; // ver como fazer isso para string 
        ler(); 
    } 
    token_inst->simbolo = "snum"; 
    token_inst->lexema = num; 
} 
