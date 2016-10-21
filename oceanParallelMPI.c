/*  
 * File: oceanParallelMPI.c
 * Ocean Approuch with Message Passing Interface (MPI)
 * 
 * Author: Johnatan Oliveira ~ johnoliv[at]gmail.com
 * www.johnatan.net
 * Version 0.1
 */

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <sys/time.h>
#include <math.h> 

#define FALSE 0
#define TRUE 1
#define OP_OCEAN "ocean" 
#define OP_TIME "time"
#define OP_ALL "all" 
#define ERRO_OPCAO "Erro: Opcao Invalida! \n"
#define ERRO_MATRIZ "Erro: Linha e coluna devem ser maior que 1! \n"
#define ERRO_MATRIZ2 "Matriz incorreta, Linha e Coluna devem ser iguais. \n"
#define ERRO_MATRIZ3 "Tamanho não divisivel pelo numero de Threads. \n"  

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

int **alocaMatriz(int lin, int col) {
  int **matrizResult; 
  matrizResult=(int **)malloc(lin * sizeof(int *)); 
  matrizResult[0]=(int *)malloc(lin * col * sizeof(int)); 
  for(int i = 1; i < lin ; i++)
    matrizResult[i] = matrizResult[i-1]+col;

  return matrizResult;
}

void desalocaMatriz(int **mat, int lin) { 
  for(int i = 1; i < lin; i++)
    mat[i]=NULL;

  free(mat[0]);
  free(mat);
}

int main(int argc, char * argv[]) {

    int comm_sz, my_rank, de, ate; 
    MPI_Init (&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);  
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    int linhaTotal, linhaMatriz, colunaTotal, colunaMatriz; 
    int **matriz, **matrizS;

    // inicializacao de variaveis
    int indEntValida = 0, linha = 0, coluna = 0; 
    int i = strtol(argv[1], NULL, 10);  // linhas
    int j = strtol(argv[2], NULL, 10);  // colunas
    int u = strtol(argv[3], NULL, 10);  // unidade de tempo 
    char *s = argv[4];                  // tipos de saida  
    int fim = FALSE;
    int ciclo = 0; 
    if(u==0){ u = 1; }; 
    const char *op[] = { "ocean", "time", "all" };
    double start_time, end_init, end_calc, end_time; 

    // inicializa as matrizes com tamanho maior para nao
    // sofrer com posicoes nao encontradas...
    linhaTotal = i+2;
    linhaMatriz = i+1;
    colunaTotal = j+2;
    colunaMatriz = j+1;

    if( linhaTotal != colunaTotal ){
        printf(ERRO_MATRIZ2);
        MPI_Finalize();
        exit(EXIT_FAILURE);
    }

    if (linhaTotal % comm_sz != 0) {
        if (my_rank==0)
            printf(ERRO_MATRIZ3); 
        MPI_Finalize();
        exit(EXIT_FAILURE);
    }

    // Verifico se a entrada eh valida
    if (s == NULL) {
        printf(ERRO_OPCAO);
        MPI_Finalize();
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

    if(!indEntValida){
        printf(ERRO_OPCAO);
        MPI_Finalize();
        exit(EXIT_FAILURE);
    } 

    if( i < 1 || j < 1 ){
        printf(ERRO_MATRIZ);
        MPI_Finalize();
        exit(EXIT_FAILURE);
    }  

    // Alocando as matriz
    matriz = alocaMatriz(linhaTotal, colunaTotal);
    matrizS = alocaMatriz(linhaTotal, colunaTotal); 

    if (my_rank == 0) { 
 
        for (int linha = 0; linha < linhaTotal; linha++)
            for (int coluna = 0; coluna < colunaTotal; coluna++)
                matriz[linha][coluna] = 0;
 
        time_t t;
        srand((unsigned) time(&t));
 
        // Leitura da Matriz
        for (linha = 1; linha < linhaMatriz; linha++)
            for (coluna = 1; coluna < colunaMatriz; coluna++)
                matriz[linha][coluna] = rand() % linhaMatriz;
                //scanf("%d", &matriz[linha][coluna]); 

        start_time=MPI_Wtime(); 
    }

    MPI_Bcast (*matriz, linhaTotal*linhaTotal, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    end_init=MPI_Wtime();  

    while (!fim) {
        ++ciclo; 

        de = my_rank * linhaTotal/comm_sz;
        ate = (my_rank+1) * linhaTotal/comm_sz;

        if(de==0) de++;
        if(ate >= linhaTotal) ate = linhaTotal-1;
      
        for (int i = de; i < ate; i++) {
            for (int j = 1; j < colunaMatriz; j++) {

                int valorMax = 0;
                int novoValor = 0;
                int ix = 0;
                int vizinhos[8] = {0};

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

        if(de==1) de--;
        MPI_Gather (matrizS[de], linhaTotal*linhaTotal/comm_sz, MPI_INT, *matrizS, 
                    linhaTotal*linhaTotal/comm_sz, MPI_INT, 0, MPI_COMM_WORLD); 

        MPI_Barrier(MPI_COMM_WORLD);

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
    
    MPI_Barrier(MPI_COMM_WORLD);

    if (my_rank == 0){ 

        end_calc=MPI_Wtime();
        end_time=MPI_Wtime(); 
        
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
            printf("%0.10lg\n", (double)(end_calc-end_init)); 
        } 

        desalocaMatriz(matriz, linhaTotal);
        desalocaMatriz(matrizS, linhaTotal);
        printf("\n");

    }  

    MPI_Finalize ( );
    return 0;
    
} // fim main
