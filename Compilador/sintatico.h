// problema com o enquanto: jmpf faz a chamada de um rotulo que nao foi declarado
// problema com variáveis repetidas: caso exista uma variavel repetida ele tem que usar a variavel declarada mais internamente.
// criar uma lista que correlaciona uma variavel com um lexema e um nivel, ele procura a primeira variavel com esse lexema

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "definicoes.h"
#include "lexico.h"
#include "simbolos.h"
#include "semantico.h"
#include "geracao.h"

FILE *fp; 
token *tk;
int *linha;
int rotulo;
char str_aux[5] = "";
int num_var_total = 1;
node_lista_token* expressao_infix;

//void sintax_error(int n); 
void Analisa_Bloco(); 
void Analisa_et_variáveis(); 
void Analisa_subrotinas();  
void Analisa_comandos(); 
void Analisa_Variaveis(); 
void Analisa_Tipo(); 
void Analisa_comando_simples(); 
void Analisa_atrib_chprocedimento(); 
void Analisa_se();
void Analisa_enquanto();
void Analisa_leia(); 
void Analisa_escreva();
void Analisa_atribuicao(token ident);
void Chamada_procedimento();
void Analisa_termo();
void Analisa_declaracao_procedimento();
void Analisa_declaracao_funcao();
void Analisa_expressao_simples();
void Analisa_expressao();
void Analisa_fator();
void Analisa_chamada_funcao();
enum tipos analisa_tipo_expressao_semantica();
void AnalisadorSintatico(FILE *fp_main, int *linha_main, token *token);

/*void sintax_error(int n){
    ///rever todos os rotulos de erro abaixo (placeholders)
    char* erros[] = {"falta definir",
        "1: Esperado 'programa'",
        "2: Esperado identificador para escrever",
        "3: Esperado ';'", 
        "4: Esperado ponto final",
        "5: Encontrado ponto final antes do fim do arquivo",
        "6: Esperado ';' após declaração de variáveis",
        "7: Esperado um identificador para 'var'",
        "8: Declaração de variável com tipo inválido",
        "9: Esperado inicio",
        "10: Esperado '(' após 'leia'",
        "11: Esperado identificador para leitura",
        "12: Esperado ')' para 'leia'",
        "13: Esperado '(' após 'escreva'",
        "14: Esperado ')' para 'escreva'",
        "15: Esperado identificador para escrita",
        "16: Esperado identificador de processo",
        "17: Esperado '(' após processo",
        "18: Esperado ')' após processo",
        "19: Esperado ';' após comando",
        "20: Esperado 'faca' após enquanto",
        "21: Esperado identificador para declaração de procedimento",
        "22: Esperado ';' após a declaração de procedimento",
        "23: Esperado identificador para declaração de função",
        "24: Esperado ':' para declaração de função",
        "25: Tipo inválido para a função",
        "26: Esperado ';' após a declaração de função",
        "27: Esperado 'entao' após 'se'",
        "28: Esperado ')'",
        "29: Encontrado simbolo inesperado para um fator",
        "30: Esperado ';' após subrotina",
        };
    
    printf("Erro na linha %d: %s", *linha);
    printf(" %s ", erros[n]);
    printf("(Código de erro - Sint%d) \n", n);

}
*/
void Analisa_Bloco(){
    AnalisadorLexical(fp,linha,tk);
    Analisa_et_variáveis(tk);
    Analisa_subrotinas(tk);
    Analisa_comandos(tk);
    //tenho que colocar o dalloc aqui
}

void Analisa_comandos(){
    if(tk->simbolo == sinício){
        AnalisadorLexical(fp,linha,tk);
        Analisa_comando_simples(tk);
        while(tk->simbolo != sfim){
            if(tk->simbolo == sponto_virgula){
                AnalisadorLexical(fp,linha,tk);
                if(tk->simbolo != sfim)
                    Analisa_comando_simples(tk);
            }else{
                sintax_error(19, *linha);       
            }
        }
        AnalisadorLexical(fp,linha,tk);
    }else{
        sintax_error(9, *linha);
    }
}

void Analisa_comando_simples(){
    if(tk->simbolo == sidentificador){
        Analisa_atrib_chprocedimento(tk);
    }else if(tk->simbolo ==sse){
        Analisa_se(tk);
    }else if(tk->simbolo == senquanto){
        Analisa_enquanto(tk);
    }else if(tk->simbolo == sleia){
        Analisa_leia(tk);
    }else if(tk->simbolo == sescreva){
        Analisa_escreva(tk);
    }else{
        Analisa_comandos(tk);
    }
}

void Analisa_atrib_chprocedimento(){
    token temp = *tk;    
    token *temp1 = tk;
    AnalisadorLexical(fp,linha,tk);
    if(tk->simbolo == satribuicao){
        Analisa_atribuicao(temp); 
    }else{
        char lex = tk->lexema[0];
        ungetc(lex, fp);
        Chamada_procedimento(temp);
    }
}

void Analisa_leia(){
    AnalisadorLexical(fp,linha,tk);
    if(tk->simbolo == sabre_parenteses){
        AnalisadorLexical(fp,linha,tk);
        if(tk->simbolo == sidentificador){
            if(pesquisa_declvar_tabela(*tk)){
                entrada_tab_simbolos* tab_simb = busca_ident(tk->lexema);
                AnalisadorLexical(fp,linha,tk);
                if(tk->simbolo == sfecha_parenteses){
                    AnalisadorLexical(fp,linha,tk);
                    Gera_leia(tab_simb->rotulo);         
                } else{
                    sintax_error(12, *linha);
                }
            }else{
                semantic_error(1, *linha);
            }
        }else{
            sintax_error(11, *linha);
        }
    }else{
        sintax_error(10, *linha);
    }
}

void Analisa_et_variáveis(){
    if(tk->simbolo == svar){
        AnalisadorLexical(fp,linha,tk); 
        if(tk->simbolo == sidentificador){
            while(tk->simbolo == sidentificador){
                Analisa_Variaveis(tk);
                if(tk->simbolo == sponto_virgula){
                    AnalisadorLexical(fp,linha,tk); 
                }else{
                    sintax_error(6, *linha);
                }
            }
        }
    }
}

void Analisa_Variaveis(){        
    int num_var = 0;
    do{
        if(tk->simbolo == sidentificador){
            if(Pesquisa_duplicvar_tabela(tk->lexema)){
                //se nao encontrou duplicidade
                insere_tab_simbolos(tk->lexema, svar, '-', str_rotulo_var(num_var + num_var_total));
                num_var++;
                AnalisadorLexical(fp,linha,tk);
                if(tk->simbolo == svírgula || tk->simbolo == sdoispontos){
                    if(tk->simbolo == svírgula){
                        AnalisadorLexical(fp,linha,tk);
                        if(tk->simbolo == sdoispontos){
                            sintax_error(7, *linha);
                        }
                    }
                }else{
                    sintax_error(0, *linha);
                }
            }else{
                //se encontrou duplicidade
                semantic_error(2, *linha);
            }
        }
    }while(tk->simbolo != sdoispontos);
    AnalisadorLexical(fp,linha,tk);
    Analisa_Tipo(tk);
    Gera_alloc(num_var);
    num_var_total = num_var_total + num_var;
}

void Analisa_enquanto(){
    int auxrot1 = rotulo;
    Gera_rotulo(str_rotulo(auxrot1));
    insere_tab_simbolos(tk->lexema,senquanto,'-',str_rotulo(auxrot1));
    rotulo = rotulo+1;
    AnalisadorLexical(fp,linha,tk);
    if(analisa_tipo_expressao_semantica() == sbooleano){
        if(tk->simbolo == sfaca){
            
            int auxrot2 = rotulo;
            Gera_jmpf(str_rotulo(auxrot2));
            rotulo = rotulo+1;

            AnalisadorLexical(fp,linha,tk);
            Analisa_comando_simples(tk);

            Gera_jmp(str_rotulo(auxrot1));
            Gera_rotulo(str_rotulo(auxrot2));
        }else{
            sintax_error(20, *linha);
        }
    }else{
        semantic_error(3, *linha);
    }
}

/// gera codigo
void Analisa_declaracao_procedimento(){
    AnalisadorLexical(fp,linha,tk);
    char nivel = 'L'; //(marca ou novo galho)
    if(tk->simbolo == sidentificador){
        if(pesquisa_declfunc_tabela(tk->lexema)){
            insere_tab_simbolos(tk->lexema,sprocedimento, nivel, str_rotulo(rotulo));
            /*{guarda na TabSimb}
            {CALL irá buscar este rótulo na TabSimb}*/
            Gera_rotulo(str_rotulo(rotulo));            
            rotulo++;
            AnalisadorLexical(fp,linha,tk);
            if(tk->simbolo == sponto_virgula){
                Analisa_Bloco(tk);
                // função para rodar a tab de simbolos e retornar quantas variaveis tem até a marca.
                // Gera_dalloc(num_var)
                Gera_return();
            }else{
                sintax_error(22, *linha);
            }
        }else{
            semantic_error(4, *linha);
        }
    }else{
        sintax_error(21, *linha);
    }
    volta_nivel();
}

void Analisa_declaracao_funcao(){
    AnalisadorLexical(fp,linha,tk);
    char nivel = 'L'; //(marca ou novo galho)
    if(tk->simbolo == sidentificador){
        if(pesquisa_declfunc_tabela(tk->lexema)){
            //nao encontrou
            token nome_func = *tk;
            insere_tab_simbolos(tk->lexema,sfuncao,nivel,str_rotulo(rotulo));
            Gera_rotulo(str_rotulo(rotulo));            
            rotulo++;
            AnalisadorLexical(fp,linha,tk);
            if(tk->simbolo == sdoispontos){
                AnalisadorLexical(fp,linha,tk);
                if(tk->simbolo == sinteiro || tk->simbolo == sbooleano){
                    if(tk->simbolo == sinteiro){
                        coloca_tipo_tabela(nome_func.lexema, fint);
                    }else{
                        coloca_tipo_tabela(nome_func.lexema, fbool);
                    }
                    AnalisadorLexical(fp,linha,tk);
                    if(tk->simbolo == sponto_virgula){
                        Analisa_Bloco(tk);
                        // função para rodar a tab de simbolos e retornar quantas variaveis tem até a marca.
                        // Gera_dalloc(num_var)
                        Gera_return();
                    }else{
                        sintax_error(26, *linha);
                    }
                }else{
                    sintax_error(25, *linha);
                }
            }else{
                sintax_error(24, *linha);
            }
        }else{
            semantic_error(5, *linha); //função ja declarada
        }
    }else{
        sintax_error(23, *linha);
    }
    volta_nivel();
}

void Analisa_se(){
    AnalisadorLexical(fp,linha,tk);
    if(analisa_tipo_expressao_semantica() == sbooleano){    
        int auxrot1;
        auxrot1 = rotulo;
        rotulo = rotulo+1;
        Gera_jmpf(str_rotulo(auxrot1));

        if(tk->simbolo == sentao){

            AnalisadorLexical(fp,linha,tk);
            Analisa_comando_simples(tk);    
        
            int auxrot2;   
            auxrot2 = rotulo;
            rotulo = rotulo+1;  

            if(tk->simbolo == ssenao){ //remendo para ele no colocar um jmp caso nao tenha um ssenao         
                Gera_jmp(str_rotulo(auxrot2));
            }

            Gera_rotulo(str_rotulo(auxrot1));

            if(tk->simbolo == ssenao){
                AnalisadorLexical(fp,linha,tk);
                Analisa_comando_simples(tk);      
                Gera_rotulo(str_rotulo(auxrot2));
            }
        }else{
            sintax_error(27, *linha);
        }
    }else{
        semantic_error(6, *linha);
    }
}

void Analisa_expressao(){
    Analisa_expressao_simples(tk);
    if(tk->simbolo == smaior || tk->simbolo == smaiorig || tk->simbolo == sig || tk->simbolo == smenor || tk->simbolo == smenorig || tk->simbolo == sdif){
        expressao_infix = adicionar_token(expressao_infix, *tk);
        AnalisadorLexical(fp,linha,tk);
        Analisa_expressao_simples(tk);
    }
}

void Analisa_expressao_simples(){
    if(tk->simbolo == smais || tk->simbolo == smenos){
        tk->simbolo = ssinalu;
        strcat(tk->lexema, "u");
        expressao_infix = adicionar_token(expressao_infix, *tk);
        AnalisadorLexical(fp,linha,tk);
    }
    Analisa_termo(tk);
    while(tk->simbolo == smais || tk->simbolo == smenos || tk->simbolo == sou){
        expressao_infix = adicionar_token(expressao_infix, *tk);
        AnalisadorLexical(fp,linha,tk);
        Analisa_termo(tk);
    }
}

void Analisa_fator(){
    if(tk->simbolo == sidentificador){
        entrada_tab_simbolos* tab_simb = busca_ident(tk->lexema);
        if(tab_simb != NULL){
            if(tab_simb->tipo == fint || tab_simb->tipo == fbool){
                Analisa_chamada_funcao();
            }else{
                expressao_infix = adicionar_token(expressao_infix, *tk);
                AnalisadorLexical(fp,linha,tk);
            }
        }else{
            semantic_error(7, *linha); // ident nao esta na tabela (var nao declarada)
        }
        //AnalisadorLexical(fp,linha,tk);
    }else if(tk->simbolo == snúmero){
        tk->simbolo = sinteiro;
        expressao_infix = adicionar_token(expressao_infix, *tk);
        AnalisadorLexical(fp,linha,tk);
    }else if(tk->simbolo == snao){
        expressao_infix = adicionar_token(expressao_infix, *tk);
        AnalisadorLexical(fp,linha,tk);
        Analisa_fator(tk);
    }else if(tk->simbolo == sabre_parenteses){
        expressao_infix = adicionar_token(expressao_infix, *tk);
        AnalisadorLexical(fp,linha,tk);
        Analisa_expressao(tk);
        if(tk->simbolo == sfecha_parenteses){
            expressao_infix = adicionar_token(expressao_infix, *tk); 
            AnalisadorLexical(fp,linha,tk);
        }else{
            sintax_error(28, *linha);
        }
    }else if(tk->simbolo == sverdadeiro || tk->simbolo == sfalso){
        tk->simbolo = sbooleano;
        expressao_infix = adicionar_token(expressao_infix, *tk);
        AnalisadorLexical(fp,linha,tk);
    }else{
        sintax_error(29, *linha);
    }
}

void Analisa_Tipo(){
    if(tk->simbolo != sinteiro && tk->simbolo != sbooleano){
        sintax_error(8, *linha);
    }else{
        coloca_tipo_tabela("_var", tk->simbolo);
    }
    AnalisadorLexical(fp,linha,tk);
}

void Analisa_escreva(){
    token *ajuda = tk;
    AnalisadorLexical(fp,linha,tk);
    if(tk->simbolo == sabre_parenteses){
        AnalisadorLexical(fp,linha,tk);
        if(tk->simbolo == sidentificador){
            if(pesquisa_declvarfunc_tabela(tk->lexema) == 0){            
                entrada_tab_simbolos* tab_simb = busca_ident(tk->lexema);
                AnalisadorLexical(fp,linha,tk);
                if(tk->simbolo == sfecha_parenteses){                    
                    Gera_escreva(tab_simb->rotulo);
                    AnalisadorLexical(fp,linha,tk);
                }
                else{
                    sintax_error(14, *linha);
                }
            }else{
                semantic_error(8, *linha);
            }
        }
        else{
            sintax_error(15, *linha);
            }
    }
    else{
        sintax_error(13, *linha);
    }
}

void Analisa_subrotinas(){
    int flag = 0;        
    int auxrot = rotulo;
    token *th = tk;
    if(tk->simbolo == sprocedimento || tk->simbolo == sfuncao)
    {
        Gera_jmp(str_rotulo(auxrot));
        rotulo = rotulo + 1;
        flag = 1;
    }

    while((tk->simbolo == sprocedimento) || (tk->simbolo == sfuncao)){
        if(tk->simbolo == sprocedimento){
            Analisa_declaracao_procedimento(tk);
        }else{
            Analisa_declaracao_funcao();
        }
        if(tk->simbolo == sponto_virgula){
            AnalisadorLexical(fp,linha,tk);
        }else{
            sintax_error(30, *linha);
        }
    }
    if (flag == 1){
        Gera_rotulo(str_rotulo(auxrot));
    }
}

void Analisa_termo(){
    Analisa_fator(tk);
    while((tk->simbolo == smult) | (tk->simbolo == sdiv) | (tk->simbolo == se)){
        expressao_infix = adicionar_token(expressao_infix, *tk);
        AnalisadorLexical(fp,linha,tk);
        Analisa_fator(tk);
    }
}

/// @brief analisa atribuições, verificando tipagem (analise semantica) e diferenciando atribuicao de variavel e retorno de funcao
/// @param ident identificador lido antes do sinal de atribuição (:=) 
void Analisa_atribuicao(token ident){
    AnalisadorLexical(fp,linha,tk);
    entrada_tab_simbolos* destino = busca_ident(ident.lexema);
    if(destino != NULL){
        enum tipos tipo = analisa_tipo_expressao_semantica();
        if(destino->tipo == fint || destino->tipo == fbool){
            //retorno de funcao
            entrada_tab_simbolos* func_atual = func_proc_atual();
            if(func_atual != NULL && strcmp(func_atual->nome_ident, destino->nome_ident) == 0){
                if(destino->tipo == fint){
                    if(tipo != sinteiro){
                        semantic_error(9, *linha);
                    }
                }else{
                    if(tipo != sbooleano){
                        semantic_error(10, *linha);
                    }
                }
            }else{
                semantic_error(21, *linha);
            }
        }else if(destino->tipo == tipo){
            Gera_str(destino->rotulo);
        }else{
            semantic_error(11, *linha); //atribuição com tipo diferente
        }
    }else{
        semantic_error(12, *linha); //identificador não declarado
    }
}

void Chamada_procedimento(token ident){
    entrada_tab_simbolos* proc = busca_ident(ident.lexema);
    if(proc != NULL){
        if(proc->tipo == sprocedimento){
            Gera_call(proc->rotulo);
            AnalisadorLexical(fp,linha,tk);
        }else{
            semantic_error(13, *linha);
        }
    }else{
        semantic_error(14, *linha);
    }

}

void Analisa_chamada_funcao(){
    Gera_call(str_rotulo(rotulo));
    Gera_load_variavel(str_rotulo_var(0));
    rotulo++;
    expressao_infix = adicionar_token(expressao_infix, *tk);
    AnalisadorLexical(fp,linha,tk);
}

enum tipos analisa_tipo_expressao_semantica(){
    Analisa_expressao(tk);
    if(expressao_infix != NULL){
        expressao_infix = converte_inf_posfix(expressao_infix);
        enum tipos tipo = semantico_expressao(expressao_infix, *linha);
        expressao_infix = NULL;
        return tipo;
    }else{
        semantic_error(15, *linha); // nao tenho ctz se chega aqui (acho que o sintatico para antes), mas esperado expressao
        return serro;
    }
}

/// def rótulo inteiro
void AnalisadorSintatico(FILE *fp_main, int *linha_main, token *token_main){
    nova_tabela(); // inicializa a tabela de simbolos
    expressao_infix = NULL; // inicializa end de memoria para analise de expressoes
    ///rotulo := 1
    fp = fp_main;
    tk = token_main;
    linha = linha_main;

    AnalisadorLexical(fp,linha,tk);
    if(tk->simbolo == sprograma){
        AnalisadorLexical(fp,linha,tk);
        if(tk->simbolo == sidentificador){        
            Cria_arquivo(tk);
            Gera_start_programn();
            insere_tab_simbolos(tk->lexema, snomeprog, '-', "-");
            AnalisadorLexical(fp,linha,tk);
            if(tk->simbolo == sponto_virgula){
                Analisa_Bloco(&tk);
                if(tk->simbolo == sponto){
                    AnalisadorLexical(fp,linha,tk);
                    if(feof(fp)){
                        Gera_end_programn();
                        printf("SUCESSO!\n");
                    }else{
                        sintax_error(5, *linha);
                    }
                }else{
                    sintax_error(4, *linha);
                }
            }else{
                sintax_error(3, *linha);
            }
        }else{
            sintax_error(2, *linha);
        }
    }
    else{
        sintax_error(1, *linha);
    }
}

void limpa_memoria(){
    //libera tabela
    deleta_tabela();
    
    //libera expressao 
    while(expressao_infix!=NULL){
        node_lista_token* temp = expressao_infix;
        expressao_infix = expressao_infix->prox;
        free(temp);
    }
}
