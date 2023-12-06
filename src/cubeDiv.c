#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/types.h"
#include "../include/grids.h"
#include "../include/matvec.h"

int main(int argc,char *argv[]) {
    char fnCUBE[100];
    char fnOut[100];
    char tmp[100];
    FILE *io;
    t_grid gA,gB,gOut;
    int equal;
    int i,j,k,m;
    float esc;
    int flag=0;

    if(argc<4) {
        printf("usage: ./cubeDiv.exe A.cube B.cube output.cube [escape]\n");
        printf(" output.cube = A.cube / B.cube\n");
        printf(" escape: result division by zero\n");
        exit(1);
    }

    sscanf(argv[1],"%s",fnCUBE);
    readCUBE(fnCUBE,&gA,1.0,0);
    sscanf(argv[2],"%s",fnCUBE);
    readCUBE(fnCUBE,&gB,1.0,0);
    sscanf(argv[3],"%s",fnOut);
    if(argc>4) {
        sscanf(argv[4],"%f",&esc);
    } else esc=0.0;

    if(eqCUBEformat(gA,gB)!=1) {
        printf("ERROR: incompatible grid formats:\n");
        printf(" %s\n",argv[1]);
        printf(" %s\n",argv[2]);
        exit(1);
    } else {
        cpyCUBEformat(gA,&gOut);
    }

    allocGrd(&gOut);
    for(i=0;i<gOut.dim[0];i++) {
        for(j=0;j<gOut.dim[1];j++) {
            for(k=0;k<gOut.dim[2];k++) {
                if(gB.grid[i][j][k]!=0.0) {
                    gOut.grid[i][j][k]=gA.grid[i][j][k]/gB.grid[i][j][k];
                } else {
                    gOut.grid[i][j][k]=esc;
                    if(flag==0) {
                        flag=1;
                        printf("NOTE: division by zero encountered\n");
                        printf("      result set to: %f\n,esc");
                    }
                }
            }
        }
    }
    writeCUBE(fnOut,gOut,1.0,0);

    return 0;
}
