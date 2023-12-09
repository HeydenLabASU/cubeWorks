#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../include/types.h"
#include "../include/get.h"
#include "../include/grids.h"

int main(int argc,char *argv[]) {
    char fnCUBE[300];
    int i,j,k,m;
    float **gCrd;
    t_grid g;

    if(argc<2) {
        printf("usage: cubeDelAtoms input.cube\n");
        exit(1);
    }

    getString(argv[1],fnCUBE);
    printf("%-20s : %s\n","input file",fnCUBE);
    
    readCUBE(fnCUBE,&g,1.0,0);
    g.nAtoms=0;
    writeCUBE(fnCUBE,g,1.0,0);

    return 0;
}
