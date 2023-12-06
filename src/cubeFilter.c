#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/types.h"
#include "../include/grids.h"

int main(int argc,char *argv[]) {
    char fnCUBE[100];
    char fnOut[100];
    char tmp[100];
    FILE *io;
    t_grid g,gSmooth;
    int n;
    float sigma;

    if(argc<3) {
        printf("usage: ./smooth.exe input.cube sigma\n");
        printf(" input.cube: cube file to apply filter on\n");
        printf(" sigma     : sigma of Gaussian filter (Angstrom)\n");
        exit(1);
    }

    sscanf(argv[1],"%s",fnCUBE);
    sscanf(argv[2],"%f",&sigma);

    readCUBE(fnCUBE,&g,1.0,0);

    smoothGrid(g,sigma,&gSmooth);

    n=strlen(fnCUBE);
    strncpy(tmp,fnCUBE,n-5);
    tmp[n-5]=(char)0;
    sprintf(fnOut,"%s_f-%.2fA.cube",tmp,sigma);
    writeCUBE(fnOut,gSmooth,1.0,0);

    return 0;
}
