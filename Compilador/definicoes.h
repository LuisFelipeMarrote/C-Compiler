#pragma once
#include <stdlib.h> 
#include <stdio.h> 
#include <string.h> 
#include <ctype.h> 
#define lexema_size_max 30

enum tipos {smais, smenos, smult, snúmero, satribuicao, sdoispontos,svírgula, 
            sabre_parenteses, sfecha_parenteses, sponto_virgula, sprograma, sse, sentao,
            ssenao, senquanto, sfaca, sinício, sfim, sescreva, sleia, svar, sinteiro, sbooleano, 
            sverdadeiro, sfalso, sprocedimento, sfuncao, sdiv, se, sou, snao, sidentificador, 
            sdif, smenorig, smenor, smaiorig, smaior, sponto, sig, serro, sbase, snull, fint, 
            fbool, snomeprog, ssinalu};  

char* print_enum(enum tipos simbolo){
    char* tipo[] = {"smais", "smenos", "smult", "snúmero", "satribuicao", "sdoispontos", "svírgula",
            "sabre_parenteses", "sfecha_parenteses", "sponto_virgula", "sprograma", "sse", "sentao",
            "ssenao", "senquanto", "sfaca", "sinício", "sfim", "sescreva", "sleia", "svar", "sinteiro", "sbooleano", 
            "sverdadeiro", "sfalso", "sprocedimento", "sfuncao", "sdiv", "se", "sou", "snao", "sidentificador",
            "sdif", "smenorig", "smenor", "smaiorig", "smaior", "sponto", "sig", "serro", "sbase", "snull", "fint", 
            "fbool", "snomeprog", "ssinalu"}; 
    return tipo[simbolo];       
}

typedef struct{ 
  char lexema[lexema_size_max]; 
  enum tipos simbolo; 
}token;

typedef struct node_lista_token node_lista_token;
typedef struct node_lista_token{
    token tk;
    node_lista_token* prox;
}node_lista_token;

//cria novo node de lista de tokens
node_lista_token* novo_no_token(token tk) {
    node_lista_token* novo_no = (node_lista_token*)malloc(sizeof(node_lista_token));
    if (novo_no) {
        novo_no->tk = tk;
        novo_no->prox = NULL;
    }
    return novo_no;
}

//adiciona um token a uma lista
node_lista_token* adicionar_token(node_lista_token* lista, token tk) {
    node_lista_token* novo_no = novo_no_token(tk);
    if (!lista) {
        return novo_no;
    }
    node_lista_token* atual = lista;
    while (atual->prox != NULL) {
        atual = atual->prox;
    }
    atual->prox = novo_no;
    return lista;
}

typedef struct entrada_tab_simbolos entrada_tab_simbolos;
struct entrada_tab_simbolos{
    char nome_ident[lexema_size_max];
    char escopo;
    enum tipos tipo;
    char rotulo[5]; //end de memoria
    entrada_tab_simbolos* prev;
};

void sintax_error(int n, int linha){
    ///rever todos os rotulos de erro abaixo (placeholders)
    char* erros[] = {"falta definir",
        "O código não começa com 'programa'",
        "O código deve ter um nome para o programa após 'programa'",
        "Não foi encontrado ';' após declaração de nome do programa", 
        "Não foi encontrado '.' ao final do código",
        "O código deve terminar no '.' (não pode ter mais código após o ponto)",
        "Não foi encontrado ';' após declaração de variáveis",
        "O código deveria ter um nome de variável entre ',' e ':' (remova ',' caso não queira adicionar mais variaveis)",
        "Declaração de variável com tipo inválido (deve ser booleano ou inteiro)",
        "Não foi encontrado 'Inicio' antes dos comandos",
        "Não foi encontrado '(' após 'leia'",
        "Esperado identificador para leitura",
        "Parentesis aberto não é fechado em 'leia('",
        "Não foi encontrado '(' após 'escreva'",
        "Parentesis aberto não é fechado em 'escreva('",
        "Esperado identificador para escrita",
        "16: Esperado identificador de processo",
        "17: Esperado '(' após processo",
        "18: Esperado ')' após processo",
        "Não foi encontrado ';' após um comando",
        "Não foi encontrado 'faca' após a condição do comando 'enquanto'",
        "Esperado identificador para declaração de procedimento (seu procedimento precisa ter nome)",
        "Não foi encontrado ';' após a declaração de procedimento",
        "Esperado identificador para declaração de função (sua função precisa ter nome)",
        "Não foi encontrado ':' para declaração de tipo da função (formato esperado: 'nome_da_funcao : tipo_da_funcao;')",
        "Tipo inválido para a função (deve ser 'inteiro' ou 'booleano')",
        "Não foi encontrado ';' após a declaração de função",
        "Esperado 'entao' após a condição do comando 'se'",
        "Parêntesis não foi fechado corretamente (era esperado ')')",
        "Foi Encontrado um simbolo inesperado dentro da expressao",
        "Era Esperado ';' após o 'fim' de uma subrotina (procedimento ou função)",
        "Era esperado um símbolo separando as palavras na declaração (se quiser declarar várias variáveis, separe-as por vírgulas. Se quiser colocar o tipo, coloque ':' antes do tipo escolhido)",        
        };
    
    printf("Erro na linha %d:", linha);
    printf(" %s ", erros[n]);
    printf("[Código de erro - Sint%d] \n", n);

    exit(1);
}

void semantic_error(int n, int linha){
    //rever todos os rotulos de erro abaixo (placeholders)
    char* erros[] = {"falta definir",
        "A variável não foi declarada antes de 'leia(variavel)'",
        "Tentou declarar uma variável já declarada! (nomes devem ser únicos dentro de um mesmo escopo)",
        "Tipo de expressão errado após 'enquanto' (esperado uma expressão com resultado booleano)",
        "Nome do procedimento já usado em outra declaração (nomes devem ser únicos dentro de um mesmo escopo)",
        "Nome do procedimento já usado em outra declaração (nomes devem ser únicos dentro de um mesmo escopo)",
        "Tipo de expressão errado após 'se' (esperado uma expressão com resultado booleano)",
        "Tentando usar uma variável não declarada na expressão (nome não encontrado no escopo)",
        "Tentando escrever uma variável não declarada",
        "Tipo incorreto na atribuição de retorno da função (esperado inteiro e recebeu booleano)",
        "Tipo incorreto na atribuição de retorno da função (esperado booleano e recebeu inteiro)",
        "Tipo incorreto na atribuição para uma variável (o que está sendo atribuído deve ser do mesmo tipo da variável)",
        "Tentando atribuir valor para um identificador não declarado (uma atribuição deve ser feita para uma variável já declarada ou para o nome da função onde se encontra, funcionando como retorno)",
        "Tentativa falha de chamada de procedimento - o identificador chamado não é um procedimento (ao usar um identificador fora de uma expressão, o código tenta chamar este como procedimento)",
        "Tentativa falha de chamada de procedimento - o procedimento não foi declarado (nome de procedimento não encontrado no escopo)",
        "{Confirmar se é possível chegar aqui sem que o sintático pare o programa antes} Tentando analisar uma expressão inexistente!",
        "Variável de tipo incorreto (em 'leia()', a variável deve ser do tipo inteiro)",
        "Nome não declarado encontrado durante a análise da expressão (a variável ou função deve ser declarada antes de ser utilizada)",
        "18 - erro na propria função (analise exppressao)",
        "Expressão Incompleta (não é possível chegar a um resultado final)",
        };
    
    printf("Erro na linha %d:", linha);
    printf(" %s ", erros[n]);
    printf("[Código de erro - Sem%d] \n", n);

    exit(1);
}

char* print_tipo_erros(enum tipos simbolo){ //alterei os tipos na lista conforme a utilização (essa função imprime algo intuitivo como sinteiro = Inteiro)
    char* tipo[] = {"smais", "smenos", "smult", "Numero", "satribuicao", "sdoispontos", "svírgula",
            "sabre_parenteses", "sfecha_parenteses", "sponto_virgula", "sprograma", "sse", "sentao",
            "ssenao", "senquanto", "sfaca", "sinício", "sfim", "sescreva", "sleia", "svar", "Inteiro", "Booleano", 
            "sverdadeiro", "sfalso", "sprocedimento", "sfuncao", "sdiv", "se", "sou", "snao", "sidentificador",
            "sdif", "smenorig", "smenor", "smaiorig", "smaior", "sponto", "sig", "serro", "sbase", "snull", "fint", 
            "fbool", "snomeprog", "ssinalu"}; 
    return tipo[simbolo];       
}