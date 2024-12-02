#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>

// Error types
typedef enum {
    ERROR_LEXICAL,
    ERROR_SYNTAX,
    ERROR_SEMANTIC
} ErrorType;

// Error message structure
typedef struct {
    const char* message;
    int code;
} ErrorMessage;

// Define error messages for each type
static const ErrorMessage lexical_errors[] = {
    {"", 0},
    {"Comentário não foi encerrado corretamente - Chaves Abertas ('{') sem chaves fechadas depois (não foi encontrado '}' no código)", 1},
    {"Chaves Fechadas ('}') sem chaves abertas antes (era esperado '{' antes no código para iniciar comentário)", 2},
    {"Caractere Inválido", 3},
    {"Foi encontrado um caractere de Exclamação ('!') sem Sinal de Igual ('=') depois (símbolo inválido)", 4}
};

static const ErrorMessage sintax_errors[] = {
    {"falta definir", 0},
    {"O código não começa com 'programa'", 1},
    {"O código deve ter um nome para o programa após 'programa'", 2},
    {"Não foi encontrado ';' após declaração de nome do programa", 3},
    {"Não foi encontrado '.' ao final do código", 4},
    {"O código deve terminar no '.' (não pode ter mais código após o ponto)", 5},
    {"Não foi encontrado ';' após declaração de variáveis", 6},
    {"O código deveria ter um nome de variável entre ',' e ':' (remova ',' caso não queira adicionar mais variaveis)", 7},
    {"Declaração de variável com tipo inválido (deve ser booleano ou inteiro)", 8},
    {"Não foi encontrado 'Inicio' antes dos comandos", 9},
    {"Não foi encontrado '(' após 'leia'", 10},
    {"Esperado identificador para leitura", 11},
    {"Parentesis aberto não é fechado em 'leia('", 12},
    {"Não foi encontrado '(' após 'escreva'", 13},
    {"Parentesis aberto não é fechado em 'escreva('", 14},
    {"Esperado identificador para escrita", 15},
    {"Esperado identificador de processo", 16},
    {"Esperado '(' após processo", 17},
    {"Esperado ')' após processo", 18},
    {"Não foi encontrado ';' após um comando", 19},
    {"Não foi encontrado 'faca' após a condição do comando 'enquanto'", 20},
    {"Esperado identificador para declaração de procedimento (seu procedimento precisa ter nome)", 21},
    {"Não foi encontrado ';' após a declaração de procedimento", 22},
    {"Esperado identificador para declaração de função (sua função precisa ter nome)", 23},
    {"Não foi encontrado ':' para declaração de tipo da função (formato esperado: 'nome_da_funcao : tipo_da_funcao;')", 24},
    {"Tipo inválido para a função (deve ser 'inteiro' ou 'booleano')", 25},
    {"Não foi encontrado ';' após a declaração de função", 26},
    {"Esperado 'entao' após a condição do comando 'se'", 27},
    {"Parêntesis não foi fechado corretamente (era esperado ')')", 28},
    {"Foi Encontrado um simbolo inesperado dentro da expressao", 29},
    {"Era Esperado ';' após o 'fim' de uma subrotina (procedimento ou função)", 30},
    {"Era esperado um símbolo separando as palavras na declaração (se quiser declarar várias variáveis, separe-as por vírgulas. Se quiser colocar o tipo, coloque ':' antes do tipo escolhido)", 31}
};

static const ErrorMessage semantic_errors[] = {
    {"falta definir", 0},
    {"A variável não foi declarada antes de 'leia(variavel)'", 1},
    {"Tentou declarar uma variável já declarada! (nomes devem ser únicos dentro de um mesmo escopo)", 2},
    {"Tipo de expressão errado após 'enquanto' (esperado uma expressão com resultado booleano)", 3},
    {"Nome do procedimento já usado em outra declaração (nomes devem ser únicos dentro de um mesmo escopo)", 4},
    {"Nome do procedimento já usado em outra declaração (nomes devem ser únicos dentro de um mesmo escopo)", 5},
    {"Tipo de expressão errado após 'se' (esperado uma expressão com resultado booleano)", 6},
    {"Tentando usar uma variável não declarada na expressão (nome não encontrado no escopo)", 7},
    {"Tentando escrever uma variável não declarada", 8},
    {"Tipo incorreto na atribuição de retorno da função (esperado inteiro e recebeu booleano)", 9},
    {"Tipo incorreto na atribuição de retorno da função (esperado booleano e recebeu inteiro)", 10},
    {"Tipo incorreto na atribuição para uma variável (o que está sendo atribuído deve ser do mesmo tipo da variável)", 11},
    {"Tentando atribuir valor para um identificador não declarado (uma atribuição deve ser feita para uma variável já declarada ou para o nome da função onde se encontra, funcionando como retorno)", 12},
    {"Tentativa falha de chamada de procedimento - o identificador chamado não é um procedimento (ao usar um identificador fora de uma expressão, o código tenta chamar este como procedimento)", 13},
    {"Tentativa falha de chamada de procedimento - o procedimento não foi declarado (nome de procedimento não encontrado no escopo)", 14},
    {"Tentando analisar uma expressão inexistente!", 15},
    {"Variável de tipo incorreto (em 'leia()', a variável deve ser do tipo inteiro)", 16},
    {"Nome não declarado encontrado durante a análise da expressão (a variável ou função deve ser declarada antes de ser utilizada)", 17},
    {"Erro na própria função (análise expressão)", 18},
    {"Expressão Incompleta (não é possível chegar a um resultado final)", 19},
    {"Tentando atribuir retorno fora da função (o nome do destino da atribuição é o nome de outra)", 20},
    {"Tentando atribuir retorno a uma função diferente da atual (declare outra variável com este nome neste escopo ou mude a atribuição)", 21}
};

// Global variable to store error message
char error_buffer[1024];

// Declare jmp_buf global para controle de fluxo
jmp_buf error_jump;



void semantico18(int line, char lexema[], char* tipo1, char* tipo2){
    snprintf(error_buffer, sizeof(error_buffer), 
            "Erro na linha %d: Tipo de operando inválido (%s) para a operação. (encontrado %s = %s. O tipo esperado é %s) [Código de erro - SEM18]\n",
            line, lexema, lexema, tipo1, tipo2);

    // Jump back to the compilation start point
    longjmp(error_jump, 1);
}

// Function to handle errors
void handle_error(ErrorType type, int code, int line) {
    const char* type_str;
    const ErrorMessage* messages;
    int messages_count;
    
    switch (type) {
        case ERROR_LEXICAL:
            type_str = "Lex";
            messages = lexical_errors;
            messages_count = sizeof(lexical_errors) / sizeof(ErrorMessage);
            break;
        case ERROR_SYNTAX:
            type_str = "Sint";
            messages = sintax_errors;
            messages_count = sizeof(sintax_errors) / sizeof(ErrorMessage);
            break;
        case ERROR_SEMANTIC:
            type_str = "Sem";
            messages = semantic_errors;
            messages_count = sizeof(semantic_errors) / sizeof(ErrorMessage);
            break;
    }

    if (code >= 0 && code < messages_count) {
        snprintf(error_buffer, sizeof(error_buffer), 
                "Erro na linha %d: %s [Código de erro - %s%d]\n",
                line, messages[code].message, type_str, code);
    } else {
        snprintf(error_buffer, sizeof(error_buffer),
                "Erro na linha %d: Erro desconhecido [Código de erro - %s%d]\n",
                line, type_str, code);
    }
    
    // Jump back to the compilation start point
    longjmp(error_jump, 1);
}

// Wrapper functions for different error types
void lexical_error(int code, int line) {
    handle_error(ERROR_LEXICAL, code, line);
}

void sintax_error(int code, int line) {
    handle_error(ERROR_SYNTAX, code, line);
}

void semantic_error(int code, int line) {
    handle_error(ERROR_SEMANTIC, code, line);
}

// Function to get the last error message
const char* get_last_error() {
    return error_buffer;
}