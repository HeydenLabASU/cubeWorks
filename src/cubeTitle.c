#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../include/types.h"
#include "../include/get.h"
#include "../include/grids.h"

int main(int argc,char *argv[]) {
    FILE *io;
    char fnCUBE[300];
    char title[300];
    int i,j,k,m;
    float **gCrd;
    t_grid g;

    if(argc<3) {
        printf("usage: ./cubeTitle.exe input.cube \"new_title\"\n");
        exit(1);
    }

    getString(argv[1],fnCUBE);
    printf("%-20s : %s\n","input file",fnCUBE);

    getString(argv[2],title);
    printf("%-20s : %s\n","output file",title);
    
    readCUBE(fnCUBE,&g,1.0,0);
    setCUBEtitle(&g,title);
    writeCUBE(fnCUBE,g,1.0,0);

    return 0;
}
