#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/types.h"
#include "../include/grids.h"
#include "../include/matvec.h"

int main(int argc,char *argv[]) {
    char fnCUBE[100];
    char fnOut[100];
    char tmp[100];
    FILE *io;
    t_grid gA;
    int i,j,k,m;
    float scale;

    if(argc<4) {
        printf("usage: ./cubeScaleInv.exe A.cube scale output.cube\n");
        printf(" output.cube = A.cube / scale\n");
        exit(1);
    }

    sscanf(argv[1],"%s",fnCUBE);
    readCUBE(fnCUBE,&gA,1.0,0);

    sscanf(argv[2],"%f",&scale);
    if(scale==0.0) {
        printf("refuse to divide by zero\n");
        exit(1);
    }
    sscanf(argv[3],"%s",fnOut);

    for(i=0;i<gA.dim[0];i++) {
        for(j=0;j<gA.dim[1];j++) {
            for(k=0;k<gA.dim[2];k++) {
                gA.grid[i][j][k]/=scale;
            }
        }
    }
    writeCUBE(fnOut,gA,1.0,0);

    return 0;
}
