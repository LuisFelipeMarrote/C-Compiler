#include <stdlib.h> 
#include <stdio.h> 
#include <string.h> 
#include <ctype.h> 


enum {
  smais, smenos, smul;
};

struct token {
  int lexema;
  char simbol;
};
