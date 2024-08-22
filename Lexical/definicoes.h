#include <stdlib.h> 
#include <stdio.h> 
#include <string.h> 
#include <ctype.h> 

enum {smais, smenos, smul};  

typedef struct{ 
  int lexema; 
  char simbolo; 
}token; 