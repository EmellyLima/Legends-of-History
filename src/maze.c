#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "maze.h"

void generate_maze(int rows, int cols) {
    srand(time(NULL));
    printf("Gerando labirinto de %dx%d...\n", rows, cols);

    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            printf("%d ", rand() % 2);
        }
        printf("\n");
    }
}
