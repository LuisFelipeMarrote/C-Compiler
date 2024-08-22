#include <stdio.h> 
#include <stdlib.h> 

char caractere;     
char *filename; 
FILE *fp; 



void ler()
{ 
    caractere = fgetc(fp); 
} 

// void trata_pontuacao(token *tk){ 
//     tk->lexema = caractere;
//     switch (caractere)
//     {
//     case ',':
//         tk->simbolo = svirgula;
//         break;
//     case '(':
//         tk->simbolo = sabre_parenteses;
//         break;
//     case ')':
//         tk->simbolo = sfecha_parenteses;
//         break;
//     case ';':
//         tk->simbolo = sponto_virgula;
//         break;    
//     case '.':
//         tk->simbolo = sfim;
//         break;    
//     default:
//         break;
//     }
// } 

void AnalisadorLexicalN1()
{
    filename = "new.txt";
    fp = fopen(filename, "r");

    if (fp == NULL) 
    { 
        printf("Error: could not open file %s", filename); 
        return; 
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
    fclose(fp); 
}
    
int main() 
{ 
    AnalisadorLexicalN1();
    // close the file 
    return 0; 

} 