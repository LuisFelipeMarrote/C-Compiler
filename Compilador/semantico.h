#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "definicoes.h"
#include "lexico.h"
#include "simbolos.h"
void semantic_error(int n){
    //rever todos os rotulos de erro abaixo (placeholders)
    char* erros[] = {"falta definir",
        "1: ",
        };
    
    printf("Erro semantico");
    printf(" %s ", erros[n]);
    printf("na linha %d\n", *linha);

}

//recebe expressao como lista de tokens (esquerda = início) e retorna lista pos-fixa
node_lista_token* converte_inf_posfix(node_lista_token* lista_infix){
    node_lista_token* lista_pos; // inicio da lista (sai depois)
    
    while(lista_infix != NULL){
        if(lista_infix->tk.simbolo == snúmero || lista_infix->tk.simbolo == sidentificador){
            //Se for variável ou número copia na saída; 

        }else if(lista_infix->tk.simbolo == sabre_parenteses){
            //Se for abre-parênteses empilha; 

        }else if(lista_infix->tk.simbolo == sfecha_parenteses){     
            //Se for fecha-parênteses, desempilha tudo copiando na saída até o primeiro abre-parênteses (desempilhando-o);

        }else if(0/*operadores*/){
            /*Se for operador, empilha após percorrer desempilhando e copiando na saída todos os operadores com precedência maior ou igual ao 
            que será empilhado, isso até encontrar o primeiro operador com precedência menor, terminar a pilha ou encontrar o primeiro abreparênteses*/

        }
    }
    //Se terminar a expressão, desempilhar copiando na saída todos os operadores ainda existentes na pilha

}
