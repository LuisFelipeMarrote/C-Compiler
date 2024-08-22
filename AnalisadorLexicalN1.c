#include <stdio.h> 
#include <stdlib.h> 

  

char caractere;     
char *filename; 
FILE *fp; 

  
void ler()
{ 
    caractere = fgetc(fp); 
} 
    
int main() 
{ 
    filename = "new.txt";
    fp = fopen(filename, "r");

    if (fp == NULL) 
    { 
        printf("Error: could not open file %s", filename); 
        return 1; 
    } 
    ler(); 

    while (caractere != EOF) 
    {
        while((caractere == '{' || caractere == ' ' || caractere == '\n') && caractere != EOF) 
        {
            if(caractere == '{') 
            { 
                while(caractere != '}' && caractere != EOF) 
                { 
                    ler(); 
                }
                ler(); 
            }                 

            while((caractere == ' ' || caractere == '\n') && caractere != EOF) 
            { 
                ler(); 
            } 
        } 

        if(caractere != EOF) 
        { 
            printf("%c", caractere);
            ler();
            //pega_token(); 
            //insere_lista();             
        }
    } 

    // close the file 
    fclose(fp); 
    return 0; 

} 