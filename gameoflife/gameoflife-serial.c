#include <stdio.h>
#include <stdlib.h>

#define SRAND_VALUE 1985
#define MAXGRID 500
#define MAXGEN 10
int MAXREAL = MAXGRID+2;

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
    return -1;
}

void swapValues(int **grid, int **newgrid){
    int i;
    for (i=1; i<=MAXGRID; i++){
        newgrid[0][i] = grid[MAXREAL-2][i]; // top
        newgrid[MAXREAL-1][i] = grid[1][i]; // bottom
        newgrid[i][0] = grid[i][MAXREAL-2]; // left
        newgrid[i][MAXREAL-1] = grid[i][1]; // right
    }
    newgrid[0][0] = grid[MAXREAL-2][MAXREAL-2]; // left top corner
    newgrid[0][MAXREAL-1] = grid[MAXREAL-2][1]; // rigth top corner
    newgrid[MAXREAL-1][0] = grid[1][MAXREAL-2]; // left bottom corner
    newgrid[MAXREAL-1][MAXREAL-1] = grid[1][1]; // rigth bottom corner
}

void quantVivos(int **grid, int gen){
    int i, j, total=0;
    for(i=1; i<=MAXGRID; i++){
        for (j=1; j<=MAXGRID; j++){
            if (grid[i][j] == 1){
                total += 1;
            }
        }
    }
    printf("generation %d -> %d\n", gen, total);
}

int main(){
    int i, j, neighbors, life;
    int gen=0, control=0;
    int **grid, **newgrid;
    srand(SRAND_VALUE);

    grid = (int**) malloc (MAXREAL*sizeof(int*));
    newgrid = (int**) malloc (MAXREAL*sizeof(int*));
    for (i=0; i<MAXREAL; i++){ 
        grid[i] = (int*) malloc (MAXREAL*sizeof(int));
        newgrid[i] = (int*) malloc (MAXREAL*sizeof(int));
    }

    for(i=0; i<MAXREAL; i++) {
        for (j=0; j<MAXREAL; j++){
            if (i==0||j==0||i==MAXREAL-1||j==MAXREAL-1){
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

    while(gen < MAXGEN){
        control = gen%2;
        if (control == 0) quantVivos(grid, gen);
        else quantVivos(newgrid, gen);
        for(i=1; i<=MAXGRID; i++) {
            for (j=1; j<=MAXGRID; j++){
                if (control == 0){
                    neighbors = getNeighbors(grid, i, j);
                    newgrid[i][j] = lifeOrDeath(grid[i][j], neighbors);
                    swapValues(grid, newgrid);
                } else {
                    neighbors = getNeighbors(newgrid, i, j);
                    grid[i][j] = lifeOrDeath(newgrid[i][j], neighbors);
                    swapValues(newgrid, grid);
                }
            }
        }
        gen += 1;
    }

    free(grid);
    free(newgrid);
    return 0;
}