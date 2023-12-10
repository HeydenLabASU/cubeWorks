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
    t_vec axis;
    float angle;
    t_mat rot;

    printName("cubeRot");

    if(argc<7) {
        printf("usage: cubeRot A.cube axisX axisY axisZ angle output.cube [output_title]\n");
        printf(" output.cube = rot(axis,angle) @ input.cube\n\n");
        exit(1);
    }

    getString(argv[1],fnCUBE);
    printf("%-20s : %s\n","input file",fnCUBE);

    getFloat(argv[2],&axis[0]);
    getFloat(argv[3],&axis[1]);
    getFloat(argv[4],&axis[2]);
    printf("%-20s : %f %f %f\n","rotation axis",axis[0],axis[1],axis[2]);
    
    getFloat(argv[5],&angle);
    printf("%-20s : %f\n","rotation angle",angle);
    angle=angle/360.0*2.0*3.14159265359;

    getString(argv[6],fnOut);
    printf("%-20s : %s\n","output file",fnOut);

    if(argc>7) {
        getString(argv[7],title);
        printf("%-20s : %s\n","output title",title);
    }

    readCUBE(fnCUBE,&g,1.0,0);

    rotAny(angle,axis,&rot);
    printf("%f %f %f\n",rot[0][0],rot[0][1],rot[0][2]);
    printf("%f %f %f\n",rot[1][0],rot[1][1],rot[1][2]);
    printf("%f %f %f\n",rot[2][0],rot[2][1],rot[2][2]);
    matvec(rot,g.oriUHBD,&g.oriUHBD);
    matvec(rot,g.oriMH,&g.oriMH);
    matvec(rot,g.oriCUBE,&g.oriCUBE);
    matvec(rot,g.a,&g.a);
    matvec(rot,g.b,&g.b);
    matvec(rot,g.c,&g.c);
    for(i=0;i<g.nAtoms;i++) {
        matvec(rot,g.atoms[i].crd,&g.atoms[i].crd);
    }
    
    if(argc>7) {
        setCUBEtitle(&g,title);
    }
    writeCUBE(fnOut,g,1.0,0);

    return 0;
}
