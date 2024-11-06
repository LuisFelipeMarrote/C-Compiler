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

//recebe expressao como lista de tokens (esquerda = início) e transforma em lista pos-fixa (reordena a lista) e retorna o início
node_lista_token* converte_inf_posfix(node_lista_token* lista_infix){
    node_lista_token* retorno_pos; // endereço de retorno (início da lista posfixa)
    node_lista_token* lista_pos; // ponteiro para ultimo elemento adicionado na lista posfixa
    node_lista_token* pilha; // topo da pilha
    int flag_primeira_var = 0; // flag para inicializar ponteiro lista_pos
    int flag_primeiro_empilha = 0; // flag para inicializar ponteiro pilha

    while(lista_infix != NULL){
        if(lista_infix->tk.simbolo == snúmero || lista_infix->tk.simbolo == sidentificador){ //Se for variável ou número copia na saída; 
            if(flag_primeira_var){ // se não for a primeira vez, muda o ponteiro prox e atualiza ponteiro lista
                lista_pos->prox = lista_infix;
                lista_pos = lista_pos->prox;
            }else{ //se for a primeira vez, só aponta para token, muda a flag e seta o ponteiro que vai ser retornado
                lista_pos = lista_infix;
                retorno_pos = lista_infix;
                flag_primeira_var = 1;
            }
            lista_infix = lista_infix->prox;
        }else if(lista_infix->tk.simbolo == sabre_parenteses){
            //Se for abre-parênteses empilha; 
            if(flag_primeiro_empilha){//se a pilha ja existir
                node_lista_token* temp = lista_infix;
                lista_infix = lista_infix->prox;
                temp->prox = pilha;
                pilha = temp;
            }else{ // se for a primeira vez empilhando, aponta para o token e muda flag
                pilha = lista_infix;
                pilha->prox = NULL;
                flag_primeiro_empilha = 1;
                lista_infix = lista_infix->prox;
            }
        }else if(lista_infix->tk.simbolo == sfecha_parenteses){     
            //Se for fecha-parênteses, desempilha tudo copiando na saída até o primeiro abre-parênteses
            while (pilha->tk.simbolo != sabre_parenteses){
                lista_pos->prox = pilha;
                lista_pos = lista_pos->prox;
                pilha = pilha->prox;
            }
            
            //desempilha abre parentesis
            if(pilha->prox == NULL){
                flag_primeiro_empilha = 0;
            }else{
                pilha = pilha->prox;
            }

            lista_infix = lista_infix->prox;
        }else if(0/*operadores*/){
            /*Se for operador, empilha após percorrer desempilhando e copiando na saída todos os operadores com precedência maior ou igual ao 
            que será empilhado, isso até encontrar o primeiro operador com precedência menor, terminar a pilha ou encontrar o primeiro abreparênteses*/
            if(flag_primeiro_empilha){//se a pilha ja existir
                //percorre a pilha desempilhando coisas com mais prioridade
                

                //empilha
                node_lista_token* temp = lista_infix;
                lista_infix = lista_infix->prox;
                temp->prox = pilha;
                pilha = temp;
            }else{ // se for a primeira vez empilhando, aponta para o token e muda flag
                pilha = lista_infix;
                pilha->prox = NULL;
                flag_primeiro_empilha = 1;
                lista_infix = lista_infix->prox;
            }
        }
    }

    //Se terminar a expressão, desempilhar copiando na saída todos os operadores ainda existentes na pilha
    lista_pos->prox = pilha;
    return retorno_pos;
}
