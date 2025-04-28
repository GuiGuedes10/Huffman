#include <stdio.h>
#include "huffman.h"

int main()
{
    printf("\n\nSeja bem vindo ao nosso programa !");
    printf("\nNele usaremos Huffman para compactar seus arquivos");
    
    int erro = 0;
    do{
        char UserFile[51];
        printf("\n\nDigite o nome do arquivo que deseja compactar:");
        scanf("%50s", UserFile);
        
        FILE *ArqEnt = fopen(UserFile, "rbt");
        
        if(ArqEnt == NULL){
            printf("\nErro! Arquivo: %s não encontrado, tente novamente", UserFile);
            erro = 1;
        }
        
        else{
            huffman(ArqEnt, UserFile);
            fclose(ArqEnt);
        }
        
    }while(erro == 1);
}
