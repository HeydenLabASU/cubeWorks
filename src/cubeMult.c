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

	if(argc<4) {
		printf("usage: ./cubeMult.exe A.cube B.cube output.cube\n");
		printf(" output.cube = A.cube * B.cube\n");
		exit(1);
	}

	sscanf(argv[1],"%s",fnCUBE);
	readCUBE(fnCUBE,&gA,1.0,0);
	printf("read %d voxels\n",gA.nVoxel);
	sscanf(argv[2],"%s",fnCUBE);
	readCUBE(fnCUBE,&gB,1.0,0);
	sscanf(argv[3],"%s",fnOut);

	equal=1;
	if(gA.nVoxel!=gB.nVoxel) equal=0;
	else gOut.nVoxel=gA.nVoxel;
	if(gA.dg!=gB.dg) equal=0;
	else gOut.dg=gA.dg;
	for(m=0;m<3;m++) {
		if(gA.dim[m]!=gB.dim[m]) equal=0;
		else gOut.dim[m]=gA.dim[m];
		if(gA.oriCUBE[m]!=gB.oriCUBE[m]) equal=0;
		else {
			gOut.oriUHBD[m]=gA.oriUHBD[m];
			gOut.oriMH[m]=gA.oriMH[m];
			gOut.oriCUBE[m]=gA.oriCUBE[m];
		}
	}
	if(equal!=1) {
		printf("incompatible grid formats:\n");
		printf(" %s\n",argv[1]);
		printf(" %s\n",argv[2]);
		exit(1);
	} else {
		strcpy(gOut.title,gA.title);
	}
	allocGrd(&gOut);
	for(i=0;i<gOut.dim[0];i++) {
		for(j=0;j<gOut.dim[1];j++) {
			for(k=0;k<gOut.dim[2];k++) {
				gOut.grid[i][j][k]=gA.grid[i][j][k]*gB.grid[i][j][k];
			}
		}
	}
	writeCUBE(fnOut,gOut,1.0,0);

	return 0;
}
