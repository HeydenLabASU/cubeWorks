#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/types.h"
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
    t_vec trans;

    if(argc<6) {
        printf("usage: ./cubeMove.exe A.cube dX dY dZ output.cube [output_title]\n");
        printf(" output.cube = A.cube + (dX dY dZ)\n");
        exit(1);
    }

    sscanf(argv[1],"%s",fnCUBE);
    readCUBE(fnCUBE,&gA,1.0,0);
    sscanf(argv[2],"%f",&trans[0]);
    sscanf(argv[3],"%f",&trans[1]);
    sscanf(argv[4],"%f",&trans[2]);
    sscanf(argv[5],"%s",fnOut);
    if(argc>6) {
        if(sscanf(argv[6],"%s",title)!=1) {
            printf("ERROR: expected string but read '%s'\n",argv[6]);
            exit(1);
        }
        sprintf(title,"%s",argv[6]);
    }

    vecAdd(gA.oriUHBD,trans,&gA.oriUHBD);
    vecAdd(gA.oriMH,trans,&gA.oriMH);
    vecAdd(gA.oriCUBE,trans,&gA.oriCUBE);
    for(i=0;gA.nAtoms;i++) {
        vecAdd(gA.atoms[i].crd,trans,&gA.atoms[i].crd);
    }
    
    if(argc>6) {
        setCUBEtitle(&gA,title);
    }
    writeCUBE(fnOut,gA,1.0,0);

    return 0;
}
