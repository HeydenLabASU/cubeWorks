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
    t_vec trans;

    printName("cubeMove");

    if(argc<6) {
        printf("usage: cubeMove input.cube dX dY dZ output.cube [output_title]\n");
        printf(" output.cube = input.cube + (dX dY dZ)\n");
        exit(1);
    }

    getString(argv[1],fnCUBE);
    printf("%-20s : %s\n","input file",fnCUBE);

    getFloat(argv[2],&trans[0]);
    getFloat(argv[3],&trans[1]);
    getFloat(argv[4],&trans[2]);
    printf("%-20s : %f %f %f\n","vector",trans[0],trans[1],trans[2]);

    getString(argv[5],fnOut);
    printf("%-20s : %s\n","output file",fnOut);

    if(argc>6) {
        getString(argv[6],title);
        printf("%-20s : %s\n","output title",title);
    }

    readCUBE(fnCUBE,&g,1.0,0);
    vecAdd(g.oriUHBD,trans,&g.oriUHBD);
    vecAdd(g.oriMH,trans,&g.oriMH);
    vecAdd(g.oriCUBE,trans,&g.oriCUBE);
    for(i=0;g.nAtoms;i++) {
        vecAdd(g.atoms[i].crd,trans,&g.atoms[i].crd);
    }
        if(argc>6) {
        setCUBEtitle(&g,title);
    }
    writeCUBE(fnOut,g,1.0,0);

    return 0;
}
