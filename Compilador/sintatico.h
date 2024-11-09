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
char str_aux[4] = "";
int num_var = 0;
node_lista_token* expressao_infix;

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
void Analisa_atribuicao(token ident);
void Chamada_procedimento();
void Analisa_termo();
void Analisa_Chamada_de_Procedimento();
void Analisa_declaracao_procedimento();
void Analisa_declaracao_funcao();
void Analisa_expressao_simples();
void Analisa_expressao();
void Analisa_fator();
void Analisa_chamada_funcao();
enum tipos analisa_tipo_expressao_semantica();
void AnalisadorSintatico(FILE *fp_main, int *linha_main, token *token);
<<<<<<< HEAD
=======
void Cria_arquivo();
void Gera(char rotulo[4], char instrucao[8], char atr1[4], char atr2[4]);

// essa função vou implementar dps, mas ela é so para formatar
// void Seta_string(char str[], int size){
//     char ajuda[size];
//     memset(ajuda,' ', size);
//     str[0] = 'a';
// }

void Gera(char rotulo[4], char instrucao[8], char atr1[4], char atr2[4]){
    //redeclara isso, pois ele ta passando como string literal, ou seja, so daa para ler
    //entao eu so preciso declarar variaveis aux e copiar a informaçao paraa laa
    char linha[20]; //ver como inicializa a lista vazia 
    //Seta_string(rotulo, 4);
    strcat(linha, rotulo);
    //Seta_string(instrucao, sizeof(instrucao));
    strcat(linha, instrucao);
    if (strcmp(atr1, "    ") != 0)
    {
        //Seta_string(atr1, sizeof(atr1));
        strcat(linha, atr1);
    }
    else if (strcmp(atr2, "    ") != 0)
    {
        //Seta_string(atr2, sizeof(atr2));
        strcat(linha, atr2);
    }
    strcat(linha, "\n");

    fputs(linha, new_fp);
    
}

void Cria_arquivo(){

    strcat(nome_arquivo, tk->lexema);
    strcat(nome_arquivo, ".txt");

    new_fp = fopen(nome_arquivo, "r");

    if (new_fp != NULL)
    {
        remove(new_fp);
        printf("O arquivo '%s' já existe e foi bÃÃÃnidu.\n", nome_arquivo);
    }

    new_fp = fopen(nome_arquivo, "w");

    // Verifica se o arquivo foi aberto com sucesso
    if (new_fp == NULL) {
        printf("Erro ao criar o arquivo.\n");
    }

    printf("Arquivo criado e texto escrito com sucesso.\n");

}
>>>>>>> 78300458d8ec2e8baab856c49ad310e4ae407d4c

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
    token temp = *tk;
    AnalisadorLexical(fp,linha,tk);
    if(tk->simbolo == satribuicao){
        Analisa_atribuicao(temp);
    }else{
        Chamada_procedimento(tk);
    }
}

/// tabela
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
                    Gera("    ","RD","    ","    ");
                    //retorno da tabela de simbolos
                    //Gera("    ","STR",rotulo_tab,"    ");          
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
            if(Pesquisa_duplicvar_tabela(tk->lexema)){
                //se nao encontrou duplicidade
                insere_tab_simbolos(tk->lexema, svar, '-', "-");
                AnalisadorLexical(fp,linha,tk);
                if(tk->simbolo == svírgula || tk->simbolo == sdoispontos){
                    if(tk->simbolo == svírgula){
                        AnalisadorLexical(fp,linha,tk);
                        if(tk->simbolo == sdoispontos){
                            sintax_error(7);
                        }
                    }
                }else{
                    sintax_error(0);
                }
            }else{
                //se encontrou duplicidade
                semantic_error(0);
            }
        }
    }while(tk->simbolo != sdoispontos);
    AnalisadorLexical(fp,linha,tk);
    Analisa_Tipo(tk);
}

///def auxrot1,auxrot2 inteiro
/// gera código
void Analisa_enquanto(){
    int auxrot1 = rotulo;
    Gera(itoa(auxrot1,str_aux,10),"NULL","    ","    ");
    rotulo = rotulo+1;
    AnalisadorLexical(fp,linha,tk);
    if(analisa_tipo_expressao_semantica() != sbooleano){
        semantic_error(0);
    }
    if(tk->simbolo == sfaca){
        int auxrot2 = rotulo;
        Gera("    ","JMPF",itoa(auxrot2,str_aux,10),"    ");
        rotulo = rotulo+1;
        AnalisadorLexical(fp,linha,tk);
        Analisa_comando_simples(tk);
        Gera("    ","JMP",itoa(auxrot1,str_aux,10),"    ");
        Gera(itoa(auxrot2,str_aux,10),"NULL","    ","    ");
    }else{
        sintax_error(20);
    }
}

/// gera codigo
void Analisa_declaracao_procedimento(){
    AnalisadorLexical(fp,linha,tk);
    char nivel = 'L'; //(marca ou novo galho)
    if(tk->simbolo == sidentificador){
        if(pesquisa_declfunc_tabela){ ///CONFIRMAR SE PODE USAR A MSM pesquisa_declfunc Q VARIAVEL
            insere_tab_simbolos(tk->lexema,sprocedimento,nivel, " "/*rótulo*/);
            /*{guarda na TabSimb}
            Gera(rotulo,NULL,´ ´,´ ´)
            {CALL irá buscar este rótulo na TabSimb}
            rotulo:= rotulo+1*/
            AnalisadorLexical(fp,linha,tk);
            if(tk->simbolo == sponto_virgula){
                Analisa_Bloco(tk);
            }else{
                sintax_error(22);
            }
        }else{
            semantic_error(0);
        }
    }else{
        sintax_error(21);
    }
    ///DESEMPILHA OU VOLTA NÍVEL
    volta_nivel();
}

void Analisa_declaracao_funcao(){
    AnalisadorLexical(fp,linha,tk);
    char nivel = 'L'; //(marca ou novo galho)
    if(tk->simbolo == sidentificador){
        if(pesquisa_declfunc_tabela(tk->lexema)){
            //nao encontrou
            insere_tab_simbolos(tk->lexema,snull,nivel," "/*rótulo*/);
            AnalisadorLexical(fp,linha,tk);
            if(tk->simbolo == sdoispontos){
                AnalisadorLexical(fp,linha,tk);
                if(tk->simbolo == sinteiro || tk->simbolo == sbooleano){
                    if(tk->simbolo == sinteiro){
                        coloca_tipo_tabela(tk->lexema, fint);
                    }else{
                        coloca_tipo_tabela(tk->lexema, fbool);
                    }
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
        }else{
            semantic_error(0); //função ja declarada
        }
    }else{
        sintax_error(23);
    }
    ///DESEMPILHA OU VOLTA NÍVEL
    volta_nivel();
}

void Analisa_se(){

    AnalisadorLexical(fp,linha,tk);
    if(analisa_tipo_expressao_semantica() != sbooleano){
        semantic_error(0);
    }

    int auxrot1;
    auxrot1 = rotulo;
    Gera("    ","JMPF",itoa(auxrot1,str_aux,10),"    ");
    rotulo = rotulo+1;

    if(tk->simbolo == sentao){

        AnalisadorLexical(fp,linha,tk);
        Analisa_comando_simples(tk);    
    
        int auxrot2;   
        auxrot2 = rotulo;

        if(tk->simbolo == ssenao){ //remendo para ele no colocar um jmp caso nao tenha um ssenao         
            Gera("    ","JMP",itoa(auxrot2,str_aux,10),"    ");
        }

        rotulo = rotulo+1;    
        Gera(itoa(auxrot1,str_aux,10),"NULL","    ","    ");

        if(tk->simbolo == ssenao){
            AnalisadorLexical(fp,linha,tk);
            Analisa_comando_simples(tk);            
    
            Gera(itoa(auxrot2,str_aux,10),"NULL","    ","    ");
        }
    }else{
        sintax_error(27);
    }
}

/// pos fixa
void Analisa_expressao(){
    Analisa_expressao_simples(tk);
    if(tk->simbolo == smaior || tk->simbolo == smaiorig || tk->simbolo == sig || tk->simbolo == smenor || tk->simbolo == smenorig || tk->simbolo == sdif){
        AnalisadorLexical(fp,linha,tk);
        expressao_infix = adicionar_token(expressao_infix, *tk);
        Analisa_expressao_simples(tk);
    }
}

void Analisa_expressao_simples(){
    if(tk->simbolo == smais || tk->simbolo == smenos){
        //esse smais e smenos é pos/neg
        AnalisadorLexical(fp,linha,tk);
        expressao_infix = adicionar_token(expressao_infix, *tk);
    }
    Analisa_termo(tk);
    while(tk->simbolo == smais || tk->simbolo == smenos || tk->simbolo == sou){
        AnalisadorLexical(fp,linha,tk);
        expressao_infix = adicionar_token(expressao_infix, *tk);
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
                AnalisadorLexical(fp,linha,tk);
                expressao_infix = adicionar_token(expressao_infix, *tk);
            }
        }else{
            semantic_error(0); // ident nao esta na tabela (var nao declarada)
        }
        //AnalisadorLexical(fp,linha,tk);
    }else if(tk->simbolo == snúmero){
        AnalisadorLexical(fp,linha,tk);
        expressao_infix = adicionar_token(expressao_infix, *tk);
    }else if(tk->simbolo == snao){
        AnalisadorLexical(fp,linha,tk);
        expressao_infix = adicionar_token(expressao_infix, *tk);
        Analisa_fator(tk);
    }else if(tk->simbolo == sabre_parenteses){
        AnalisadorLexical(fp,linha,tk);
        expressao_infix = adicionar_token(expressao_infix, *tk);
        Analisa_expressao(tk);
        if(tk->simbolo == sfecha_parenteses){
            AnalisadorLexical(fp,linha,tk);
            expressao_infix = adicionar_token(expressao_infix, *tk); //conferindo se precisa dessa
        }else{
            sintax_error(28);
        }
    }else if(strcmp(tk->lexema, "verdadeiro") || strcmp(tk->lexema, "falso")){
        AnalisadorLexical(fp,linha,tk);
        expressao_infix = adicionar_token(expressao_infix, *tk);
    }else{
        sintax_error(29);
    }
}

void Analisa_Tipo(){
    if(tk->simbolo != sinteiro && tk->simbolo != sbooleano){
        sintax_error(8);
    }else{
        coloca_tipo_tabela(tk->lexema, tk->simbolo);
    }
    AnalisadorLexical(fp,linha,tk);
}

/// tabela
void Analisa_escreva(){
    AnalisadorLexical(fp,linha,tk);
    if(tk->simbolo == sabre_parenteses){
        AnalisadorLexical(fp,linha,tk);
        if(tk->simbolo == sidentificador){
            AnalisadorLexical(fp,linha,tk);
            ///if(pesquisa_declvarfunc_tabela(token.lexema))
                if(tk->simbolo == sfecha_parenteses){                    
                    //retorno da tabela de simbolos pela busca do token.lexema
                    //Gera("    ","LDV",rotulo_tab,"    ");   
                    Gera("    ","PRN","    ","    ");
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

/// gera codigo
void Analisa_subrotinas(){
    int flag = 0;        
    int auxrot = rotulo;
    if(tk->simbolo == sprocedimento && tk->simbolo == sfuncao) ///tk->simbolo = sprocedimento && tk->simbolo = sfuncao
    {
        Gera("    ", "JMP",itoa(rotulo,str_aux,10),"");
        rotulo = rotulo + 1;
        flag = 1;
    }

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
    if (flag == 1){
        Gera(itoa(auxrot,str_aux,10),"NULL","    ","    ");
    }
}

void Analisa_termo(){
    Analisa_fator(tk);
    while((tk->simbolo == smult) | (tk->simbolo == sdiv) | (tk->simbolo == se)){
        AnalisadorLexical(fp,linha,tk);
        expressao_infix = adicionar_token(expressao_infix, *tk);
        Analisa_fator(tk);
    }
}

/// ?
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

/// ? rever inteiro - comecei a corrigir sem perceber que nem a base tinha!
void Analisa_atribuicao(token ident){
    AnalisadorLexical(fp,linha,tk);
    entrada_tab_simbolos* var = busca_ident(ident.lexema);
    enum tipos tipo = analisa_tipo_expressao_semantica();
    if(var->tipo != tipo){
        semantic_error(0); //atribuição com tipo diferente
    }

}

/// ?
void Chamada_procedimento(){
    //ver o que isso deve fazer
}

void Analisa_chamada_funcao(){
    printf("Ainda nao implementei analisa chamada de função!");
    /*Lembrete: adicionar o seguinte apos qualquer chamada lexical (se envolver o analisa_expressão);
        expressao_infix = adicionar_token(expressao_infix, *tk);*/
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
        Cria_arquivo(tk);
        Gera("    ", "START", "    ",  "    ");
        AnalisadorLexical(fp,linha,tk);
        if(tk->simbolo == sidentificador){
            insere_tab_simbolos(tk->lexema, snomeprog, '-', "-");
            AnalisadorLexical(fp,linha,tk);
            if(tk->simbolo == sponto_virgula){
                Analisa_Bloco(&tk);
                if(tk->simbolo == sponto){
                    AnalisadorLexical(fp,linha,tk);
                    if(feof(fp)){
                        Gera("    ","HLT","    ","    ");
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

enum tipos analisa_tipo_expressao_semantica(){
    expressao_infix = adicionar_token(expressao_infix, *tk);
    Analisa_expressao(tk);
    expressao_infix = converte_inf_posfix(expressao_infix);
    enum tipos tipo = semantico_expressao(expressao_infix);
    expressao_infix = NULL;
    return tipo;
}