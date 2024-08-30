#include <stdlib.h> 
#include <stdio.h> 
#include <string.h> 
#include <ctype.h> 

#define lexema_size_max 30

enum tipos {smais, smenos, smul, snum, satribuicao, sdoispontos,svirgula, 
            sabre_parenteses, sfecha_parenteses, sponto_virgula, sprograma, sse, sentao,
            ssenao, senquanto, sfaca, sinício, sfim, sescreva, sleia, svar, sinteiro, sbooleano, 
            sverdadeiro, sfalso, sprocedimento, sfuncao, sdiv, se, sou, snao, sidentificador, 
            sdif, smenorig, smenor, smaiorig, smaior};  

char* print_enum(enum tipos simbolo){
    char* tipo[] = {"smais", "smenos", "smul", "snum", "satribuicao", "sdoispontos", "svirgula",
            "sabre_parenteses", "sfecha_parenteses", "sponto_virgula", "sprograma", "sse", "sentao",
            "ssenao", "senquanto", "sfaca", "sinício", "sfim", "sescreva", "sleia", "svar", "sinteiro", "sbooleano", 
            "sverdadeiro", "sfalso", "sprocedimento", "sfuncao", "sdiv", "se", "sou", "snao", "sidentificador",
            "sdif", "smenorig", "smenor", "smaiorig", "smaior"}; 
    return tipo[simbolo];       
}

typedef struct{ 
  char lexema[lexema_size_max];          // ver ponteiro / string 
  enum tipos simbolo; 
}token;
