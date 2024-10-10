/*
    Ultimas funcoes antes dpo analisador geral.
*/

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "definicoes.h"
#include "lexico.h"
//#include "funcs_iniciais.c"

FILE *fp; 
token *tk;
int *linha;

void sintax_error(int n); 
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
void Analisa_atribuicao();
void Chamada_procedimento();
void Analisa_termo();
void Analisa_Chamada_de_Procedimento();
void Analisa_expressao();
void Analisa_declaracao_procedimento();
void Analisa_declaracao_funcao();
void Analisa_expressao();
void Analisa_expressao_simples();
void Analisa_fator();

void AnalisadorSintatico(FILE *fp_main, int *linha_main, token *token);

void sintax_error(int n){
    //rever todos os rotulos de erro abaixo (placeholders)
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
    
    printf("Erro de sintaxe");
    printf(" %s ", erros[n]);
    printf("na linha %d\n", *linha);

}

void Analisa_Bloco(){
    AnalisadorLexical(fp,linha,tk);
    Analisa_et_variáveis(tk);
    Analisa_subrotinas(tk);
    Analisa_comandos(tk);
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
                sintax_error(19);       
                AnalisadorLexical(fp,linha,tk);
            }
        }
        AnalisadorLexical(fp,linha,tk);
    }else{
        sintax_error(9);
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
    AnalisadorLexical(fp,linha,tk);
    if(tk->simbolo == satribuicao){
        Analisa_atribuicao(tk);
    }else{
        Chamada_procedimento(tk);
    }
}

void Analisa_leia(){
    AnalisadorLexical(fp,linha,tk);
    if(tk->simbolo == sabre_parenteses){
        AnalisadorLexical(fp,linha,tk);
        if(tk->simbolo == sidentificador){
            ///se pesquisa_declvar_tabela(tk->lexema)
            {
                ///(pesquisa em toda a tabela)
                AnalisadorLexical(fp,linha,tk);
                if(tk->simbolo == sfecha_parenteses){
                    AnalisadorLexical(fp,linha,tk);
                } else{
                    sintax_error(12);
                }
            }///senao erro
        }else{
            sintax_error(11);
        }
    }else{
        sintax_error(10);
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
                    sintax_error(6);
                }
            }
        }
    }
}

void Analisa_Variaveis(){
    do{
        if(tk->simbolo == sidentificador){
            ///Pesquisa_duplicvar_tabela(tk->lexema)
            ///se nao encontrou duplicidade
            ///entao inicio
            //{
                ///insere_tabela(token->lexema, "variavel", "", "")
                AnalisadorLexical(fp,linha,tk);
                if(tk->simbolo == svírgula || tk->simbolo == sdoispontos){
                    if(tk->simbolo == svírgula){
                        AnalisadorLexical(fp,linha,tk);
                        if(tk->simbolo == sdoispontos){
                            sintax_error(7);
                        }
                    }
                }
            //}///senao ERRO
        }
    }while(tk->simbolo != sdoispontos);
    AnalisadorLexical(fp,linha,tk);
    Analisa_Tipo(tk);
}

///def auxrot1,auxrot2 inteiro
void Analisa_enquanto(){
    ///
    /* auxrot1:= rotulo
    Gera(rotulo,NULL,´ ´,´ ´) {início do while}
    rotulo:= rotulo+1*/
    AnalisadorLexical(fp,linha,tk);
    Analisa_expressao(tk);
    if(tk->simbolo == sfaca){
        ///
        /*auxrot2:= rotulo
        Gera(´ ´,JMPF,rotulo,´ ´) {salta se falso}
        rotulo:= rotulo+1
        */
       AnalisadorLexical(fp,linha,tk);
       Analisa_comando_simples(tk);
       ///
       /*
        Gera(´ ´,JMP,auxrot1,´ ´) {retorna início loop}
        Gera(auxrot2,NULL,´ ´,´ ´) {fim do while}*/
    }else{
        sintax_error(20);
    }
}

void Analisa_declaração_procedimento(){
    AnalisadorLexical(fp,linha,tk);
    ///nivel := "L"(marca ou novo galho)
    if(tk->simbolo == sidentificador){
        ///
        /*pesquisa_declproc_tabela(token.lexema)
        se não encontrou
        então início
        Insere_tabela(token.lexema,”procedimento”,nível, rótulo)
        {guarda na TabSimb}
        Gera(rotulo,NULL,´ ´,´ ´)
        {CALL irá buscar este rótulo na TabSimb}
        rotulo:= rotulo+1*/
        AnalisadorLexical(fp,linha,tk);
        if(tk->simbolo == sponto_virgula){
            Analisa_Bloco(tk);
        }else{
            sintax_error(22);
        }
        ///
        /*fim
        senão ERRO
        */
    }else{
        sintax_error(21);
    }
    ///DESEMPILHA OU VOLTA NÍVEL
}

void Analisa_declaracao_funcao(){
    AnalisadorLexical(fp,linha,tk);
    ///nível := “L” (marca ou novo galho)
    if(tk->simbolo == sidentificador){
        ///
        /*pesquisa_declfunc_tabela(token.lexema)
        se não encontrou
        então início
        Insere_tabela(token.lexema,””,nível,rótulo)*/
        AnalisadorLexical(fp,linha,tk);
        if(tk->simbolo == sdoispontos){
            AnalisadorLexical(fp,linha,tk);
            if(tk->simbolo == sinteiro || tk->simbolo == sbooleano){
                ///
                /*se (token.símbolo = Sinteger)
                então TABSIMB[pc].tipo:=
                “função inteiro”
                senão TABSIMB[pc].tipo:=
                “função booleana”*/
                AnalisadorLexical(fp,linha,tk);
                if(tk->simbolo == sponto_virgula){
                    Analisa_Bloco(tk);
                }else{
                    sintax_error(26);
                }
            }else{
                sintax_error(25);
            }
        }else{
            sintax_error(24);
        }
        ///senao ERRO
    }else{
        sintax_error(23);
    }
    ///DESEMPILHA OU VOLTA NÍVEL
}

void Analisa_se(){
    AnalisadorLexical(fp,linha,tk);
    Analisa_expressao(tk);
    if(tk->simbolo == sentao){
        AnalisadorLexical(fp,linha,tk);
        Analisa_comando_simples(tk);
        if(tk->simbolo == ssenao){
            AnalisadorLexical(fp,linha,tk);
            Analisa_comando_simples(tk);
        }
    }else{
        sintax_error(27);
    }
}

void Analisa_expressao(){
    Analisa_expressao_simples(tk);
    if(tk->simbolo == smaior || tk->simbolo == smaiorig || tk->simbolo == sig || tk->simbolo == smenor || tk->simbolo == smenorig || tk->simbolo == sdif){
        AnalisadorLexical(fp,linha,tk);
        Analisa_expressao_simples(tk);
    }
}

void Analisa_expressao_simples(){
    if(tk->simbolo == smais || tk->simbolo == smenos)
        AnalisadorLexical(fp,linha,tk);
    Analisa_termo(tk);
    while(tk->simbolo == smais || tk->simbolo == smenos || tk->simbolo == sou){
        AnalisadorLexical(fp,linha,tk);
        Analisa_termo(tk);
    }
}

void Analisa_fator(){
    if(tk->simbolo == sidentificador){
        ///
        /*Se pesquisa_tabela(token.lexema,nível,ind)
            Então Se (TabSimb[ind].tipo = “função inteiro”) ou
            (TabSimb[ind].tipo = “função booleano”)
            Então Analisa_chamada_função
            Senão Léxico(token)
            Senão ERRO
        */
       AnalisadorLexical(fp,linha,tk);
    }else if(tk->simbolo == snúmero){
        AnalisadorLexical(fp,linha,tk);
    }else if(tk->simbolo == snao){
        AnalisadorLexical(fp,linha,tk);
        Analisa_fator(tk);
    }else if(tk->simbolo == sabre_parenteses){
        AnalisadorLexical(fp,linha,tk);
        Analisa_expressao(tk);
        if(tk->simbolo == sfecha_parenteses){
            AnalisadorLexical(fp,linha,tk);
        }else{
            sintax_error(28);
        }
    }else if(strcmp(tk->lexema, "verdadeiro") || strcmp(tk->lexema, "falso")){
        AnalisadorLexical(fp,linha,tk);
    }else{
        sintax_error(29);
    }
}

void Analisa_Tipo(){
    if(tk->simbolo != sinteiro && tk->simbolo != sbooleano){
        sintax_error(8);
    }///else{ coloca_tipo_tabela}
    AnalisadorLexical(fp,linha,tk);
}

void Analisa_escreva(){
    AnalisadorLexical(fp,linha,tk);
    if(tk->simbolo == sabre_parenteses){
        AnalisadorLexical(fp,linha,tk);
        if(tk->simbolo == sidentificador){
            AnalisadorLexical(fp,linha,tk);
            ///if(pesquisa_declvarfunc_tabela(token.lexema))
                if(tk->simbolo == sfecha_parenteses){
                    AnalisadorLexical(fp,linha,tk);
                }
                else{
                    sintax_error(14);
                }
            ///else(erro)
        }
        else{
            sintax_error(15);
            }
    }
    else{
        sintax_error(13);
    }
}

void Analisa_subrotinas(){
    ///def auxrot, flag inteiro
/*  flag = 0
    if(tk->simbolo = sprocedimento) | (tk->simbolo = sfuncao)
        auxrot = rotulo
        gera('        ', JMP,rotulo,'        ')     {salta sub-rotinas}
        rotulo = rotulo + 1
        flag = 1
*/
    while((tk->simbolo == sprocedimento) | (tk->simbolo == sfuncao)){
        if(tk->simbolo == sprocedimento){
            Analisa_declaracao_procedimento(tk);
        }
        else{
            Analisa_declaracao_funcao();
        }
        if(tk->simbolo == sponto_virgula){
            AnalisadorLexical(fp,linha,tk);
        }
        else{
            sintax_error(30);
        }
    }
    ///if (flag = 1){
    ///Gera(auxrot,NULL,'        ','         ')     {incio do principal}
    ///}
}

void Analisa_termo(){
    Analisa_fator(tk);
    while((tk->simbolo == smult) | (tk->simbolo == sdiv) | (tk->simbolo == se)){
        AnalisadorLexical(fp,linha,tk);
        Analisa_fator(tk);
    }
}

void Analisa_Chamada_de_Procedimento(){
    //Nao sei mas vou tentar
    if(tk->simbolo == sidentificador){
        AnalisadorLexical(fp,linha,tk);;
        if(tk->simbolo == sabre_parenteses){
            AnalisadorLexical(fp,linha,tk);;
            if(tk->simbolo != sfecha_parenteses){
                ///analisa_parametros
            }
            if(tk->simbolo == sfecha_parenteses){
                AnalisadorLexical(fp,linha,tk);
            }
            else{
                sintax_error(18);
            }
        }
        else{
            sintax_error(17);
        }
    }
    else{
        sintax_error(16);
    }
}

void Analisa_atribuicao(){
    AnalisadorLexical(fp,linha,tk);
    Analisa_expressao();
}

void Chamada_procedimento(){
    //ver o que isso deve fazer
}

void Analisa_declaracao_procedimento(){
    AnalisadorLexical(fp,linha,tk);
    if(tk->simbolo == sidentificador){
        AnalisadorLexical(fp,linha,tk);
        if(tk->simbolo == sponto_virgula){
            Analisa_Bloco();
        }else{
            sintax_error(0);
        }
    }else{
        sintax_error(0);
    }
}

/// def rótulo inteiro
void AnalisadorSintatico(FILE *fp_main, int *linha_main, token *token_main){
    ///rotulo := 1
    fp = fp_main;
    tk = token_main;
    linha = linha_main;

    AnalisadorLexical(fp,linha,tk);
    if(tk->simbolo == sprograma){
        AnalisadorLexical(fp,linha,tk);
        if(tk->simbolo == sidentificador){
            ///insere_tabela(tk->lexema, "nomedeprograma", "", "");
            AnalisadorLexical(fp,linha,tk);
            if(tk->simbolo == sponto_virgula){
                Analisa_Bloco(&tk);
                if(tk->simbolo == sponto){
                    AnalisadorLexical(fp,linha,tk);
                    if(feof(fp)/*acabou o arquivo ou é comentário*/){
                        printf("SUCESSO!\n");
                    }else{
                        sintax_error(5);
                    }
                }else{
                    sintax_error(4);
                }
            }else{
                sintax_error(3);
            }
        }else{
            sintax_error(2);
        }
    }
    else{
        sintax_error(1);
    }
}
