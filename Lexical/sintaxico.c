#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "definicoes.h"
#include "funcs_iniciais.c"

void sintax_error(int n); 
void Analisa_Bloco(token *tk); 
void Analisa_et_variáveis(token *tk); 
void Analisa_subrotinas();  
void Analisa_comandos(token *tk); 
void Analisa_Variaveis(token *tk); 
void Analisa_Tipo(token *tk); 
void Analisa_comando_simples(token *tk); 
void Analisa_atrib_chprocedimento(token *tk); 
void Analisa_se(token *tk);
void Analisa_enquanto(token *tk);
void Analisa_leia(token *tk);
void Analisa_escreva(token *tk);
void Analisa_atribuicao(token *tk);
void Chamada_procedimento(token *tk);


void AnalisadorSintatico();

void sintax_error(int n){
    //rever todos os rotulos de erro abaixo (placeholders)
    char* erros[] = {"",
        "1: (tk inicial != sprograma)",
        "2: (esperado sidentificador)",
        "3: (faltou ponto e virgula)", 
        "4: (faltou ponto final)",
        "5: (honestamente nn entendi ainda, rever o que é)",
        "6: (faltou ';' em et_variaveis)",
        "7: (honestamente nn entendi ainda, rever o que é)"
        "8: Declaração de variável com tipo inválido",
        "9: (falta sinicio)",
        "10: (sla)",
        "11: (sla)",
        "12: (fecha parentesis)"
        };
    
    printf("Erro de sintaxe");
    printf(" %s ", erros[n]);
    printf("na linha %d\n", linha);
}

void Analisa_Bloco(token *tk){
    //tk = lexico
    Analisa_et_variáveis(tk);
    Analisa_subrotinas();
    Analisa_comandos(tk);
}

void Analisa_comandos(token *tk){
    if(tk->simbolo == sinício){
        //tk = lexico
        Analisa_comando_simples(tk);
        while(tk->simbolo != sfim){
            if(tk->simbolo == sponto_virgula){
                //tk = lexico
                if(tk->simbolo != sfim)
                    Analisa_comando_simples(tk);
            }
        }
        //tk = lexico
    }else{
        sintax_error(9);
    }
}

void Analisa_comando_simples(token *tk){
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

void Analisa_atrib_chprocedimento(token *tk){
    //tk = lexico
    if(tk->simbolo == satribuicao){
        Analisa_atribuicao(tk);
    }else{
        Chamada_procedimento(tk);
    }
}

void Analisa_leia(token *tk){
    //tk = lexico
    if(tk->simbolo == sabre_parenteses){
        //tk = lexico
        if(tk->simbolo == sidentificador){
            ///se pesquisa_declvar_tabela(tk.lexema)
            {
                ///(pesquisa em toda a tabela)
                //tk = lexico
                if(tk->simbolo = sfecha_parenteses){
                    //tk = lexico
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



void Analisa_et_variáveis(token *tk){
    if(tk->simbolo == sidentificador){
        while(tk->simbolo == sidentificador){
            Analisa_Variaveis(tk);
            if(tk->simbolo == sponto_virgula){
                // tk = lexico (ver escopo / ponteiro, etc)
            }else{
                sintax_error(6);
            }
        }
    }
}

void Analisa_Variaveis(token *tk){
    do{
        if(tk->simbolo == sidentificador){
            ///Pesquisa_duplicvar_tabela(tk->lexema)
            ///se nao encontrou duplicidade
            ///entao inicio
            {
                ///insere_tabela(token->lexema, "variavel", "", "")
                //tk = lexico
                if(tk->simbolo == svírgula || tk->simbolo == sdoispontos){
                    if(tk->simbolo == svírgula){
                        //tk = lexico
                        if(tk->simbolo == sdoispontos){
                            sintax_error(7);
                        }
                    }
                }
            }///senao ERRO
        }
    }while(tk->simbolo == sdoispontos);
    //tk = lexico();
    Analisa_Tipo(tk);
}

/// def rótulo inteiro
void AnalisadorSintatico(){
    ///rotulo := 1
    token tk; //tk = lexico
    if(tk.simbolo == sprograma){
        //tk = lexico
        if(tk.simbolo == sidentificador){
            ///insere_tabela(tk.lexema, "nomedeprograma", "", "");
            //tk = lexico
            if(tk.simbolo == sponto_virgula){
                Analisa_Bloco(&tk);
                if(tk.simbolo == sponto){
                    if(1/*acabou o arquivo ou é comentário*/){
                        //sucesso (return something?)
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

void Analisa_Tipo(token *tk){
    if(tk->simbolo != sinteiro && tk->simbolo != sbooleano){
        sintax_error(8);
    }///else{ coloca_tipo_tabela}
    //tk = lexico
}

int main(){
    AnalisadorSintatico();
}