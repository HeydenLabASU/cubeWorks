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
    t_grid g;

    if(argc<3) {
        printf("ERROR: need input file name\n");
        printf("usage: ./convert-cube input.cube \"new_title\"\n");
        exit(1);
    }

    strcpy(fnInput,argv[1]);
    i=strlen(fnInput);
    if(strncmp(&fnInput[i-5],".cube",5)!=0) {
        printf("ERROR: expected inpute file extension \".cube\", but read %s\n",&fnInput[i-5]);
        exit(1);
    }
    printf("%-20s : %s\n","input cube file",fnInput);

    
    if(sscanf(argv[2],"%s",title)!=1) {
        printf("ERROR: expected string but read '%s'\n",argv[2]);
        exit(1);
    }
    sprintf(title,"%s",argv[2]);
    
    readCUBE(fnInput,&g,1.0,0);
    setCUBEtitle(&g,title);
    writeCUBE(fnInput,g,1.0,0);

    return 0;
}
