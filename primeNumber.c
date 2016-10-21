/*
 * File: primeNumber.c
 * Prime Number Finder with OpenMP (Open Multi-Processing)
 *
 * Author: Johnatan Oliveira ~ johnoliv[at]gmail.com
 * www.johnatan.net
 * Version 0.1
 *
 * Este programa busca por todos os números primos em um intervalo estipulado de 2 a m e
 * exibe o resultado de acordo com a opção escolhida pelo usuario.
 * Entrada:
 *     - argv[1] = Limite m (Busca numeros primos de 2 a m)
 *     - argv[2] = Opção de Saida 
 *         - time: Uma linha contendo o tempo de execucão do programa.
 *         - list: Uma linha contendo a lista de números primos.
 *         - sum: Uma linha contendo a soma dos números primos.
 *         - all: Lista de números primos, soma destes números e o tempo de execução. Nesta ordem. 
 *     - argv[3] = Quantidade de Threads a serem criadas
 *
 * ---------------------------------------------------------------
 * WARNING: THIS IS NOT THE BEST METHOD TO MEET PRIME NUMBERS, 
 * THERE ARE OTHER METHODS MORE EFFICIENT. THIS IS ONLY AN EXAMPLE.
 * ---------------------------------------------------------------
 */ 

#include <stdio.h>
#include <omp.h>
#include <string.h>
#include <stdlib.h>  
#include <math.h>

#define schedType dynamic
#define OP_TIME "time"
#define OP_LIST "list"
#define OP_SUM "sum"
#define OP_ALL "all" 
#define MINLIM 2
#define MAXLIM 1000000000
#define TRUE 0
#define FALSE 1
#define ERRO_LIM_MIN "Erro: Limite superior muito baixo. \n"
#define ERRO_LIM_MAX "Erro: Limite superior muito alto, > 10^9. \n"
#define ERRO_OPCAO "Erro: Opcao Invalida! \n"
 
int main( int argc, char *argv[] ){

    // inicializacao de variaveis
    int i, j, isPrime, limitTest;
    double tempoInicio = omp_get_wtime();
    double tempoTotal;
    int indEntValida = 0;
    int unsigned long soma = 0LU;  
    int m = strtol(argv[1], NULL, 10);
    char *s = argv[2];
    int t = strtol(argv[3], NULL, 10);   
    if(t==0){ t = 1; };
    int chunk = m/2; 
    const char *op[] = { "time", "list", "sum", "all" }; 

    // Verifico se a entrada eh valida
    int y = 0;  
    int len = sizeof(op)/sizeof(op[0]); 
    for (y=0; y < len; y++) {
        if(strstr(op[y], s)){ 
            indEntValida = 1;
            break; 
        }
    }

    // validacao de limite
    if(m < MINLIM){
        printf(ERRO_LIM_MIN);
        abort();
    }
    if(m > MAXLIM){
        printf(ERRO_LIM_MAX);
        abort(); 
    }    

    // inicializo array de numeros primos encontrados
    int *arrayNumbers;
    int tamArea = m * sizeof(int);
    arrayNumbers = malloc( tamArea );
    memset(arrayNumbers, 0, tamArea );

    // se a entrada de opcao for valida 
    if(indEntValida){  
        
        // Paralizacao do FOR responsavel por encontrar os numeros primos entre o range informado
        #pragma omp parallel for num_threads ( t ) reduction(+:soma) \
            schedule(schedType,chunk) shared(m, s, arrayNumbers, chunk) private(i, j, isPrime, limitTest) default(none)
            for(i = 2; i <= m; i++){
                isPrime = TRUE;
                // limitando a busca ate a raiz quadrada
                limitTest = sqrt(i) + 1;
                //printf("\n i = %d e limitTest = %d", i, limitTest);
                for(j = 2; j < limitTest || j*j <= i; j++){ 
                     //printf("\n j=%d (j*j=%d i=%d)", j, j*j, i);  
                     if(i % j == 0){ 
                         isPrime = FALSE;
                         break;
                     }
                }
                // Se o usuario escolher as opcoes list ou all, gravo os numeros primos em um array.
                // Se opcao sum ou all, somo os numeros primos encontrados.
                if(isPrime==TRUE){
                    if( (strstr(s, OP_LIST)) || (strstr(s, OP_ALL)) )
                        arrayNumbers[i] = i;
                    if( (strstr(s, OP_SUM)) || (strstr(s, OP_ALL)) )
                        soma += i;
                }
            }   

        // Saida do programa
        if((strstr(s, OP_LIST)) || (strstr(s, OP_ALL)) ){
            for (y=0; y < m; y++) { 
                if( (arrayNumbers[y] > 0) )
                    printf("%d ", arrayNumbers[y]);
            }
            printf("\n"); 
        }

        if( (strstr(s, OP_SUM)) || (strstr(s, OP_ALL)) )
            printf("%lu\n", soma);
        
        if( (strstr(s, OP_TIME)) || (strstr(s, OP_ALL)) ){
            tempoTotal = omp_get_wtime() - tempoInicio;
            printf("%f \n", tempoTotal);
        }
 
       printf("\n");
       free(arrayNumbers);
       return 0;

    // Se opcao invalida, exibo erro.
    }else if(!indEntValida){
        printf(ERRO_OPCAO);
        return 0;
    }
} 
