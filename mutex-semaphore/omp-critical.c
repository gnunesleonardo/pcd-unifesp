#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <sys/time.h>

#define MAX_VET 100000
#define MAX_THREADS 8

int main(int argc, char *argv[]){
  double *vet1, *vet2, redux, soma;
  int i=0, th_id, t_mili;
  struct timeval t_init, t_final;

  vet1 = (double*)malloc(MAX_VET*sizeof(double));
  vet2 = (double*)malloc(MAX_VET*sizeof(double));

  #pragma omp parallel \
    num_threads(4) \
    default(none) \
    private (i, th_id) \
    shared(vet1, vet2)
  {
    th_id = omp_get_thread_num();
    #pragma omp for
    for (i=0; i<MAX_VET; i++){
      vet1[i] = rand();
      vet2[i] = rand();
    }
  }

  gettimeofday(&t_init, NULL);
  #pragma omp parallel \
    num_threads(MAX_THREADS) \
    default(none) \
    private (i, redux, th_id) \
    shared(vet1, vet2, soma)
  {
    redux = 0;
    #pragma omp for
    for (i=0; i<MAX_VET; i++){
      redux += vet1[i]*vet2[i];
    }
    #pragma omp critical
    {
      soma = soma+redux;
    }
  }
  printf("soma=%f\n", soma);
  gettimeofday(&t_final, NULL);
  t_mili = (int) (1000*(t_final.tv_sec-t_init.tv_sec)+(t_final.tv_usec-t_init.tv_usec)/1000);
  printf("total time=%dms\n", t_mili);
  return 0;
}
