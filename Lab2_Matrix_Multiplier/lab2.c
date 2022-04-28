/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 1 - Laboratório: 2 */
/* Codigo:   */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include "timer.h"

int nthreads;
int *mat1, *mat2, *matFinalSequencial, *matFinalConc;

typedef struct{
   int id;
   int dim;
} tArgs;

int* createAndPopulateArr(int dim) {
    int *array;

    array = calloc((dim * dim), sizeof(int));
    if (array == NULL) {
        printf("--ERROR: calloc\n"); exit(-1);
    }

    for (int i = 0; i < dim * dim; i++) {
        array[i] = rand() % 10;
    }

    return array;
}

int* prepareFinalArr(int dim) {
    int *array;

    array = calloc((dim * dim), sizeof(int));
    if (array == NULL) {
        printf("--ERROR: calloc\n"); exit(-1);
    }

    for (int i = 0; i < dim * dim; i++) {
        array[i] = 0;
    }

    return array;
}

void* matrixMultiplySeq (int dim) {
    for (int i = 0; i < dim; i ++) {
        for(int j = 0; j< dim; j++){
            for(int k = 0; k<dim; k++){
                matFinalSequencial[i*dim+j] += mat1[i*dim+k] * mat2[k*dim+j];
            }
        }
    }
}

void* matrixMultiplyConc (void *arg) {
    tArgs *args = (tArgs *) arg;
    
    for (int i = args->id; i < (args->dim); i+=nthreads) {
        for(int j = 0; j< (args->dim); j++){
            for(int k = 0; k<(args->dim); k++){
                matFinalConc[i*(args->dim)+j] += mat1[i*(args->dim)+k] * mat2[k*(args->dim)+j];
            }
        }
    }

    printf(">>> thread %d executada com sucesso. \n", args->id);
    pthread_exit(NULL);
}

void* compareMatrices(int dim) {   
    for (int i = 0; i < dim; i++) {
        for(int j = 0; j< dim; j++){
            if (matFinalConc[i*dim+j] != matFinalSequencial[i*dim+j]) {
                  printf(">>>>> As matrizes são diferentes.\n");
                  return 0;
           }
        }
    }

    printf(">>>>> As matrizes são iguais.\n");
}

int main(int argc, char* argv[]) {
    int compare, dim;
    double start, finish, elapsedSeq, elapsedConc;
    pthread_t *tid; 
    tArgs *args; 

    if(argc<3) {
      printf("Digite: %s <dimensao da matriz> <numero de threads>\n", argv[0]);
      return 1;
    }

    dim = atoi(argv[1]);
    nthreads = atoi(argv[2]);
    if (nthreads > dim) nthreads=dim;

    printf("--- PROGRAMA INICIADO COM %d THREADS E MATRIZES DE DIMENSÃO %d --- \n\n", nthreads, dim); 

    printf("--- CRIANDO E POVOANDO AS MATRIZES --- \n\n"); 
    mat1 = createAndPopulateArr(dim);

    printf(">>>>> Primeira matriz criada.\n");

    mat2 = createAndPopulateArr(dim);

    printf(">>>>> Segunda matriz criada.\n\n");

    matFinalSequencial = prepareFinalArr(dim);
    matFinalConc = prepareFinalArr(dim);

    printf("--- COMEÇO DA MULTIPLICAÇÃO SEQUENCIAL --- \n\n"); 
    GET_TIME(start);

    matrixMultiplySeq(dim); 
    
    GET_TIME(finish);
    elapsedSeq = finish - start;

    printf(">>>>> Tempo decorrido para multiplicar: \n%lf segundos.\n\n", elapsedSeq);

    printf("--- FIM DA MULTIPLICAÇÃO SEQUENCIAL --- \n\n\n"); 
    printf("--- COMEÇO DA MULTIPLICAÇÃO CONCORRENTE --- \n\n"); 
    GET_TIME(start);
    
    tid = (pthread_t*) malloc(sizeof(pthread_t)*nthreads);
    if(tid==NULL) {puts("ERRO--malloc"); return 2;}
    args = (tArgs*) malloc(sizeof(tArgs)*nthreads);
    if(args==NULL) {puts("ERRO--malloc"); return 2;}

    printf(">>>>> Iniciando as threads: \n"); 
    for(int i=0; i<nthreads; i++) {
        (args+i)->id = i;
        (args+i)->dim = dim;
        if(pthread_create(tid+i, NULL, matrixMultiplyConc, (void*) (args+i))){
            puts("ERRO--pthread_create"); return 3;
        }
    }


    for(int i=0; i<nthreads; i++) {
        pthread_join(*(tid+i), NULL);
    }

    GET_TIME(finish);
    elapsedConc = finish - start;

    printf(">>>>> Tempo decorrido para multiplicar: \n%lf segundos. \n\n", elapsedConc);

    printf("--- FIM DA MULTIPLICAÇÃO CONCORRENTE --- \n\n\n"); 

    printf("--- RESULTADOS DO PROGRAMA --- \n\n"); 

    compareMatrices(dim);

    
    printf(">>>>> Ganho de desempenho: %lf \n", (elapsedSeq/elapsedConc));
}
