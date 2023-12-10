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
    int i,j,k,l;
    char plane[3];
    double aver;

    printName("cubeMirrpr");

    if(argc<4) {
        printf("usage: cubeScale input.cube plane output.cube [output_title]\n");
        printf(" output.cube = scale * A.cube\n");
        exit(1);
    }

    getString(argv[1],fnCUBE);
    printf("%-20s : %s\n","input file",fnCUBE);

    if(strlen(argv[2])>2 || (
                             strncmp(argv[2],"XY",2)!=0 &&
                             strncmp(argv[2],"XZ",2)!=0 &&
                             strncmp(argv[2],"YZ",2)!=0
                            )
    ) {
        printf("ERROR: %s is neither XY,XZ,YZ\n",argv[2]);
        exit(1);
    }
    getString(argv[2],plane);
    printf("%-20s : %f\n","mirror plane",plane);

    getString(argv[3],fnOut);
    printf("%-20s : %s\n","output file",fnOut);

    if(argc>4) {
        getString(argv[4],title);
        printf("%-20s : %s\n","output title",title);
    }

    readCUBE(fnCUBE,&g,1.0,0);

    if(strncmp(plane,"XY",2)==0) {
        for(i=0;i<g.dim[0];i++) {
            for(j=0;j<g.dim[1];j++) {
                for(k=0;k<g.dim[2]/2;k++) {
                    l=g.dim[2]-k-1;
                    aver=(g.grid[i][j][k]+g.grid[i][j][l])/2.0;
                    gA.grid[i][j][k]=aver;
                    gA.grid[i][j][l]=aver;
                }
            }
        }
    } else if(strncmp(plane,"XZ",2)==0) {
        for(i=0;i<g.dim[0];i++) {
            for(j=0;j<g.dim[1];j++) {
                l=g.dim[1]-j-1;
                for(k=0;k<g.dim[2]/2;k++) {
                    aver=(g.grid[i][j][k]+g.grid[i][l][k])/2.0;
                    gA.grid[i][j][k]=aver;
                    gA.grid[i][l][k]=aver;
                }
            }
        }
    } else if(strncmp(plane,"YZ",2)==0) {
        for(i=0;i<g.dim[0];i++) {
            l=g.dim[0]-i-1;
            for(j=0;j<g.dim[1];j++) {
                l=g.dim[1]-j-1;
                for(k=0;k<g.dim[2]/2;k++) {
                    aver=(g.grid[i][j][k]+g.grid[l][j][k])/2.0;
                    gA.grid[i][j][k]=aver;
                    gA.grid[l][j][k]=aver;
                }
            }
        }
    }
    if(argc>4) {
        setCUBEtitle(&g,title);
    }
    writeCUBE(fnOut,g,1.0,0);

    return 0;
}
