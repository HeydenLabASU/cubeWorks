#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/types.h"
#include "../include/get.h"
#include "../include/grids.h"
#include "../include/filter.h"
#include "../include/cubeWorks.h"

int main(int argc,char *argv[]) {
    char fnCUBE[100];
    char fnOut[100];
    char tmp[100];
    FILE *io;
    t_grid g,gSmooth;
    int n;
    float sigma;

    printName("cubeFilter");

    if(argc<3) {
        printf("usage: cubeFilter input.cube sigma\n");
        printf(" input.cube: cube file to apply filter on\n");
        printf(" sigma     : sigma of Gaussian filter (Angstrom)\n");
        exit(1);
    }

    getString(argv[1],fnCUBE);
    printf("%-20s : %s\n","input file",fnCUBE);

    getFloat(argv[2],&sigma);
    printf("%-20s : %f\n","sigma",sigma);

    n=strlen(fnCUBE);
    strncpy(tmp,fnCUBE,n-5);
    tmp[n-5]=(char)0;
    sprintf(fnOut,"%s_f-%.2fA.cube",tmp,sigma);
    printf("%-20s : %s\n","output file",fnOut);

    readCUBE(fnCUBE,&g,1.0,0);
    smoothGrid(g,sigma,&gSmooth);
    writeCUBE(fnOut,gSmooth,1.0,0);

    return 0;
}
