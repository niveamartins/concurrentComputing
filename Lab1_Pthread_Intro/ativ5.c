/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 1 - Laboratório: 1 */
/* Codigo:  Programa que utiliza 2 threads retorna a² para todos os elementos de um array de 10000 elementos. */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

#define ARRSIZE 10000

typedef struct {
   int *array, initPos, finalPos;
} t_Args;


void *power2 (void *arg) {
    t_Args *args = (t_Args *) arg;
    int i;
    for (i = args->initPos; i <= args->finalPos; i++) {
        args->array[i] = args->array[i] * args->array[i]; 
    }

    free(arg); 

    pthread_exit(NULL);
}

int* createArr() {
    int i, *array;

    array = calloc(ARRSIZE, sizeof(int));
    if (array == NULL) {
        printf("--ERROR: calloc\n"); exit(-1);
    }

    for (i = 0; i < ARRSIZE; i++) {
        array[i] = i + 1;
    }

    return array;
}

//funcao principal do programa
int main() {
    pthread_t tid_sistema[2];
    int i, *arr;

    t_Args *thread1, *thread2;

    arr = createArr();

    printf(">>> ARRAY INICIAL <<< \n");
    for(i=0; i<ARRSIZE; i++) {
        printf("%d ", arr[i]);
    }

    thread1 = malloc(sizeof(t_Args));
    if (thread1 == NULL) {
        printf("--ERRO: malloc()\n"); exit(-1);
    }

    thread1->array = arr;
    thread1->initPos = 0;
    thread1->finalPos = (ARRSIZE/2) - 1;


    thread2 = malloc(sizeof(t_Args));
    if (thread2 == NULL) {
        printf("--ERRO: malloc()\n"); exit(-1);
    }

    thread2->array = arr;
    thread2->initPos = (ARRSIZE/2);
    thread2->finalPos = ARRSIZE; 

    if (pthread_create(&tid_sistema[0], NULL, power2, (void*) thread1)) {
      printf("--ERRO: pthread_create()\n"); exit(-1);
    }

    if (pthread_create(&tid_sistema[1], NULL, power2, (void*) thread2)) {
      printf("--ERRO: pthread_create()\n"); exit(-1);
    }

    if (pthread_join(tid_sistema[0], NULL)) {
         printf("--ERRO: pthread_join() \n"); exit(-1); 
    }

    if (pthread_join(tid_sistema[1], NULL)) {
         printf("--ERRO: pthread_join() \n"); exit(-1); 
    }

    printf("\n\n >>> ARRAY FINAL <<< \n");
    for(i=0; i<ARRSIZE; i++) {
        printf("%d ", arr[i]);
    }

    printf("\n\n\n");
    pthread_exit(NULL);
}
