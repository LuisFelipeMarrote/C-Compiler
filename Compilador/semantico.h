#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "definicoes.h"
#include "simbolos.h"
#include "geracao.h"

//empilha primeiro token de uma lista
void empilha_token(node_lista_token** pilha, node_lista_token** lista){
    if(*pilha != NULL){//se a pilha ja existir
        node_lista_token* temp = *lista;
        *lista = (*lista)->prox;
        temp->prox = *pilha;
        *pilha = temp;
    }else{ // se for a primeira vez empilhando, aponta para o token
        *pilha = *lista;
        *lista = (*lista)->prox;
        (*pilha)->prox = NULL;
    }
}

int prioridade(enum tipos tipo){
    switch(tipo){
        case sou:
            return(1);
        case se:
            return(2);
        case snao:
            return(3);
        case sdif:
        case smaior:
        case smenor:
        case smenorig:
        case smaiorig:
        case sig:
            return(4);
        case smais:
        case smenos:
            return(5);
        case smult:
        case sdiv:
            return(6);
        case ssinalu:
            return(7);
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
        case ssinalu:
            return(1);
        default: 
            return(0);
    }
}

//struct com dados de cada operação (tipos de operadores e resultados e numero de operadores)
typedef struct operacao operacao;
typedef struct operacao{
    int qtd_operadores;
    enum tipos tipos_operadores;
    enum tipos tipo_resultado;
}operacao;

//recebe um sinal e retorna o formato da operacao
operacao formato_operacao(enum tipos tipo){
    operacao retorno;
    switch(tipo){
        case smult:
        case sdiv:
        case smais:
        case smenos:
            retorno.qtd_operadores = 2;
            retorno.tipo_resultado = sinteiro;
            retorno.tipos_operadores = sinteiro;
            break;
        case sdif:
        case smaior:
        case smenor:
        case smenorig:
        case smaiorig:
        case sig:
            retorno.qtd_operadores = 2;
            retorno.tipos_operadores = sinteiro;
            retorno.tipo_resultado = sbooleano;
            break;
        case se:
        case sou:
            retorno.qtd_operadores = 2;
            retorno.tipos_operadores = sbooleano;
            retorno.tipo_resultado = sbooleano;
            break;
        case snao:
            retorno.qtd_operadores = 1;
            retorno.tipos_operadores = sbooleano;
            retorno.tipo_resultado = sbooleano;
            break;
        case ssinalu:
            retorno.qtd_operadores = 1;
            retorno.tipos_operadores = sinteiro;
            retorno.tipo_resultado = sinteiro;
    }
    return(retorno);
}

//recebe expressao como lista de tokens (esquerda = início) e transforma em lista pos-fixa (reordena a lista) e retorna o início
node_lista_token* converte_inf_posfix(node_lista_token* lista_infix){
    node_lista_token* retorno_pos = NULL; // endereço de retorno (início da lista posfixa)
    node_lista_token* lista_pos = NULL; // ponteiro para ultimo elemento adicionado na lista posfixa
    node_lista_token* pilha = NULL; // topo da pilha

    while(lista_infix != NULL){
        if(lista_infix->tk.simbolo == sinteiro || lista_infix->tk.simbolo == sidentificador || lista_infix->tk.simbolo == sbooleano){ //Se for ident(var ou func) ou número, copia na saída;     
            if(lista_pos != NULL){ // se não for a primeira vez, muda o ponteiro prox e atualiza ponteiro lista
                lista_pos->prox = lista_infix;
                lista_pos = lista_pos->prox;
            }else{ //se for a primeira vez, só aponta para token e seta o ponteiro que vai ser retornado
                lista_pos = lista_infix;
                retorno_pos = lista_infix;
            }
            lista_infix = lista_infix->prox;
        }else if(lista_infix->tk.simbolo == sabre_parenteses){ //Se for abre-parênteses empilha; 
            empilha_token(&pilha, &lista_infix);
        }else if(lista_infix->tk.simbolo == sfecha_parenteses){ //Se for fecha-parênteses, desempilha tudo copiando na saída até o primeiro abre-parênteses
            while (pilha->tk.simbolo != sabre_parenteses){
                lista_pos->prox = pilha;
                lista_pos = lista_pos->prox;
                pilha = pilha->prox;
            }
            
            //desempilha abre parentesis
            node_lista_token* temp_free = pilha;
            pilha = pilha->prox;
            free(temp_free);

            if(pilha == NULL){
                lista_pos->prox = NULL;
            }

            temp_free = lista_infix;
            lista_infix = lista_infix->prox;
            free(temp_free);

        }else if(checa_operador(lista_infix->tk.simbolo)){
            /*Se for operador, empilha após percorrer desempilhando e copiando na saída todos os operadores com precedência maior ou igual ao 
            que será empilhado, isso até encontrar o primeiro operador com precedência menor, terminar a pilha ou encontrar o primeiro abreparênteses*/
            while(pilha != NULL && pilha->tk.simbolo != sabre_parenteses && (prioridade(lista_infix->tk.simbolo) <= prioridade(pilha->tk.simbolo))){
                lista_pos->prox = pilha;
                lista_pos = lista_pos->prox;
                pilha = pilha->prox;
            }

            //empilha
            empilha_token(&pilha, &lista_infix);
        }
    }

    //Se terminar a expressão, desempilhar copiando na saída todos os operadores ainda existentes na pilha
    if(pilha != NULL){
        lista_pos->prox = pilha;
    }

    return retorno_pos;
}

//recebe expressao pos-fixa e faz a analise semantica retornando seu tipo (sinteiro / sbooleano) - destroi a lista no processo
enum tipos semantico_expressao(node_lista_token** lista_posfix, int linha){
    // percorre a lista empilhando os valores, e desempilhando conforme encontra operadores
    node_lista_token* pilha = NULL;

    while(*lista_posfix != NULL){
        if((*lista_posfix)->tk.simbolo == sinteiro || (*lista_posfix)->tk.simbolo == sidentificador || (*lista_posfix)->tk.simbolo == sbooleano){ //se for variável, empilha
            if((*lista_posfix)->tk.simbolo == sidentificador){
                printf("oi1");
                //procurar tabela
                entrada_tab_simbolos* entrada_tabela_operador;
                entrada_tabela_operador = busca_ident((*lista_posfix)->tk.lexema);
                if(entrada_tabela_operador == NULL){
                    printf("oi12");
                    while(pilha!=NULL){
                        node_lista_token* liberar = pilha;
                        pilha = pilha->prox;
                        free(liberar);
                    }// libera a pilha
                    semantic_error(17, linha);
                    return serro;
                }
                if(entrada_tabela_operador->tipo == fint){
                    (*lista_posfix)->tk.simbolo = sinteiro;
                    printf("oi2");
                }else if(entrada_tabela_operador->tipo == fbool){
                    (*lista_posfix)->tk.simbolo = sbooleano;
                    printf("oi3");
                }else{
                    Gera_load_variavel(entrada_tabela_operador->rotulo);
                    (*lista_posfix)->tk.simbolo = entrada_tabela_operador->tipo;
                    printf("oi4");
                }
            } else {
                if((*lista_posfix)->tk.simbolo == sbooleano){
                    if(strcmp((*lista_posfix)->tk.lexema, "verdadeiro")){
                        memset((*lista_posfix)->tk.lexema, '\0',30);
                        strcpy((*lista_posfix)->tk.lexema, "0");
                    }else{
                        memset((*lista_posfix)->tk.lexema, '\0',30);
                        strcpy((*lista_posfix)->tk.lexema, "1");
                    }
                }
                Gera_load_const((*lista_posfix)->tk.lexema);                    
            }
            empilha_token(&pilha, lista_posfix);
        
        }else if(checa_operador((*lista_posfix)->tk.simbolo)){ //se for operador, ver quantos desempilhar, e checa se os tipos estão corretos
            operacao analisando = formato_operacao((*lista_posfix)->tk.simbolo);
            for(int i = 0; i<analisando.qtd_operadores; i++){
                if(pilha->tk.simbolo != analisando.tipos_operadores){
                    char pilha_lexema[lexema_size_max] = "";
                    enum tipos pilha_tipo = pilha->tk.simbolo;
                    strcpy(pilha_lexema, pilha->tk.lexema);
            
                    while(pilha!=NULL){
                        node_lista_token* liberar = pilha;
                        pilha = pilha->prox;
                        free(liberar);
                    }// libera a pilha


                    if(pilha_tipo == sbooleano){
                        if (strcmp(pilha_lexema, "0   ") == 0){
                            semantico18(linha, "falso", print_tipo_erros(pilha_tipo), print_tipo_erros(analisando.tipos_operadores));
                        }else{
                            semantico18(linha, "verdadeiro", print_tipo_erros(pilha_tipo), print_tipo_erros(analisando.tipos_operadores));  
                        }
                    }else{
                        semantico18(linha, pilha_lexema, print_tipo_erros(pilha_tipo), print_tipo_erros(analisando.tipos_operadores));
                    }
                    return serro;   
                }else{
                    node_lista_token* temp_free = pilha;
                    pilha = pilha->prox;
                    free(temp_free);
                }
            } 

            //empilha resultado
            token temp_token;
            temp_token.simbolo = analisando.tipo_resultado;
            node_lista_token* temp_node = novo_no_token(temp_token);
            
            temp_node->prox = pilha;
            pilha = temp_node; 

            //tira o operador da lista
            temp_node = *lista_posfix;
            *lista_posfix = (*lista_posfix)->prox;
            Gera_operador(temp_node->tk.simbolo);
            free(temp_node);
        }
    }
    if(pilha->prox != NULL){
        while(pilha!=NULL){
            node_lista_token* liberar = pilha;
            pilha = pilha->prox;
            free(liberar);
        }// libera a pilha
        semantic_error(19, linha);
        return serro;
    }

    return (pilha->tk.simbolo);
}
