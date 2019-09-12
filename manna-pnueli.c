#include <stdio.h>
#include <stdlib.h>

#define MAX_ITER 100
#define NUM_THREADS 5

int main(int argc, char *argv[]){
  int soma=0, request=0, respond=0, iter=0;
  int th_id, local, local_iter=0;
  #pragma omp parallel \
  num_threads(NUM_THREADS) \
  default(none) \
  firstprivate(local_iter) \
  private(th_id, local) \
  shared(soma, request, respond, iter)
  {
    th_id = omp_get_thread_num();
    if (th_id == 0){
      /* SERVER */
      while (iter < MAX_ITER){
        while (request == 0){/* BUSY WAIT */}
        respond = request;
        while (respond != 0){/* BUSY WAIT */}
        request = 0;
      }
      /* SERVER */
    } else {
      /* CLIENT */
      while (local_iter < MAX_ITER/(NUM_THREADS-1)){
        while (respond != th_id){
          request = th_id;
        }
        /* CRITICAL SECTION */
        local = soma;
        sleep(rand()%2);
        soma = local + 1;
        iter += 1;
        printf("thread %d iter %d\n", th_id, iter);
        local_iter += 1;
        /* CRITICAL SECTION */
        respond = 0;
      }
      /* CLIENT */
    }
  }
  printf("soma=%d\n", soma);
  return 0;
}
