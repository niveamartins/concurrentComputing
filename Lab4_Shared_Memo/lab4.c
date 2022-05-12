/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 1 - Laboratório: 2 */
/* Codigo:   */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <math.h>
#include "timer.h"

int nthreads;
float *finalVectorC, *finalVectorS, *initialVector;
long int dim = 0;
pthread_mutex_t mutex; //variavel de lock para exclusao mutua

typedef struct{
   int id;
   int posInitial;
   int posFinal;
} tArgs;


float* createAndPopulateArr(long int quantity) {
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

float* createArr(long int quantity) {
    float *array;

    array = calloc(quantity, sizeof(float));
    if (array == NULL) {
        printf("--ERROR: calloc\n"); exit(-1);
    }

    for (int i = 0; i < quantity; i++) {
        array[i] = 0;
    }

    return array;
}

int isPrime(int x) {
    int aux, i;

    if (x==2) return 1;

    for(i=2; i<=(x/2); i++) {
        if((x%i) == 0) return 0;
    }

    return 1;
}


void * processingVectorConcurrent(void *arg) {
    tArgs *args = (tArgs *) arg;
    float result;
    float actual;
    int initial = args->posInitial;
    int final = args->posFinal;

    for(int i = initial; i <= final; i++) {
        pthread_mutex_lock(&mutex);
        actual = initialVector[i];
        pthread_mutex_unlock(&mutex);

        if (isPrime(actual) == 1) {
            result = sqrt(actual);
        } else {
            result = actual;
        }
        pthread_mutex_lock(&mutex);
        finalVectorC[i] = result;
        pthread_mutex_unlock(&mutex);
    }
}

void compareMatrices() {  
    for (int i = 0; i < dim; i++) {
        if (finalVectorC[i] != finalVectorS[i]) {
            printf("%lf   %lf \n", finalVectorC[i], finalVectorS[i]);
            printf(">>>>> As matrizes são diferentes.\n"); 
            return;
        }
    }

    printf(">>>>> As matrizes são iguais.\n");
}


int main(int argc, char* argv[]) {
    
    double start, finish, elapsedSeq, elapsedConc;
    pthread_t *tid; 
    tArgs *args; 
    float temp;

    if(argc<3) {
      printf("Digite: %s <dimensao da vetor> <numero de threads>\n", argv[0]);
      return 1;
    }

    dim = atoi(argv[1]);
    nthreads = atoi(argv[2]);
    if (nthreads > dim) nthreads=dim;

    printf("--- PROGRAMA INICIADO COM %d THREADS E VETOR DE TAMANHO %d --- \n\n", nthreads, dim); 

    printf("--- CRIANDO E POVOANDO O VETOR --- \n\n"); 
    initialVector = createAndPopulateArr(dim);
    finalVectorS = createArr(dim);
    finalVectorC = createArr(dim);

    printf(">>>>> Vetores criados.\n");

    

    printf("--- COMEÇO DO ALGORITMO SEQUENCIAL --- \n\n"); 
    GET_TIME(start);

    for(int i=0; i<dim; i++) {
        if (isPrime(initialVector[i]) == 1) {
            finalVectorS[i] = sqrt(initialVector[i]);
        }
        else {
            finalVectorS[i] = initialVector[i];
        }
    }


    GET_TIME(finish);
    elapsedSeq = finish - start;

    printf(">>>>> Tempo decorrido para processar valores primos: \n%lf segundos.\n\n", elapsedSeq);

    printf("--- FIM DO ALGORITMO SEQUENCIAL --- \n\n\n"); 
    printf("--- COMEÇO DO ALGORITMO CONCORRENTE --- \n\n"); 
    GET_TIME(start);
    
    tid = (pthread_t*) malloc(sizeof(pthread_t)*nthreads);
    if(tid==NULL) {puts("ERRO--malloc"); return 2;}
    args = (tArgs*) malloc(sizeof(tArgs)*nthreads);
    if(args==NULL) {puts("ERRO--malloc"); return 2;}

    pthread_mutex_init(&mutex, NULL);

    printf(">>>>> Iniciando as threads: \n"); 

    for(int i=0; i<nthreads; i++) {
        printf("criando thread %d \n", i);
        (args+i)->id = i;
        if (dim % nthreads != 0) {
            (args+i)->posInitial= i * ((dim + (dim % nthreads))/nthreads);
            (args+i)->posFinal = ((i+1) * ((dim + (dim % nthreads))/nthreads));
        } else {
            (args+i)->posInitial= i * (dim/nthreads); 
            (args+i)->posFinal = ((i+1) * (dim/nthreads));
        }
        if(pthread_create(tid+i, NULL, processingVectorConcurrent, (void*) (args+i))){
            puts("ERRO--pthread_create"); return 3;
        }
    }


    for(int i=0; i<nthreads; i++) {
        pthread_join(*(tid+i), NULL);
    }

    pthread_mutex_destroy(&mutex);

    GET_TIME(finish);
    elapsedConc = finish - start;

    printf(">>>>> Tempo decorrido para processar valores primos: \n%lf segundos. \n\n", elapsedConc);

    printf("--- FIM DO ALGORITMO CONCORRENTE --- \n\n\n"); 

    printf("--- RESULTADOS DO PROGRAMA --- \n\n"); 
    compareMatrices();
    

    printf(">>>>> Ganho de desempenho: %lf \n", (elapsedSeq/elapsedConc));
}
