#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int readCube(char *fn,char *title,int *nXYZ,float *gOri, float *gVox,float **grid,int *gN) {
	FILE *io;
	char buffer[300];
	int i;
	float dum1,dum2;

	if((io=fopen(fn,"r"))==NULL) {
		printf("ERROR: file not found -> %s\n",fn);
		exit(1);
	}

	fgets(title,300,io);
	printf("%-20s : %s","input file title",title);
        fgets(buffer,300,io);
        fgets(buffer,300,io);
        sscanf(buffer,"%d %f %f %f",&i,&gOri[0],&gOri[1],&gOri[2]);
        gOri[0]*=0.5292;
        gOri[1]*=0.5292;
        gOri[2]*=0.5292;
        fgets(buffer,300,io);
        sscanf(buffer,"%d %f %f %f",&nXYZ[0],&gVox[0],&dum1,&dum2);
        gVox[0]*=0.5292;
        fgets(buffer,300,io);
        sscanf(buffer,"%d %f %f %f",&nXYZ[1],&dum1,&gVox[1],&dum2);
        gVox[1]*=0.5292;
        fgets(buffer,300,io);
        sscanf(buffer,"%d %f %f %f",&nXYZ[2],&dum1,&dum2,&gVox[2]);
        gVox[2]*=0.5292;
        fgets(buffer,300,io);

	gN[0]=nXYZ[0]*nXYZ[1]*nXYZ[2];
	grid[0]=(float*)malloc(gN[0]*sizeof(float));

	for(i=0;i<gN[0];i++) fscanf(io,"%f",&grid[0][i]);
	fclose(io);
	return 0;
}

int printVolumeData(char *fn,char *title,float *ori,float *vox,int nx,int ny,int nz,float *data) {
        FILE *cube;
	int a,b,c;
        char name[100];

        cube=fopen(fn,"w");
        fprintf(cube,"%s",title);
        fprintf(cube,"created by M. Heyden\n");
        fprintf(cube,"%5d%12.6f%12.6f%12.6f\n",1,ori[0]/0.5292,ori[1]/0.5292,ori[2]/0.5292);
        fprintf(cube,"%5d%12.6f%12.6f%12.6f\n",nx,vox[0]/0.5292,0.0,0.0);
        fprintf(cube,"%5d%12.6f%12.6f%12.6f\n",ny,0.0,vox[1]/0.5292,0.0);
        fprintf(cube,"%5d%12.6f%12.6f%12.6f\n",nz,0.0,0.0,vox[2]/0.5292);
        fprintf(cube,"%d%12.6f%12.6f%12.6f%12.6f\n",8,0.0,0.0,0.0,0.0);
	
        for(a=0;a<nx;a++) {
                for(b=0;b<ny;b++) {
                        for(c=0;c<nz;c++) {
                                fprintf(cube," %12.5e",(double)data[a*ny*nz+b*nz+c]);
                                if (c % 6 == 5) fprintf(cube,"\n");
                        }
                        fprintf(cube,"\n");
                }
        }
        fclose(cube);
}

int main(int argc,char *argv[]) {
	FILE *io;
	char fnInput[300],fnOutput[300];
	char title[300];
	int i,j,k,m;
	int nXYZ[3];
	float gOri[3];
	float gVox[3];
	float **gCrd;
	float *grid;
	int gN;
	float scale=1.0;
	int format;

	if(argc<2) {
		printf("ERROR: need input file name\n");
		printf("usage: ./convert-cube input.cube [output_filename] [scaling factor] [\"output_title\"]\n");
		exit(1);
	}
	strcpy(fnInput,argv[1]);
	printf("%-20s : %s\n","input cube file",fnInput);
	readCube(fnInput,title,nXYZ,gOri,gVox,&grid,&gN);
	if(argc>2) {
		if(sscanf(argv[2],"%s",fnOutput)!=1) {
			printf("ERROR: expected string but read '%s'\n",argv[2]);
			exit(1);
		}
		i=strlen(fnOutput);
		if(strncmp(&fnOutput[i-5],".cube",5)==0) {
			format=1;
			printf("%-20s : %s\n","output cube file",fnOutput);
		} else {
			format=0;
			printf("%-20s : %s\n","output data file",fnOutput);
		}
	} else {
		i=strlen(fnInput);
		if(strncmp(&fnInput[i-5],".cube",5)!=0) {
			printf("ERROR: expected inpute file extension \".cube\", but read %s\n",&fnInput[i-5]);
			exit(1);
		}
		strcpy(fnOutput,fnInput);
		sprintf(&fnOutput[i-5],"%s",".dat");
		printf("%-20s : %s\n","output data file",fnOutput);
		format=0;
	}
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
		sprintf(title,"%s\n",argv[4]);
        }
	printf("%-20s : %s","output file title",title);
	printf("%-20s : %f %f %f\n","grid origin (A)",gOri[0],gOri[1],gOri[2]);
	printf("%-20s : %f %f %f\n","voxel dimensions (A)",gVox[0],gVox[1],gVox[2]);

	gCrd=(float**)malloc(gN*sizeof(float*));
	for(i=0;i<gN;i++) {
		gCrd[i]=(float*)malloc(3*sizeof(float));
	}
	m=0;
	for(i=0;i<nXYZ[0];i++) {
		for(j=0;j<nXYZ[1];j++) {
			for(k=0;k<nXYZ[2];k++) {
				gCrd[m][0]=gOri[0]+i*gVox[0];
				gCrd[m][1]=gOri[1]+j*gVox[1];
				gCrd[m][2]=gOri[2]+k*gVox[2];
				m++;
			}
		}
	}

	if(scale!=1.0) {
		for(i=0;i<gN;i++) {
			grid[i]*=scale;
		}
		printf("message: multiplied all grid values with scaling factor %f\n",scale);
	}
	if(format==1) {
		printVolumeData(fnOutput,title,gOri,gVox,nXYZ[0],nXYZ[1],nXYZ[2],grid);
	} else {
		io=fopen(fnOutput,"w");
		fprintf(io,"%s",title);
		fprintf(io,"grid dimension: %4d %4d %4d\n",nXYZ[0],nXYZ[1],nXYZ[2]);
		fprintf(io,"voxel size (A): %f %f %f\n",gVox[0],gVox[1],gVox[2]);
		fprintf(io,"x (A)        y (A)        z (A)        (see title)\n");
		for(i=0;i<gN;i++) {
			fprintf(io,"%12.5e %12.5e %12.5e %12.5e\n",gCrd[i][0],gCrd[i][1],gCrd[i][2],grid[i]);
		}
		fclose(io);
	}

	return 0;
}
