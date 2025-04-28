#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman.h"

#define MAX 256

typedef struct nodo {
    char caractere;
    int frequencia;
    struct nodo* esquerda;
    struct nodo* direita;
} nodo;

typedef struct {
    nodo* dados[MAX];
    int tamanho;
} FilaPrioridade;


typedef struct {
    char caractere;
    char* codigo;
} Resultado;


nodo* criarNo(char caractere, int frequencia) {
    nodo* novo = (nodo*)malloc(sizeof(nodo));
    novo->caractere = caractere;
    novo->frequencia = frequencia;
    novo->esquerda = NULL;
    novo->direita = NULL;
    return novo;
}

void inicializarFila(FilaPrioridade* fila) {
    fila->tamanho = 0;
}

void inserirFila(FilaPrioridade* fila, nodo* novo) {
    int i = fila->tamanho - 1;
    while (i >= 0 && fila->dados[i]->frequencia > novo->frequencia) {
        fila->dados[i + 1] = fila->dados[i];
        i--;
    }
    fila->dados[i + 1] = novo;
    fila->tamanho++;
}

nodo* removerMenor(FilaPrioridade* fila) {
    if (fila->tamanho == 0) return NULL;
    return fila->dados[--fila->tamanho];
}

void SearchCharacters(nodo *raiz, char *caminho, int profundidade, Resultado **vetor, int *tamanho) {
    if (raiz == NULL) return;

    if (raiz->esquerda == NULL && raiz->direita == NULL) {
        caminho[profundidade] = '\0';

        Resultado r;
        r.caractere = raiz->caractere;
        r.codigo = strdup(caminho);

       
        *vetor = realloc(*vetor, (*tamanho + 1) * sizeof(Resultado));
        (*vetor)[*tamanho] = r;
        (*tamanho)++;

        return;
    }

    if (raiz->esquerda != NULL) {
        caminho[profundidade] = '0';
        SearchCharacters(raiz->esquerda, caminho, profundidade + 1, vetor, tamanho);
    }

    if (raiz->direita != NULL) {
        caminho[profundidade] = '1';
        SearchCharacters(raiz->direita, caminho, profundidade + 1, vetor, tamanho);
    }
}

void CodGenerator(nodo *raiz, Resultado **vetor, int *tamanho) {
    char caminho[101];
    *tamanho = 0;
    *vetor = NULL; 
    SearchCharacters(raiz, caminho, 0, vetor, tamanho);
}



nodo* arvoreDeHuffman(FilaPrioridade* fila) {
    while (fila->tamanho > 1) {
        nodo* esquerda = removerMenor(fila);
        nodo* direita = removerMenor(fila);

        int somaFrequencia = esquerda->frequencia + direita->frequencia;
        nodo* novo = criarNo('*', somaFrequencia);

        novo->esquerda = esquerda;
        novo->direita = direita;

        inserirFila(fila, novo);
    }
    return removerMenor(fila);
}

void FileCompress(FILE* entrada, const char* nomeSaida, Resultado* vetor, int tamanho) {
    FILE* saida = fopen(nomeSaida, "wbt");
    if (saida == NULL) {
        printf("Erro ao criar arquivo de saída!\n");
        return;
    }

    
    char c;
    while ((c = fgetc(entrada)) != EOF) {
        for (int i = 0; i < tamanho; i++) {
            if (vetor[i].caractere == c) {
                fputs(vetor[i].codigo, saida);
                break;
            }
        }
    }

    fclose(saida);
}

void huffman(FILE* arquivo, char* UserFile){
    int frequencias[MAX] = {0};
    char c;
    while ((c = fgetc(arquivo)) != EOF) {
        frequencias[(unsigned char)c]++;
    }
    

    FilaPrioridade fila;
    inicializarFila(&fila);

    for (int i = 0; i < MAX; i++) {
        if (frequencias[i] > 0) {
            inserirFila(&fila, criarNo((char)i, frequencias[i]));
        }
    }

    nodo* raiz = arvoreDeHuffman(&fila);

    Resultado *vetor;
    int tamanho;
    CodGenerator(raiz, &vetor, &tamanho);

    rewind(arquivo);
    
    char FileName[101];
    strcpy(FileName, UserFile);
    strcat(FileName, "_compactado");
    
    FileCompress(arquivo, FileName, vetor, tamanho);  
    
    for (int i = 0; i < tamanho; i++) {
        free(vetor[i].codigo); 
    }
    free(vetor); 
}
