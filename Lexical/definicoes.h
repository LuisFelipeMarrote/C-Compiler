#include <stdlib.h> 
#include <stdio.h> 
#include <string.h> 
#include <ctype.h> 

enum tipos {smais, smenos, smul, snum, satribuicao, sdoispontos};  

typedef struct{ 
  char lexema;          // ver ponteiro / string 
  enum tipos simbolo; 
}token;
