#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../include/types.h"
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
        printf("ERROR: need input file name\n");
        printf("usage: ./convert-cube input.cube [output_filename] [scaling factor] [\"output_title\"]\n");
        exit(1);
    }

    strcpy(fnInput,argv[1]);
    i=strlen(fnInput);
    if(strncmp(&fnInput[i-5],".cube",5)!=0) {
        printf("ERROR: expected inpute file extension \".cube\", but read %s\n",&fnInput[i-5]);
        exit(1);
    }
    printf("%-20s : %s\n","input cube file",fnInput);

    if(argc>2) {
        if(sscanf(argv[2],"%s",fnOutput)!=1) {
            printf("ERROR: expected string but read '%s'\n",argv[2]);
            exit(1);
        }
    } else {
        strcpy(fnOutput,fnInput);
        sprintf(&fnOutput[i-5],"%s",".dat");
    }
    printf("%-20s : %s\n","output data file",fnOutput);

    if(argc>3) {
        if(sscanf(argv[3],"%f",&scale)!=1) {
            printf("ERROR: expected floating point number but read '%s'\n",argv[3]);
            exit(1);
        }
        printf("%-20s : %f\n","scaling factor",scale);
    }
    if(argc>4) {
        if(sscanf(argv[4],"%s",title)!=1) {
            printf("ERROR: expected string but read '%s'\n",argv[4]);
            exit(1);
        }
        sprintf(title,"%s",argv[4]);
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
