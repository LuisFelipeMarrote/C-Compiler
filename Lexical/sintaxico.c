#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "definicoes.h"
#include "funcs_iniciais.c"

void sintax_error(int n);
void Analisa_Bloco();
void Analisa_et_variáveis();
void Analisa_subrotinas();
void Analisa_comandos();

void AnalisadorSintatico();

void sintax_error(int n){
    //rever todos os rotulos de erro abaixo (placeholders)
    char* erros[] = {"",
        "1: (tk inicial != sprograma)",
        "2: (esperado sidentificador)",
        "3: (faltou ponto e virgula)", 
        "4: (faltou ponto final)",
        "5: (honestamente nn entendi ainda, rever o que é)"
        };
    
    printf("Erro de sintaxe");
    printf(" %s ", erros[n]);
    printf("na linha %d\n", linha);
}

void Analisa_Bloco(){
    //tk = lexico
    Analisa_et_variáveis();
    Analisa_subrotinas();
    Analisa_comandos();
}

// def rótulo inteiro
void AnalisadorSintatico(){
    //rotulo := 1
    token tk; //tk = lexico
    if(tk.simbolo == sprograma){
        //tk = lexico
        if(tk.simbolo == sidentificador){
            //insere_tabela(tk.lexema, "nomedeprograma", "", "");
            //tk = lexico
            if(tk.simbolo == sponto_virgula){
                Analisa_Bloco();
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

int main(){
    AnalisadorSintatico();
}