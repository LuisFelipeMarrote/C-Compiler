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

/*
Precedência (maior para menor):
    Aritméticos: (+ positivo, - negativo) (*,div) (+,-)
    Relacionais: (todos iguais)
    Lógicos: (não) (e) (ou) 
*/

//falta pos/neg
int prioridade(enum tipos tipo){
    //rever positivo e negativo (talvez lidar com eles fora daqui) - ou colocar um sinal
    switch(tipo){
        case se:
        case sou:
        case snao:
            return(1);
        case sdif:
        case smaior:
        case smenor:
        case smenorig:
        case smaiorig:
        case sig:
            return(2);
        case smais:
        case smenos:
            return(3);
        case smult:
        case sdiv:
            return(4);
        default: 
            return(0);
    }
}

int checa_operador(enum tipos tipo){
    switch(tipo){
        case se:
        case sou:
        case snao:
        case sdif:
        case smaior:
        case smenor:
        case smenorig:
        case smaiorig:
        case sig:
        case smais:
        case smenos:
        case smult:
        case sdiv:
            return(1);
        default: 
            return(0);
    }
}

//recebe expressao como lista de tokens (esquerda = início) e transforma em lista pos-fixa (reordena a lista) e retorna o início
node_lista_token* converte_inf_posfix(node_lista_token* lista_infix){
    node_lista_token* retorno_pos = NULL; // endereço de retorno (início da lista posfixa)
    node_lista_token* lista_pos = NULL; // ponteiro para ultimo elemento adicionado na lista posfixa
    node_lista_token* pilha = NULL; // topo da pilha
    int flag_primeiro_empilha = 0; // flag para inicializar ponteiro pilha

    while(lista_infix != NULL){
        if(lista_infix->tk.simbolo == snúmero || lista_infix->tk.simbolo == sidentificador){ //Se for variável ou número copia na saída;     
            if(lista_pos != NULL){ // se não for a primeira vez, muda o ponteiro prox e atualiza ponteiro lista
                lista_pos->prox = lista_infix;
                lista_pos = lista_pos->prox;
            }else{ //se for a primeira vez, só aponta para token e seta o ponteiro que vai ser retornado
                lista_pos = lista_infix;
                retorno_pos = lista_infix;
            }
            lista_infix = lista_infix->prox;
        }else if(lista_infix->tk.simbolo == sabre_parenteses){
            //Se for abre-parênteses empilha; 
            if(pilha != NULL){//se a pilha ja existir
                node_lista_token* temp = lista_infix;
                lista_infix = lista_infix->prox;
                temp->prox = pilha;
                pilha = temp;
            }else{ // se for a primeira vez empilhando, aponta para o token
                pilha = lista_infix;
                lista_infix = lista_infix->prox;
                pilha->prox = NULL;
            }

        }else if(lista_infix->tk.simbolo == sfecha_parenteses){     
            //Se for fecha-parênteses, desempilha tudo copiando na saída até o primeiro abre-parênteses
            while (pilha->tk.simbolo != sabre_parenteses){
                lista_pos->prox = pilha;
                lista_pos = lista_pos->prox;
                pilha = pilha->prox;
            }
            
            //desempilha abre parentesis
            pilha = pilha->prox;

            lista_infix = lista_infix->prox;
        }else if(checa_operador(lista_infix->tk.simbolo)){
            /*Se for operador, empilha após percorrer desempilhando e copiando na saída todos os operadores com precedência maior ou igual ao 
            que será empilhado, isso até encontrar o primeiro operador com precedência menor, terminar a pilha ou encontrar o primeiro abreparênteses*/
            while(pilha != NULL && pilha->tk.simbolo != sabre_parenteses && (prioridade(lista_infix->tk.simbolo) <= prioridade(pilha->tk.simbolo))){
                lista_pos->prox = pilha;
                lista_pos = lista_pos->prox;
                pilha = pilha->prox;
            }

            //empilha
            if(pilha != NULL){//se a pilha ja existir
                node_lista_token* temp = lista_infix;
                lista_infix = lista_infix->prox;
                temp->prox = pilha;
                pilha = temp;
            }else{ // se for a primeira vez empilhando, aponta para o token
                pilha = lista_infix;
                lista_infix = lista_infix->prox;
                pilha->prox = NULL;
            }
        }
    }

    //Se terminar a expressão, desempilhar copiando na saída todos os operadores ainda existentes na pilha
    lista_pos->prox = pilha;
    return retorno_pos;
}
