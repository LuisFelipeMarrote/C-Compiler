#include <definicoes.h>

void trata_relacional(token *tk){
    char switchcaractere = caractere;
    ler();
    switch(switchcaractere){
        case '!':
            if(caractere == "="){
                tk->simbolo = sdif;
                ler();
            }
            else{
                //erro
            }
            break;
        case '<':
            if(caractere == "="){
                tk->simbolo = smenorig;
                ler();
            }
            else{
                tk->simbolo = smenor;
            }
            break;
        case '>':
            if(caractere == "="){
                tk->simbolo = smaiorig;
                ler();
            }
            else{
                tk->simbolo = smaior;
            }
            break;
    }
}