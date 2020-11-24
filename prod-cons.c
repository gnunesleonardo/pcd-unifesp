#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <sys/time.h>

#define MAX_N 100
#define MAX_TP 1
#define MAX_TC 4

int verifyPrime(long number, int th_id){
  int i, is_prime=1;
  for (i=2; i<number; i++){
    if (is_prime==1){
      if ((number%i)==0){
        is_prime = 0;
      }
    } else {
      break;
    }
  }
  return is_prime;
}

int main(int argc, char *argv[]){
  long *vet, number_tp, number_tc;
  int max_vet, max_thread, th_id;
  int index=0, nums=1, index_tc=0, index_tp=0;

  max_vet = MAX_N+MAX_TC;
  max_thread = MAX_TP+MAX_TC;

  vet = (long*)malloc(max_vet*sizeof(long));

  #pragma omp parallel \
    num_threads(max_thread) \
    default(none) \
    private (th_id, number_tp, number_tc) \
    shared(vet, nums, index_tp, index_tc, max_vet)
  {
    th_id = omp_get_thread_num();
    th_id = th_id+1;
    //printf("th_id %d\n", th_id);
    if (th_id <= MAX_TP){
      printf("th %d entrou\n", th_id);
      /* PRODUTOR */
      while (nums <= max_vet){
        if (nums <= MAX_N){
          number_tp = (long)(rand()%10);
        } else {
          number_tp = -1;
        }
        /* CRITICAL SECTION */
        #pragma omp critical
        {
          nums += 1;
          vet[index_tp] = number_tp;
          printf("th %d %d\n", th_id, number_tp);
          index_tp = index_tp+1;
        }
        /* CRITICAL SECTION */
      }
      /* PRODUTOR */
    } else {
      /* CONSUMIDOR */
      while (number_tc>=0) {
        if (index_tc < index_tp){
          /* CRITICAL SECTION */
          #pragma omp critical
          {
            number_tc = vet[index_tc];
            index_tc = index_tc+1;
          }
          /* CRITICAL SECTION */
          if (number_tc > 0){
            if(verifyPrime(number_tc, th_id)){
              printf("thread %d found that %d is prime\n", th_id, number_tc);
            }
          }
        }
      };
      /* CONSUMIDOR */
    }
  }
  return 0;
}