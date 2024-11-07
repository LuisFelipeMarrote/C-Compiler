#include "lexico.h"
#include "sintatico.h"
#include "simbolos.h"

node_lista_token* novo_no_token(token tk) {
    node_lista_token* novo_no = (node_lista_token*)malloc(sizeof(node_lista_token));
    if (novo_no) {
        novo_no->tk = tk;
        novo_no->prox = NULL;
    }
    return novo_no;
}

node_lista_token* adicionar_token(node_lista_token* lista, token tk) {
    node_lista_token* novo_no = novo_no_token(tk);
    if (!lista) {
        return novo_no;
    }
    node_lista_token* atual = lista;
    while (atual->prox != NULL) {
        atual = atual->prox;
    }
    atual->prox = novo_no;
    return lista;
}

int main(){
    FILE *fp; 
    char* filename = "teste_posfix.txt";
    //char* filename = "new.txt";
    token main_tk;
    int linha_main = 1;
    fp = fopen(filename, "r");
    caractere = fgetc(fp); 
    //AnalisadorSintatico(fp,&linha_main,&main_tk);
    //AnalisadorLexical(fp,&linha_main,&tk);
    //print_token(&main_tk);
    //print_linhas();
    
    /*nova_tabela();

    
    insere_tab_simbolos("sla", 'x', snull, ' ');
    for(int i = 0; i < 5; i++){
        insere_tab_simbolos("entrada provisoria", 'x', i, ' ');
        printf("Nome: %s\t", tabela->nome_ident);
        printf("Tipo: %s\t", print_enum(tabela->tipo));
        printf("Escopo: %c\n", tabela->escopo);
    }
    insere_tab_simbolos("sla2", 'x', snull, ' ');
    insere_tab_simbolos("oi", 'x', snull, ' ');
    
    printf("===========================================================\n");
    for(int i = 0; i< 7; i++){
        printf("Nome: %s\t", tabela->nome_ident);
        printf("Tipo: %s\t", print_enum(tabela->tipo));
        printf("Escopo: %c\n", tabela->escopo);

        pop_tab_simbolos();
    }*/
   
    //teste conversao polonesa inversa:
    
    node_lista_token* lista = NULL;
    while (!feof(fp)) {

        AnalisadorLexical(fp, &linha_main, &main_tk);
        lista = adicionar_token(lista, main_tk);
    }

    lista = converte_inf_posfix(lista);

    node_lista_token* atual = lista;
    int i = 1;
    while (atual != NULL) {
        //printf("\ntoken %d: ", i);
        printf("%s", atual->tk.lexema);
        i++;
        atual = atual->prox;
    }

    // Liberação de memória (opcional)
    atual = lista;
    while (atual != NULL) {
        node_lista_token* temp = atual;
        atual = atual->prox;
        free(temp);
    }

}