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

void converte_inf_posfix(){
    //Percorre expressão in-fixa da esquerda para direita, fazendo:
    //Se for variável ou número copia na saída;     
    //Se for abre-parênteses empilha;
    //Se for fecha-parênteses, desempilha tudo copiando na saída até o primeiro abre-parênteses (desempilhando-o);
    /*Se for operador, empilha após percorrer desempilhando e copiando na saída todos os operadores com precedência maior ou igual ao que será empilhado, isso até encontrar o primeiro operador com precedência menor, terminar a pilha ou encontrar o primeiro abreparênteses*/
    //Se terminar a expressão, desempilhar copiando na saída todos os operadores ainda existentes na pilha

}
