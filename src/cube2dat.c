#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../include/types.h"
#include "../include/get.h"
#include "../include/grids.h"

int main(int argc,char *argv[]) {
    FILE *io;
    char fnInput[300],fnOutput[300];
    char title[300];
    int i,j,k,m;
    float **gCrd;
    float scale=1.0;
    t_grid g;

    if(argc<2) {
        printf("usage: ./convert-cube input.cube [output_filename] [scaling factor] [\"output_title\"]\n");
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
    if(g.aligned!=1 || g.orthorhombic!=1) {
        printf("ERROR: grid with non-orthorhombic/non-aligned voxels\n");
        printf("ERROR: cannot write in simple data format\n");
        exit(1);
    }
    if(argc>4) {
        setCUBEtitle(&g,title);
    }

    printf("%-20s : %s\n","output file title",g.title);
    printf("%-20s : %f %f %f\n","grid origin (A)",g.oriCUBE[0],g.oriCUBE[1],g.oriCUBE[2]);
    printf("%-20s : %f %f %f\n","voxel dimensions (A)",g.a[0],g.b[1],g.c[2]);

    gCrd=(float**)malloc(g.nVoxel*sizeof(float*));
    for(i=0;i<g.nVoxel;i++) {
        gCrd[i]=(float*)malloc(3*sizeof(float));
    }
    m=0;
    for(i=0;i<g.dim[0];i++) {
        for(j=0;j<g.dim[1];j++) {
            for(k=0;k<g.dim[2];k++) {
                gCrd[m][0]=g.oriCUBE[0]+i*g.a[0];
                gCrd[m][1]=g.oriCUBE[1]+j*g.b[1];
                gCrd[m][2]=g.oriCUBE[2]+k*g.c[2];
                m++;
            }
        }
    }
    
    io=fopen(fnOutput,"w");
    fprintf(io,"#%s\n",title);
    fprintf(io,"#grid dimension: %4d %4d %4d\n",g.dim[0],g.dim[1],g.dim[2]);
    fprintf(io,"#voxel size (A): %f %f %f\n",g.a[0],g.b[1],g.c[2]);
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
