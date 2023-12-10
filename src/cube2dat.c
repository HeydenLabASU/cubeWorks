#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../include/types.h"
#include "../include/get.h"
#include "../include/matvec.h"
#include "../include/grids.h"
#include "../include/cubeWorks.h"

int main(int argc,char *argv[]) {
    FILE *io;
    char fnInput[300],fnOutput[300];
    char title[300];
    int i,j,k,m;
    float **gCrd;
    float scale=1.0;
    t_grid g;

    printName("cube2dat");

    if(argc<2) {
        printf("usage: cube2dat input.cube [output_filename] [scaling factor] [\"output_title\"]\n\n");
        exit(1);
    }

    getString(argv[1],fnInput);
    printf("%-20s : %s\n","input file",fnInput);

    if(argc>2) {
        getString(argv[2],fnOutput);
    } else {
        strcpy(fnOutput,fnInput);
        sprintf(&fnOutput[i-5],"%s",".dat");
    }
    printf("%-20s : %s\n","output file",fnOutput);

    if(argc>3) {
        getFloat(argv[3],&scale);
        printf("%-20s : %f\n","scaling factor",scale);
    }
    if(argc>4) {
        getString(argv[4],title);
    }

    readCUBE(fnInput,&g,scale,0);

    if(argc>4) {
        setCUBEtitle(&g,title);
    }

    gCrd=(float**)malloc(g.nVoxel*sizeof(float*));
    for(i=0;i<g.nVoxel;i++) {
        gCrd[i]=(float*)malloc(3*sizeof(float));
    }
    m=0;
    for(i=0;i<g.dim[0];i++) {
        for(j=0;j<g.dim[1];j++) {
            for(k=0;k<g.dim[2];k++) {
                vecAdd3idx(g.oriCUBE,i,g.a,j,g.b,k,g.c,&gCrd[m]);
                m++;
            }
        }
    }
    
    io=fopen(fnOutput,"w");
    fprintf(io,"#%s\n",title);
    fprintf(io,"#grid dimension: %4d %4d %4d\n",g.dim[0],g.dim[1],g.dim[2]);
    fprintf(io,"#x (A)       y (A)        z (A)        (see title)\n");
    i=0;
    j=0;
    k=0;
    for(m=0;m<g.nVoxel;m++) {
        fprintf(io,"%12.5e %12.5e %12.5e %12.5e\n",gCrd[m][0],gCrd[m][1],gCrd[m][2],g.grid[i][j][k]);
        k++;
        if(k==g.dim[2]) {
            k=0;
            j++;
        }
        if(j==g.dim[1]) {
            j=0;
            i++;
        }
    }
    fclose(io);

    return 0;
}
