#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/types.h"
#include "../include/get.h"
#include "../include/grids.h"
#include "../include/matvec.h"

int main(int argc,char *argv[]) {
    char fnCUBE[100];
    char fnOut[100];
    char title[300];
    char tmp[100];
    FILE *io;
    t_grid gA;
    int i,j,k,m;
    float scale;

    if(argc<4) {
        printf("usage: cubeScale A.cube scale output.cube [output_title]\n");
        printf(" output.cube = scale * A.cube\n");
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

    readCUBE(fnCUBE,&gA,1.0,0);

    for(i=0;i<gA.dim[0];i++) {
        for(j=0;j<gA.dim[1];j++) {
            for(k=0;k<gA.dim[2];k++) {
                gA.grid[i][j][k]*=scale;
            }
        }
    }
    if(argc>4) {
        setCUBEtitle(&gA,title);
    }
    writeCUBE(fnOut,gA,1.0,0);

    return 0;
}
