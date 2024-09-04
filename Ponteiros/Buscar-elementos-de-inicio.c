#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void preencherA(int *, int, int, int);
int buscar (int *, int, int);

int main () {
    int n, elemento, posicao;
    srand(time(NULL)); 

    printf("Tamanho do vetor: ");
    scanf("%d", &n);

    int vetor[n];

    preencherA(vetor, n, 1, 5);

    for(int i = 0; i < n; i++){
        printf("%d\n", vetor[i]);
    }

    printf("Qual elemento deseja buscar: ");
    scanf("%d", &elemento);

    posicao = buscar (vetor, n, elemento);
    if (posicao == -1){
        printf("Elemento nao encontrado no array.\n\n");
    }
    else{
        printf ("Esse elemento aparece pela primeira vez na posicao %d do vetor.\n\n", posicao);
    }


    return 0;
}

void preencherA (int vetor[], int n, int min, int max){
    if (n == 0){
        return;
    }
    vetor[n-1] = min + (rand()%(max-min+1)); 
    preencherA(vetor, n-1, min, max);
}

int buscar (int vetor[], int n, int elemento){
    if (n == 0){
        return (-1);
    }
    if (vetor[n-1] == elemento){
        return (n-1);
    }
    return buscar (vetor, n-1, elemento);
}