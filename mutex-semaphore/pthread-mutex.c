#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

#define MAX_VET 10000000000
#define MAX_THREADS 8

double *vet1, *vet2, soma=0;
long DIV_VET = MAX_VET/MAX_THREADS;
pthread_mutex_t sb;

void *calcReduceSoma(void *tid){
    long i, thid, inicial, final;
    double redux=0;
    thid = (int) tid+1;

    if (thid==1) {
        inicial = 0;
        final = DIV_VET;
    } else {
        inicial = ((thid-1)*DIV_VET);
        if (thid != MAX_THREADS) final = inicial+DIV_VET;
        else final = MAX_VET;
    }
    
    for (i=inicial; i<final; i++){
        redux += vet1[i]*vet2[i];
    }
    pthread_mutex_lock(&sb);
    soma = soma+redux;
    pthread_mutex_unlock(&sb);
}

int main(){
    long i, th, tmili;
    struct timeval start, end;
    pthread_t t[MAX_THREADS];
    pthread_mutex_init(&sb, NULL);

    vet1 = (double*)malloc(MAX_VET*sizeof(double));
    vet2 = (double*)malloc(MAX_VET*sizeof(double));

    for (i=0; i<MAX_VET; i++){
      vet1[i] = rand();
      vet2[i] = rand();
    }

    gettimeofday(&start, NULL);
    for (th=0; th<MAX_THREADS; th++) pthread_create(&t[th], NULL, calcReduceSoma, (void *) th);
    for (th=0; th<MAX_THREADS; th++) pthread_join(t[th], NULL); 
    printf("soma=%f\n", soma);
    gettimeofday(&end, NULL);
    tmili = (int) (1000*(end.tv_sec-start.tv_sec)+(end.tv_usec-start.tv_usec)/1000);
    printf("%d ms\n", tmili);
    return 0;
}