#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../../include/types.h"
#include "../../include/matvec.h"

/*read coordinates for molecule conformation*/
int readPDBcrd(char *fnPDB,t_conf *conf) {
	FILE *io;
	char buffer[300];
	char tmp[7];
	int i,n;
	float m;
	int el;
	
        io=fopen(fnPDB,"r");
	if(io==NULL) {
		printf("ERROR: grid file %s not found\n",fnPDB);
		exit(1);
	}
	printf("READING FILE %s\n",fnPDB);
	n=0;
	while(fgets(buffer,300,io)!=NULL) {
		if(strncmp(buffer,"ATOM  ",6)==0 || strncmp(buffer,"HETATM",6)==0) n++;
	}
	rewind(io);
	conf[0].nAtoms=n;
	if(n>5000) {
		printf("number of atoms in %s exceeds current limit (%d>5000)\n",fnPDB,n);
		printf("adapt t_conf datatype and function readPDBcrd\n");
		exit(1);
	}
	printf("found %d atom entries\n",n);
	i=0;
	conf[0].totMass=0.0;
	while(fgets(buffer,300,io)!=NULL) {
                if(strncmp(buffer,"ATOM  ",6)==0 || strncmp(buffer,"HETATM",6)==0) {
			strncpy(tmp,&buffer[11],6);
			tmp[6]=(char)0;
			sscanf(tmp,"%s",conf[0].atName[i]);
			el=0;
			while(strncmp(&conf[0].atName[i][el],"1",1)==0 
			|| strncmp(&conf[0].atName[i][el],"2",1)==0 
			|| strncmp(&conf[0].atName[i][el],"3",1)==0) {
				el++;
			}
			conf[0].elem[i]=conf[0].atName[i][el];
			strncpy(tmp,&buffer[17],4);
			tmp[4]=(char)0;
			sscanf(tmp,"%s",conf[0].resName[i]);
			strncpy(tmp,&buffer[22],4);
			tmp[4]=(char)0;
			sscanf(tmp,"%d",&conf[0].resID[i]);
			sscanf(&buffer[30],"%f %f %f",
				&conf[0].atCrd[i][0],
				&conf[0].atCrd[i][1],
				&conf[0].atCrd[i][2]);
			if(strncmp(&conf[0].elem[i],"H",1)==0) m=1.0079;
			else if(strncmp(&conf[0].elem[i],"C",1)==0) m=12.0110;
			else if(strncmp(&conf[0].elem[i],"N",1)==0) m=14.0067;
			else if(strncmp(&conf[0].elem[i],"O",1)==0) m=15.9994;
			else if(strncmp(&conf[0].elem[i],"S",1)==0) m=32.0600;
			else if(strncmp(&conf[0].elem[i],"P",1)==0) m=30.9740;
			else {
				printf("ERROR: unknown atom mass for %s\n",conf[0].atName[i]);
				exit(1);
			}
			conf[0].atMass[i]=m;
			conf[0].totMass+=m;
			i++;
		}
        }
        printf("read %d atoms with total mass %f from file %s\n",i,conf[0].totMass,fnPDB);
        fclose(io);
	return 0;
}

int writePDB(FILE *outPDB,t_conf conf,float box) {
	int atCnt,chCnt;
	int k,l;
	char ch[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	char name[10];

	chCnt=0;
	if(box!=0.0) {
		fprintf(outPDB,"CRYST1 %8.3f %8.3f %8.3f  90.00  90.00  90.00 P 1           1\n",box,box,box);
	}
	atCnt=1;
	for(l=0;l<conf.nAtoms;l++) {
		fprintf(outPDB,"ATOM  %5d",atCnt);
		strcpy(name,conf.atName[l]);
		if(strlen(name)==1) fprintf(outPDB,"  %s   ",name);
		if(strlen(name)==2) fprintf(outPDB,"  %s  ",name);
		if(strlen(name)==3) fprintf(outPDB,"  %s ",name);
		if(strlen(name)==4) fprintf(outPDB," %s ",name);
		if(strlen(name)==5) fprintf(outPDB," %s",name);
		if(strlen(name)==6) fprintf(outPDB,"%s",name);
		fprintf(outPDB,"%-4s%c%4d    %8.3f%8.3f%8.3f%6.2f%6.2f\n",
		        conf.resName[l],ch[chCnt],
		        conf.resID[l],
		        conf.atCrd[l][0],conf.atCrd[l][1],conf.atCrd[l][2],1.0,1.0);
		atCnt++;
		if(atCnt==100000) atCnt=1;
	}
	fflush(outPDB);

	return 0;
}
