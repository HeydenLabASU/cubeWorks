#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../include/types.h"
#include "../include/get.h"
#include "../include/grids.h"

int main(int argc,char *argv[]) {
    char fnCUBE[300],fnOut[300];
    char title[300];
    int i,j,k,m;
    float **gCrd;
    t_grid g;
    t_vec center;

    if(argc<3) {
        printf("usage: cubeCenter input.cube output.cube [output title]\n");
        exit(1);
    }

    getString(argv[1],fnCUBE);
    printf("%-20s : %s\n","input file",fnCUBE);
    getString(argv[2],fnOut);
    printf("%-20s : %s\n","output file",fnOut);

    if(argc>3) {
        getString(argv[3],title);
        printf("%-20s : %s\n","output title",title);
    }
    
    readCUBE(fnCUBE,&g,1.0,0);

    vecAdd3idx(g.oriMH,g.dim[0],g.a,g.dim[1],g.b,g.dim[2],g.c,&center);
    vecAdd(g.oriMH,center,&center);
    vecScale(0.5,center,&center);
    vecSub(g.oriUHBD,center,&g.oriUHBD);
    vecSub(g.oriMH,center,&g.oriMH);
    vecSub(g.oriCUBE,center,&g.oriCUBE);
    for(i=0;i<g.nAtoms;i++) {
        vecSub(g.atoms[i].crd,center,&g.atoms[i].crd);
    }

    if(argc>3) {
        setCUBEtitle(&g,title);
    }
    writeCUBE(fnOut,g,1.0,0);

    return 0;
}