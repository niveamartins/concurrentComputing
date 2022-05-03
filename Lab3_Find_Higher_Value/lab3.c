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
float biggestNumberSeq, smallestNumberSeq;

typedef struct{
   int id;
   int posInitial;
   int posFinal;
} tArgs;

typedef struct{
    float biggestNumberConc;
    float smallestNumberConc;
} retThread;


float* createAndPopulateArr(long long int quantity) {
    float *array;

    array = calloc(quantity, sizeof(float));
    if (array == NULL) {
        printf("--ERROR: calloc\n"); exit(-1);
    }

    for (int i = 0; i < quantity; i++) {
        array[i] = rand();
    }

    return array;
}


float findNumbersSeq (long long int quantity) {
    float biggestNumber = randomVector[0];
    float smallestNumber = randomVector[0];

    for (int i = 0; i < quantity; i++) {
        if (randomVector[i] > biggestNumber) {
            biggestNumber = randomVector[i];
        }

        if (randomVector[i] < smallestNumber) {
            smallestNumber = randomVector[i];
        }
    }

    biggestNumberSeq = biggestNumber;
    smallestNumberSeq = smallestNumber;
}

void* findNumbersConc (void *arg) {
    tArgs *args = (tArgs *) arg;
    retThread *ret;

    ret = (pthread_t*) malloc(sizeof(retThread));
    if(ret==NULL) {puts("ERRO--malloc"); return 2;}

    int firstPos = args->posInitial;
    
    float biggestNumber = randomVector[firstPos];
    float smallestNumber = randomVector[firstPos];

    for (int i = args->posInitial; i <= (args->posFinal) ; i++) {

        if (randomVector[i] > biggestNumber) {
            biggestNumber = randomVector[i];
        }

        if (randomVector[i] < smallestNumber) {
            smallestNumber = randomVector[i];
        }
    }

    ret->biggestNumberConc = biggestNumber;
    ret->smallestNumberConc = smallestNumber;

    printf(">>> thread %d executada com sucesso. \n", args->id);
    pthread_exit((void *) ret); 
}

int main(int argc, char* argv[]) {
    long long int dim;
    float biggestNumberConc = -1, smallestNumberConc = -1;
    retThread * retorno;
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

    findNumbersSeq(dim); 

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
        (args+i)->posFinal = ((i+1) * (int)(dim/nthreads))-1;
        if(pthread_create(tid+i, NULL, findNumbersConc, (void*) (args+i))){
            puts("ERRO--pthread_create"); return 3;
        }
    }

    biggestNumberConc = randomVector[0];
    smallestNumberConc = randomVector[0];

    for(long int i=0; i<nthreads; i++) {
    
      if(pthread_join(*(tid+i), (void **) &retorno)){
         fprintf(stderr, "ERRO--pthread_create\n");
         return 3;
      }


      if (biggestNumberConc < (retorno)->biggestNumberConc) {
          biggestNumberConc = (retorno)->biggestNumberConc;
      }
      
      if (smallestNumberConc > (retorno)->smallestNumberConc) {
          smallestNumberConc = (retorno)->smallestNumberConc;
      }
   }


    GET_TIME(finish);
    elapsedConc = finish - start;

    printf(">>>>> Tempo decorrido para achar o maior: \n%lf segundos. \n\n", elapsedConc);

    printf("--- FIM DO ALGORITMO CONCORRENTE --- \n\n\n"); 

    printf("--- RESULTADOS DO PROGRAMA --- \n\n"); 


    if (biggestNumberConc == biggestNumberSeq) {
        printf(">>>>> O maior número é igual.\n");
    } else {
        printf(">>>>> O maior número NÃO é igual.\n");
    }

    if (smallestNumberConc == smallestNumberSeq) {
        printf(">>>>> O menor número é igual.\n");
    } else {
        printf(">>>>> O menor número NÂO é igual: %f %f\n", smallestNumberConc, smallestNumberSeq);
    }

    
    printf(">>>>> Ganho de desempenho: %lf \n", (elapsedSeq/elapsedConc));
}
