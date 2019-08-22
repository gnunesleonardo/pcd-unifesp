#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

#define SRAND_VALUE 1985
#define MAX_GRID 2048
#define MAX_GEN 2000
#define MAX_THREADS 8

int **grid, **newgrid, gen=0;
int MAX_REAL = MAX_GRID+2;
int DIV_GRID = MAX_GRID/MAX_THREADS;

int getNeighbors(int** grid, int i, int j){
    int neighbors = 0;
    if (grid[i-1][j-1]==1) neighbors+=1;
    if (grid[i-1][j]==1) neighbors+=1;
    if (grid[i-1][j+1]==1) neighbors+=1;
    if (grid[i][j-1]==1) neighbors+=1;
    if (grid[i][j+1]==1) neighbors+=1;
    if (grid[i+1][j-1]==1) neighbors+=1;
    if (grid[i+1][j]==1) neighbors+=1;
    if (grid[i+1][j+1]==1) neighbors+=1;
    return neighbors;
}

int lifeOrDeath(int life, int neighbors){
    switch (life){
        case 1:
            if (neighbors < 2) return 0;
            if (neighbors==2||neighbors==3) return 1;
            if (neighbors >= 4) return 0;
            break;
        case 0:
            if (neighbors == 3) return 1;
            else return 0;
            break;
        default:
            return -1;
    }
}

void swapValues(int **grid, int **newgrid){
    int i, j;
    for (i=1; i<=MAX_GRID; i++){
        newgrid[0][i] = grid[MAX_REAL-2][i]; // top
        newgrid[MAX_REAL-1][i] = grid[1][i]; // bottom
        newgrid[i][0] = grid[i][MAX_REAL-2]; // left
        newgrid[i][MAX_REAL-1] = grid[i][1]; // right
    }
    newgrid[0][0] = grid[MAX_REAL-2][MAX_REAL-2]; // left top corner
    newgrid[0][MAX_REAL-1] = grid[MAX_REAL-2][1]; // rigth top corner
    newgrid[MAX_REAL-1][0] = grid[1][MAX_REAL-2]; // left bottom corner
    newgrid[MAX_REAL-1][MAX_REAL-1] = grid[1][1]; // rigth bottom corner
}

void quantVivos(int **grid, int gen){
    int i, j, total=0;
    for(i=1; i<=MAX_GRID; i++){
        for (j=1; j<=MAX_GRID; j++){
            if (grid[i][j] == 1){
                total += 1;
            }
        }
    }
    printf("generation %d -> %d\n", gen, total);
}

void *calcNextGen(void *tid){
    int thid, inicial, final, neighbors, i, j;
    thid = (int) tid+1;

    if (thid==1) {
        inicial = 1;
        final = inicial*DIV_GRID;
    } else if(thid < MAX_THREADS) {
        inicial = ((thid-1)*DIV_GRID)+1;
        final = inicial+DIV_GRID;
    } else {
        inicial =  ((thid-1)*DIV_GRID)+1;
        final = MAX_GRID;
    }

    for (i=inicial; i<=final; i++){
        for (j=1; j<=MAX_GRID; j++){
            if (gen%2 == 0){
                neighbors = getNeighbors(grid, i, j);
                newgrid[i][j] = lifeOrDeath(grid[i][j], neighbors);
            } else {
                neighbors = getNeighbors(newgrid, i, j);
                grid[i][j] = lifeOrDeath(newgrid[i][j], neighbors);
            }
        }
    }
}

int main(){
    int i, j, life, th;
    srand(SRAND_VALUE);
    pthread_t t[MAX_THREADS];

    grid = (int**) malloc (MAX_REAL*sizeof(int*));
    newgrid = (int**) malloc (MAX_REAL*sizeof(int*));
    for (i=0; i<MAX_REAL; i++){ 
        grid[i] = (int*) malloc (MAX_REAL*sizeof(int));
        newgrid[i] = (int*) malloc (MAX_REAL*sizeof(int));
    }

    for(i=0; i<MAX_REAL; i++) {
        for (j=0; j<MAX_REAL; j++){
            if (i==0||j==0||i==MAX_REAL-1||j==MAX_REAL-1){
                grid[i][j] = 0;
                newgrid[i][j] = 0;
            } else {
                life = rand() % 2;
                grid[i][j] = life;
                newgrid[i][j] = life;
            }
        }
    }
    
    swapValues(newgrid, grid);
    while(gen < MAX_GEN){
        for (th=0; th<MAX_THREADS; th++) pthread_create(&t[th], NULL, calcNextGen, (void *) th);
        for (th=0; th<MAX_THREADS; th++) pthread_join(t[th], NULL); 
        if (gen%2 == 0) swapValues(grid, newgrid);
        else swapValues(newgrid, grid);
        gen += 1;
    }
    quantVivos(grid, gen);
    free(grid);
    free(newgrid);

    return 0;
}