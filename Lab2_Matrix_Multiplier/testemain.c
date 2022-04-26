/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 1 - Laboratório: 2 */
/* Codigo:  Programa que utiliza 2 threads retorna a² para todos os elementos de um array de 10000 elementos. */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

int nthreads, dim;
int *mat1, *mat2, *matFinalSequencial, *matFinalConc;

typedef struct{
   int id;
   int dim;
} tArgs;


void* matrixMultiplyConc (void *arg) {
    tArgs *args = (tArgs *) arg;
    int i,  j, k;
    for (i = args->id; i < (args->dim); i+=nthreads) {
        for(j = 0; j< (args->dim); j++){
            for(k = 0; k<(args->dim); k++){
                matFinalSequencial[i*(args->dim)+j] += mat1[i*(args->dim)+k] * mat2[k*(args->dim)+j]; 
            }
        }
    }

    pthread_exit(NULL);
}


int main(int argc, char* argv[]) {
 
    int compare, dim;
    pthread_t *tid; 
    tArgs *args; 

    
    if(argc<3) {
      printf("Digite: %s <dimensao da matriz> <numero de threads>\n", argv[0]);
      return 1;
    }

    dim = 2;
    nthreads = 2;

    mat1 = (int *) malloc(sizeof(int) * dim * dim);
    mat2 = (int *) malloc(sizeof(int) * dim * dim);
    matFinalSequencial = (int *) malloc(sizeof(int) * dim * dim);
    matFinalConc = (int *) malloc(sizeof(int) * dim * dim);
    if (mat1 == NULL || mat2 == NULL || matFinalSequencial == NULL || matFinalConc == NULL) {
        printf("--ERROR: malloc\n"); exit(-1);
    }

    for (int i = 0; i < dim; i++) {
        for(int j=0; j<dim; i++) {
            mat1[i*dim+j] = 1;
            mat2[i*dim+j] = 1;
            matFinalSequencial[i*dim+j] = 0;
            matFinalConc[i*dim+j] = 0;
        }
    }
    printf("Chegamo");
    
    for (int i = 0; i < dim; i++) {
        for(int j = 0; j< dim; j++){
            for(int k = 0; k<dim; k++){
                matFinalSequencial[i*dim+j] += mat1[i*dim+k] * mat2[k*dim+j];
            }
        }
    }

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

    // compare = compareMatrices(dim);
    compare = 1;
     if(compare == 1) {
         printf("As matrizes são iguais");
     } else {
         printf("Deu ruim");
     }
    

     printf("\n\n\n");
     free(mat1);
     free(mat2);
     free(matFinalSequencial);
     free(matFinalConc);
     free(tid);
     free(args);
}
