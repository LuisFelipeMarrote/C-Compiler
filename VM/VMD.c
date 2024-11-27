#include "VMD.h"
int main() {
    printf("comecou\n");
    fflush(stdout);
    Pilha *p = inicializarPilha(8);
    if(p == NULL){
        return 1;
    }
    char* filename = "../Compilador/test.txt";
    FILE *file = fopen(filename, "r");
    MVD(p, file);
    fclose(file);
    liberarPilha(p);
    return 0;
}

