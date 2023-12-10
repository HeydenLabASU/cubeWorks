#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/types.h"
#include "../include/get.h"
#include "../include/grids.h"
#include "../include/cubeWorks.h"

int main(int argc,char *argv[]) {
    char fnCUBE[100];
    char fnOut[100];
    char title[300];
    char tmp[100];
    FILE *io;
    t_grid gA,gB,gOut;
    int i,j,k,m;

    printName("cubeAdd");

    if(argc<4) {
        printf("usage: cubeAdd A.cube B.cube output.cube [output_title]\n");
        printf(" output.cube = A.cube + B.cube\n");
        exit(1);
    }

    getString(argv[1],fnCUBE);
    printf("%-20s : %s\n","input file A",fnCUBE);
    readCUBE(fnCUBE,&gA,1.0,0);

    getString(argv[2],fnCUBE);
    printf("%-20s : %s\n","input file B",fnCUBE);
    readCUBE(fnCUBE,&gB,1.0,0);

    getString(argv[3],fnOut);
    printf("%-20s : %s\n","output file",fnOut);

    if(argc>4) {
        getString(argv[4],title);
        printf("%-20s : %s\n","output title",title);
    }

    if(eqCUBEformat(gA,gB)!=1) {
        printf("ERROR: incompatible grid formats:\n");
        printf(" %s\n",argv[1]);
        printf(" %s\n",argv[2]);
        exit(1);
    } else {
        cpyCUBEformat(gA,&gOut);
    }

    allocGrd(&gOut);
    for(i=0;i<gOut.dim[0];i++) {
        for(j=0;j<gOut.dim[1];j++) {
            for(k=0;k<gOut.dim[2];k++) {
                gOut.grid[i][j][k]=gA.grid[i][j][k]+gB.grid[i][j][k];
            }
        }
    }
    if(argc>4) {
        setCUBEtitle(&gOut,title);
    }
    writeCUBE(fnOut,gOut,1.0,0);

    return 0;
}
