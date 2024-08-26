#include <stdlib.h> 
#include <stdio.h> 
#include <string.h> 
#include <ctype.h> 


enum tipos {smais, smenos, smul, snum, satribuicao, sdoispontos,svirgula, 
            sabre_parenteses, sfecha_parenteses, sponto_virgula, sprograma, sse, sentao,
            ssenao, senquanto, sfaca, sinício, sfim, sescreva, sleia, svar, sinteiro, sbooleano, 
            sverdadeiro, sfalso, sprocedimento, sfuncao, sdiv, se, sou, snao, sidentificador};  

typedef struct{ 
  char* lexema;          // ver ponteiro / string 
  enum tipos simbolo; 
}token;
