/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 1 - Laboratório: 2 */
/* Codigo:   */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include "timer.h"

int nthreads;
float *randomVector;

typedef struct{
   int id;
   int posInitial;
   int posFinal;
} tArgs;

float* createAndPopulateArr(long long int quantity) {
    float *array;

    array = calloc(quantity, sizeof(float));
    if (array == NULL) {
        printf("--ERROR: calloc\n"); exit(-1);
    }

    for (int i = 0; i < quantity; i++) {
        array[i] = rand() % 10;
    }

    return array;
}


float findBiggestNumberSeq (long long int quantity) {
    float biggestNumber = -1;
    for (int i = 0; i < quantity; i ++) {
        if (randomVector[i] > biggestNumber) {
            printf("%d %d \n", randomVector[i], biggestNumber);
            biggestNumber = randomVector[i];
        }
    }

    return biggestNumber;
}

void* findBiggestNumberConc (void *arg) {
    tArgs *args = (tArgs *) arg;
    
    long int biggestNumber = -1;

    for (int i = args->posInitial; i <= (args->posFinal) ; i ++) {
        printf("thread %d: %d %d \n", args->id, randomVector[i], biggestNumber);
        if (randomVector[i] > biggestNumber) {
            biggestNumber = randomVector[i];
        }
    }

    printf(">>> thread %d executada com sucesso. \n", args->id);
     pthread_exit((void *) biggestNumber); 
}

// void* compareMatrices(int dim) {   
//     for (int i = 0; i < dim; i++) {
//         for(int j = 0; j< dim; j++){
//             if (matFinalConc[i*dim+j] != matFinalSequencial[i*dim+j]) {
//                   printf(">>>>> As matrizes são diferentes.\n");
//                   return 0;
//            }
//         }
//     }

//     printf(">>>>> As matrizes são iguais.\n");
// }

int main(int argc, char* argv[]) {
    long long int dim;
    float biggestNumberSeq = -1;
    float biggestNumberConc = -1;
    long int retorno;
    double start, finish, elapsedSeq, elapsedConc;
    pthread_t *tid; 
    tArgs *args; 

    if(argc<3) {
      printf("Digite: %s <dimensao da vetor> <numero de threads>\n", argv[0]);
      return 1;
    }

    dim = atoll(argv[1]);
    nthreads = atoi(argv[2]);
    if (nthreads > dim) nthreads=dim;

    printf("--- PROGRAMA INICIADO COM %d THREADS E VETOR DE TAMANHO %d --- \n\n", nthreads, dim); 

    printf("--- CRIANDO E POVOANDO O VETOR --- \n\n"); 
    randomVector = createAndPopulateArr(dim);

    printf(">>>>> Vetor criado.\n");

    printf("--- COMEÇO DO ALGORITMO SEQUENCIAL --- \n\n"); 
    GET_TIME(start);

    biggestNumberSeq = findBiggestNumberSeq(dim); 
    
    GET_TIME(finish);
    elapsedSeq = finish - start;

    printf(">>>>> Tempo decorrido para achar o maior: \n%lf segundos.\n\n", elapsedSeq);

    printf("--- FIM DO ALGORITMO SEQUENCIAL --- \n\n\n"); 
    printf("--- COMEÇO DO ALGORITMO CONCORRENTE --- \n\n"); 
    GET_TIME(start);
    
    tid = (pthread_t*) malloc(sizeof(pthread_t)*nthreads);
    if(tid==NULL) {puts("ERRO--malloc"); return 2;}
    args = (tArgs*) malloc(sizeof(tArgs)*nthreads);
    if(args==NULL) {puts("ERRO--malloc"); return 2;}

    printf(">>>>> Iniciando as threads: \n"); 
    for(int i=0; i<nthreads; i++) {
        (args+i)->id = i;
        (args+i)->posInitial= i * (int)(dim/nthreads); 
        (args+i)->posFinal = ((i+1) * (int)(dim/nthreads));
        if(pthread_create(tid+i, NULL, findBiggestNumberConc, (void*) (args+i))){
            puts("ERRO--pthread_create"); return 3;
        }
    }


    for(long int i=0; i<nthreads; i++) {
      if(pthread_join(*(tid+i), (void**) &retorno)){
         fprintf(stderr, "ERRO--pthread_create\n");
         return 3;
      }
      
      if (biggestNumberConc < retorno) {
          biggestNumberConc = retorno;
      }
   }

    GET_TIME(finish);
    elapsedConc = finish - start;

    printf(">>>>> Tempo decorrido para achar o maior: \n%lf segundos. \n\n", elapsedConc);

    printf("--- FIM DO ALGORITMO CONCORRENTE --- \n\n\n"); 

    printf("--- RESULTADOS DO PROGRAMA --- \n\n"); 

    // compareMatrices(dim);

    printf("%f %f", biggestNumberConc, biggestNumberSeq);
    
    printf(">>>>> Ganho de desempenho: %lf \n", (elapsedSeq/elapsedConc));
}
