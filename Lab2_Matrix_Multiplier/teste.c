/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 1 - Laboratório: 2 */
/* Codigo:  Programa que utiliza 2 threads retorna a² para todos os elementos de um array de 10000 elementos. */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

int nthreads;
int *mat1, *mat2, *matFinalSequencial, *matFinalConc;

typedef struct{
   int id;
   int dim;
} tArgs;


void* matrixMultiplyConc (void *arg) {
    tArgs *args = (tArgs *) arg;
    int i;
    for (i = args->id; i < (args->dim); i+=nthreads) {
        for(int j = 0; j< (args->dim); j++){
            for(int k = 0; k<(args->dim); k++){
                matFinalSequencial[i*(args->dim)+j] += mat1[i*(args->dim)+k] * mat2[k*(args->dim)+j];
            }
        }
    }

    free(arg); 

    pthread_exit(NULL);
}

void* matrixMultiplySeq (int dim) {
    int i;
    for (i = 0; i < dim; i ++) {
        for(int j = 0; j< dim; j++){
            for(int k = 0; k<dim; k++){
                matFinalSequencial[i*dim+j] += mat1[i*dim+k] * mat2[k*dim+j];
            }
        }
    }
}

int* createArr(int dim) {
    int i;
    float *array;

    array = malloc(sizeof(int) * dim * dim);
    if (array == NULL) {
        printf("--ERROR: malloc\n"); exit(-1);
    }

    for (i = 0; i < dim; i++) {
        for(int j=0; j<dim; i++) {
            array[i*dim+j] = rand() % 10;
        }
    }

    return (int *) array;
}

int* allocArr(int dim) {
    int *array;

    array = malloc(sizeof(int) * dim * dim);
    if (array == NULL) {
        printf("--ERROR: malloc\n"); exit(-1);
    }

    return (int *) array;
}

int compareMatrices(int dim) {
    
    for (int i = 0; i < dim; i++) {
        for (int j = 0; i < dim; j++) {
         if (matFinalSequencial[i*dim+j] =! matFinalConc[i*dim+j]) {
             return 0;
         }
        }
    }

    return 1;
} 

int main(int argc, char* argv[]) {
 
    int compare, dim;
    pthread_t *tid; 
    tArgs *args; 

    
    if(argc<3) {
      printf("Digite: %s <dimensao da matriz> <numero de threads>\n", argv[0]);
      return 1;
    }
    dim = atoi(argv[1]);
    nthreads = atoi(argv[2]);
    if (nthreads > dim) nthreads=dim;

    mat1 = createArr(dim);
    mat2 = createArr(dim);

    matFinalSequencial = createArr(dim);
    matFinalConc = createArr(dim);

    matFinalSequencial = matrixMultiplySeq(dim);

    tid = (pthread_t*) malloc(sizeof(pthread_t)*nthreads);
    if(tid==NULL) {puts("ERRO--malloc"); return 2;}
    args = (tArgs*) malloc(sizeof(tArgs)*nthreads);
    if(args==NULL) {puts("ERRO--malloc"); return 2;}
    
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

     compare = compareMatrices(dim);

     if(compare == 1) {
         printf("As matrizes são iguais");
     } else {
         printf("Deu ruim");
     }
    

     printf("\n\n\n");
     // pthread_exit(NULL);
}
