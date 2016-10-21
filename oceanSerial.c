/*  
 * File: oceanSerial.c
 * Ocean Approuch Serial Version
 * 
 * Author: Johnatan Oliveira ~ johnoliv[at]gmail.com
 * www.johnatan.net
 * Version 0.1
 */

#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
 
#define FALSE 0
#define TRUE 1
#define OP_OCEAN "ocean" 
#define OP_TIME "time"
#define OP_ALL "all" 
#define ERRO_OPCAO "Erro: Opcao Invalida! \n"
#define ERRO_MATRIZ "Erro: Linha e coluna devem ser maior que 1! \n"
#define DIVISOR_TIME 1000000

int qtdThreads;
int linhaTotal, linhaMatriz, colunaTotal, colunaMatriz; 
int **matriz, **matrizS;

int achaMaxVizinho(int a[]);
void ZeraMatrizes ();
void *verifMatriz();

int main(int argc, char * argv[]) {

    // inicializacao de variaveis
    int indEntValida = 0, linha = 0, coluna = 0; 
    int i = strtol(argv[1], NULL, 10);  // linhas
    int j = strtol(argv[2], NULL, 10);  // colunas
    int u = strtol(argv[3], NULL, 10);  // unidade de tempo
    int t = strtol(argv[4], NULL, 10);  // qtd de threads
    char *s = argv[5];                  // tipos de saida
    struct timeval start, end;
    float tempoExec = 0.0; 
    int fim = FALSE;
    int ciclo = 0;
    if(t==0){ t = 1; };
    if(t > i){ t = i; }; 
    if(u==0){ u = 1; }; 
    const char *op[] = { "ocean", "time", "all" };  

    // Verifico se a entrada eh valida
    if ((s == NULL)) {
        printf(ERRO_OPCAO);
        exit(EXIT_FAILURE);
    }
    int y = 0;  
    int len = sizeof(op)/sizeof(op[0]); 
    for (y=0; y < len; y++) {
        if(strstr(op[y], s)){  
            if(!strcmp(op[y],s)){ 
                indEntValida = 1;
                break;
            } 
        }
    }
    if( i < 1 || j < 1 ){
        printf(ERRO_MATRIZ);
        exit(EXIT_FAILURE);
    }

    // se a entrada de opcao for valida 
    if(indEntValida){ 

        // inicializa as matrizes com tamanho maior para nao
        // sofrer com posicoes nao encontradas...
        linhaTotal = i+2;
        linhaMatriz = i+1;
        colunaTotal = j+2;
        colunaMatriz = j+1;  

        matriz = (int **) malloc(sizeof(int *) * linhaTotal);
        for (linha = 0; linha < linhaTotal; linha++) { 
            matriz[linha] = (int *) malloc(sizeof(int) * colunaTotal);
        }  
        matrizS = (int **) malloc(sizeof(int *) * linhaTotal); 
        for (linha = 0; linha < linhaTotal; linha++) {
            matrizS[linha] = (int *) malloc(sizeof(int) * colunaTotal);
        } 
        ZeraMatrizes();

        /* Intializes random number generator */
        time_t t;
        srand((unsigned) time(&t));
 
        // Leitura da Matriz 
        for (linha = 1; linha < linhaMatriz; linha++)
            for (coluna = 1; coluna < colunaMatriz; coluna++)
                matriz[linha][coluna] = rand() % linhaMatriz;
                //scanf("%d", &matriz[linha][coluna]);  

        // inicio da contagem de tempo
        gettimeofday(&start, NULL);  

        while (!fim) {
            ++ciclo;

            // executa as threads
            verifMatriz();

            // troca os ponteiros da matriz secundaria para principal
            int **matrizT;
            matrizT = matrizS;
            matrizS = matriz;
            matriz = matrizT;

            // verifica unidade de tempo
            if(ciclo==u){
                fim = TRUE; 
            }
        }

        // Saidas do programa
        if((strstr(s, OP_OCEAN)) || (strstr(s, OP_ALL)) ){ 
            for (linha = 1; linha < linhaMatriz; linha++) {
                for (coluna = 1; coluna < colunaMatriz; coluna++) {
                    printf("%d ", (int)matriz[linha][coluna]); 
                }
                printf("\n"); 
            } 
        }

        if( (strstr(s, OP_TIME)) || (strstr(s, OP_ALL)) ){
            gettimeofday(&end, NULL);
            tempoExec = ( (end.tv_sec*1000000) + end.tv_usec)
                        - ( (start.tv_sec*1000000) + start.tv_usec);
            printf("%.8f \n", tempoExec / DIVISOR_TIME );
        }

        // Finaliza
        printf("\n"); 
        free(matriz);
        free(matrizS);
        return 0;

    // Se opcao invalida, exibo erro.
    }else if(!indEntValida){
        printf(ERRO_OPCAO);
        exit(EXIT_FAILURE);
    }
    
} // fim main

// Encontra, em um array, o maior valor.
int achaMaxVizinho(int a[]) {
    int c, max;
    max = a[0];  
    for (c = 1; c < 9; c++){   
        if(a[c] > max) 
            max = a[c];
    }  
    return max;
} 

// Zera as matrizes, inicializando com zeros
void ZeraMatrizes (){
    for (int linha = 0; linha < linhaTotal; linha++) {
        for (int coluna = 0; coluna < colunaTotal; coluna++) { 
            matriz[linha][coluna] = 0;
            matrizS[linha][coluna] = 0;
        }
    }
}

// Varre os vizinhos de cada posicao da matriz
// Aplicando as regras estabelecidas no exercicio
void *verifMatriz(){ 

    int valorMax = 0;
    int novoValor = 0;
    int ix = 0;
    int vizinhos[8] = {0};

    for (int i = 1; i < linhaMatriz; i++) {
        for (int j = 1; j < colunaMatriz; j++) {

            memset(vizinhos, 0, sizeof vizinhos);
            vizinhos[0] = matriz[i][j];
            ix = 1;
            valorMax = 0;
            novoValor = 0;
            for(int ll = i-1; ll <= i+1; ll++) {
                for(int cc = j-1; cc <= j+1; cc++) {
                    if(ll != i || cc != j) { 
                        vizinhos[ix] = matriz[ll][cc];
                        ++ix;
                    }
                }
            }

            // acha maximo e atualiza matriz secundaria
            valorMax = achaMaxVizinho(vizinhos); 
            if(valorMax==0){ novoValor = 0; }else{ novoValor = valorMax - 1; }
            matrizS[i][j] = novoValor;
        }
    } 
 
    return NULL;
}
