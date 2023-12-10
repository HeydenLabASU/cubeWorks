#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/types.h"
#include "../include/get.h"
#include "../include/grids.h"
#include "../include/matvec.h"
#include "../include/cubeWorks.h"

int main(int argc,char *argv[]) {
    char fnCUBE[100];
    char fnOut[100];
    char title[300];
    char tmp[100];
    FILE *io;
    t_grid g;
    int i,j,k,m;
    float scale;

    printName("cubeScale");

    if(argc<4) {
        printf("usage: cubeScale input.cube scale output.cube [output_title]\n");
        printf(" output.cube = scale * input.cube\n\n");
        exit(1);
    }

    getString(argv[1],fnCUBE);
    printf("%-20s : %s\n","input file",fnCUBE);

    getFloat(argv[2],&scale);
    printf("%-20s : %f\n","scaling factor",scale);

    getString(argv[3],fnOut);
    printf("%-20s : %s\n","output file",fnOut);

    if(argc>4) {
        getString(argv[4],title);
        printf("%-20s : %s\n","output title",title);
    }

    readCUBE(fnCUBE,&g,1.0,0);

    for(i=0;i<g.dim[0];i++) {
        for(j=0;j<g.dim[1];j++) {
            for(k=0;k<g.dim[2];k++) {
                g.grid[i][j][k]*=scale;
            }
        }
    }
    if(argc>4) {
        setCUBEtitle(&g,title);
    }
    writeCUBE(fnOut,g,1.0,0);

    return 0;
}
