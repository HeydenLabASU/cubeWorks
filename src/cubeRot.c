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
    t_vec axis;
    float angle;
    t_mat rot;

    if(argc<7) {
        printf("usage: cubeRot A.cube axisX axisY axisZ angle output.cube [output_title]\n");
        printf(" output.cube = rot(axis,angle) @ A.cube\n");
        exit(1);
    }

    getString(argv[1],fnCUBE);
    printf("%-20s : %s\n","input file A",fnCUBE);

    getFloat(argv[2],&axis[0]);
    getFloat(argv[3],&axis[1]);
    getFloat(argv[4],&axis[2]);
    printf("%-20s : %f %f %f\n","rotation axis",axis[0],axis[1],axis[2]);
    
    getFloat(argv[5],&angle);
    printf("%-20s : %f\n","rotation angle",angle);

    getString(argv[6],fnOut);
    printf("%-20s : %s\n","output file",fnOut);

    if(argc>7) {
        getString(argv[7],title);
        printf("%-20s : %s\n","output title",title);
    }

    readCUBE(fnCUBE,&gA,1.0,0);

    rotAny(angle,axis,&rot);
    matvec(rot,gA.oriUHBD,&gA.oriUHBD);
    matvec(rot,gA.oriMH,&gA.oriMH);
    matvec(rot,gA.oriCUBE,&gA.oriCUBE);
    matvec(rot,gA.a,&gA.a);
    matvec(rot,gA.b,&gA.b);
    matvec(rot,gA.c,&gA.c);
    for(i=0;gA.nAtoms;i++) {
        matvec(rot,gA.atoms[i].crd,&gA.atoms[i].crd);
    }
    
    if(argc>7) {
        setCUBEtitle(&gA,title);
    }
    writeCUBE(fnOut,gA,1.0,0);

    return 0;
}
