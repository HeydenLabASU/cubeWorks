#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/types.h"
#include "../include/matvec.h"
#include "../include/grids.h"
#include "../include/qsort.h"

void initProgress(int end,int *cnt,int *maxCnt,int *lastPercent) {
    cnt[0]=1;
    maxCnt[0]=end;
    printf("0%%"); fflush(stdout);
    lastPercent[0]=0;
}

void reportProgress(int cnt,int maxCnt,int *lastPercent) {
    double fraction;
    int percent;
    int last;

    last=lastPercent[0];

    fraction=((double)cnt)/((double)maxCnt);
    percent=(int)(fraction*40);
    if(percent>last) {
        lastPercent[0]=percent;
        last=percent;
        if(last%4==0) {
            printf("%d%%",10*last/4); fflush(stdout);
        } else {
            printf("."); fflush(stdout);
        }
    }
}

int testGridLimits(int *idx,int gLimits[][2]) {
    int ix,iy,iz;

    ix=idx[0];
    iy=idx[1];
    iz=idx[2];
    if(ix>=gLimits[0][0] &&
       ix<=gLimits[0][1] &&
       iy>=gLimits[1][0] &&
       iy<=gLimits[1][1] &&
       iz>=gLimits[2][0] &&
       iz<=gLimits[2][1]
    ) {
        return 1;
    } else {
        return 0;
    }
}

int findGridMaxOneStep(t_grid g,int *currentIdx,int gLimits[][2],int displ[][3],int *nextIdx) {
    int trials3D[27][3];
    int nTrials;
    int i,j;
    int ix,iy,iz;
    int ixm,iym,izm;
    float max;
    int test;

    nTrials=0;
    for(i=0;i<27;i++) {
        for(j=0;j<3;j++) {
            trials3D[nTrials][j]=currentIdx[j]+displ[i][j];
        }
        test=testGridLimits(trials3D[nTrials],gLimits);
        ix=trials3D[nTrials][0];
        iy=trials3D[nTrials][1];
        iz=trials3D[nTrials][2];
        if(test==1) {
            nTrials++;
        }
    }
    i=0;
    j=0;
    ixm=trials3D[i][0];
    iym=trials3D[i][1];
    izm=trials3D[i][2];
    max=g.grid[ixm][iym][izm];
    for(i=1;i<nTrials;i++) {
        ix=trials3D[i][0];
        iy=trials3D[i][1];
        iz=trials3D[i][2];
        if(g.grid[ix][iy][iz]==max) {
            if(ix>ixm) {
                max=g.grid[ix][iy][iz];
                ixm=ix;
                iym=iy;
                izm=iz;
                j=i;
            } else if(ix==ixm) {
                if(iy>iym) {
                    max=g.grid[ix][iy][iz];
                    ixm=ix;
                    iym=iy;
                    izm=iz;
                    j=i;
                } else if(iy==iym) {
                    if(iz>izm) {
                        max=g.grid[ix][iy][iz];
                        ixm=ix;
                        iym=iy;
                        izm=iz;
                        j=i;
                    }
                }
            }
        } else if(g.grid[ix][iy][iz]>max) {              
            max=g.grid[ix][iy][iz];
            ixm=ix;
            iym=iy;
            izm=iz;
            j=i;
        }
    }
    ix=trials3D[j][0];
    iy=trials3D[j][1];
    iz=trials3D[j][2];
    nextIdx[0]=ix;
    nextIdx[1]=iy;
    nextIdx[2]=iz;
    return 0;
}

int findGridMax(t_grid g,int ***idx3Dto1D,int *startIdx,int gLimits[][2],int *gTraces,int displ[][3]) {
    int currentIdx[3];
    int nextIdx[3];
    int trace=0;
    int test;
    int ix,iy,iz,i1;
    int i;
    
    ix=startIdx[0];
        iy=startIdx[1];
        iz=startIdx[2];
    test=testGridLimits(startIdx,gLimits);
    if(test==1) {
        i1=idx3Dto1D[ix][iy][iz];
        trace=gTraces[i1];
        if(trace==0) {
            gTraces[i1]=-1;
            currentIdx[0]=ix;
            currentIdx[1]=iy;
            currentIdx[2]=iz;
            while(trace==0) {
                findGridMaxOneStep(g,currentIdx,gLimits,displ,nextIdx);
                ix=nextIdx[0];
                iy=nextIdx[1];
                iz=nextIdx[2];
                i1=idx3Dto1D[ix][iy][iz];
                trace=gTraces[i1];
                if(trace==0) {
                    gTraces[i1]=-1;
                    currentIdx[0]=ix;
                    currentIdx[1]=iy;
                    currentIdx[2]=iz;
                } else if(trace==-1) {
                    /*reached maximum, implies last step did not move*/
                    /*printf("reached maximum: idx = %d\n",i1);*/
                    for(i=0;i<g.nVoxel;i++) {
                        if(gTraces[i]==-1) {
                            gTraces[i]=i1;
                        }
                    }
                } else {
                    /*crossed path with previous search*/
                    /*printf("crossed path with previous search: idx = %d\n",trace);*/
                    for(i=0;i<g.nVoxel;i++) {
                        if(gTraces[i]==-1) {
                            gTraces[i]=trace;
                        }
                    }
                }
            }
        }
    } else {
        printf("starting point %d %d %d out of bounds\n",ix,iy,iz);
        exit(1);
    }
    return 0;
}

int main(int argc,char *argv[]) {
    char fnCUBE[100];
    char fnOut[100];
    char tmp[100];
    FILE *io;
    t_grid g;
    int *gTraces;
    int *sorted;
    int i,j,k,l,m,n;
    int maxCnt,lastPercent;
    int displ[27][3];
    int ***idx3Dto1D;
    int **idx1Dto3D;
    int gLimits[3][2];
    int nMax;
    int startIdx[3];
    int *tmpCnts,*cnts;
    double *tmpSums,*sums;
    t_vec *gridCrd;
    int maxMembers;
    int **members;
    int *memberCnt;
    int *pos;

    if(argc<2) {
        printf("usage: ./findMax.exe input.cube\n");
        printf(" input.cube: cube file with density\n");
        exit(1);
    }

    sscanf(argv[1],"%s",fnCUBE);

    readCUBE(fnCUBE,&g,1.0,0);
    
    gTraces=(int*)malloc(g.nVoxel*sizeof(int));
    for(i=0;i<g.nVoxel;i++) {
        gTraces[i]=0;
    }
    sorted=(int*)malloc(g.nVoxel*sizeof(int));
    l=0;
    for(i=-1;i<=1;i++) {
        for(j=-1;j<=1;j++) {
            for(k=-1;k<=1;k++) {
                displ[l][0]=i;
                displ[l][1]=j;
                displ[l][2]=k;
                l++;
            }
        }
    }

    idx1Dto3D=(int**)malloc(g.nVoxel*sizeof(int*));
        for(i=0;i<g.nVoxel;i++) {
                idx1Dto3D[i]=(int*)malloc(3*sizeof(int));
        }
    l=0;
    idx3Dto1D=(int***)malloc(g.dim[0]*sizeof(int**));
    for(i=0;i<g.dim[0];i++) {
        idx3Dto1D[i]=(int**)malloc(g.dim[1]*sizeof(int*));
        for(j=0;j<g.dim[1];j++) {
            idx3Dto1D[i][j]=(int*)malloc(g.dim[2]*sizeof(int));
            for(k=0;k<g.dim[2];k++) {
                idx3Dto1D[i][j][k]=l;
                idx1Dto3D[l][0]=i;
                idx1Dto3D[l][1]=j;
                idx1Dto3D[l][2]=k;
                l++;
            }
        }
    }

    gLimits[0][0]=0;
    gLimits[0][1]=g.dim[0]-1;
    gLimits[1][0]=0;
    gLimits[1][1]=g.dim[1]-1;
    gLimits[2][0]=0;
    gLimits[2][1]=g.dim[2]-1;

    l=0;
    initProgress(g.nVoxel,&l,&maxCnt,&lastPercent);
    for(i=0;i<g.dim[0];i++) {
        for(j=0;j<g.dim[1];j++) {
            for(k=0;k<g.dim[2];k++) {
                startIdx[0]=i;
                startIdx[1]=j;
                startIdx[2]=k;
                findGridMax(g,idx3Dto1D,startIdx,gLimits,gTraces,displ);
                l++;
                reportProgress(l,maxCnt,&lastPercent);
            }
        }
    }
    printf("\n"); fflush(stdout);

    for(i=0;i<g.nVoxel;i++) {
        sorted[i]=gTraces[i];
    }

    /*equivalent of 'Union' in Mathematica*/
    qsort(sorted,g.nVoxel,sizeof(int),int_cmp);
    j=sorted[0];
    nMax=1;
    for(i=1;i<g.nVoxel;i++) {
        if(sorted[i]>j) {
            j=sorted[i];
            sorted[nMax]=j;
            nMax++;
        }
    }
    /*end*/

    tmpCnts=(int*)malloc(g.nVoxel*sizeof(int));
    cnts=(int*)malloc(nMax*sizeof(int));
    tmpSums=(double*)malloc(g.nVoxel*sizeof(double));
    sums=(double*)malloc(nMax*sizeof(double));
    for(i=0;i<g.nVoxel;i++) {
        tmpCnts[i]=0;
        tmpSums[i]=0.0;
    }
    for(l=0;l<g.nVoxel;l++) {
        tmpCnts[gTraces[l]]++;
        i=idx1Dto3D[l][0];
        j=idx1Dto3D[l][1];
        k=idx1Dto3D[l][2];
        tmpSums[gTraces[l]]+=g.grid[i][j][k];
    }
    printf("detected %d maxima\n",nMax);
    for(i=0;i<nMax;i++) {
        j=sorted[i];
        cnts[i]=tmpCnts[j];
        sums[i]=tmpSums[j];
    }
    maxMembers=0;
    for(i=0;i<nMax;i++) {
        if(cnts[i]>maxMembers) {
            maxMembers=cnts[i];
        }
    }
    /*maxMembers is the largest number of voxel members for any density maximum*/
    members=(int**)malloc(nMax*sizeof(int*));
    memberCnt=(int*)malloc(nMax*sizeof(int));
    for(i=0;i<nMax;i++) {
        members[i]=(int*)malloc(maxMembers*sizeof(int));
        memberCnt[i]=0;
    }
    for(l=0;l<g.nVoxel;l++) {
        /*this voxel belongs to the maximum with index i in grid*/
        i=gTraces[l];
        /*the index of the maximum in list of maxima 'sorted' is j*/
        pos=(int*)bsearch(&i,sorted,nMax,sizeof(int),int_cmp);
        j=&pos[0]-&sorted[0];
        /*update information for maximum with index j*/
        members[j][memberCnt[j]]=l;
        memberCnt[j]++;
    }
    gridCrd=(t_vec*)malloc(nMax*sizeof(t_vec));
    n=strlen(fnCUBE);
    strncpy(tmp,fnCUBE,n-5);
    tmp[n-5]=(char)0;
    sprintf(fnOut,"%s_maxima.dat",tmp);
    io=fopen(fnOut,"w");
    fprintf(io,"#Note: all reported indices start at 1 (not 0)\n");
    fprintf(io,"#%11s %11s %11s% 7s %11s %11s %7s : %-16s\n",
        "x (A)","y (A)","z (A)"," index","peak","sum","nVoxels","member-indices");
    for(l=0;l<nMax;l++) {
        m=sorted[l];
        i=idx1Dto3D[m][0];
        j=idx1Dto3D[m][1];
        k=idx1Dto3D[m][2];
        vecAdd3idx(g.oriCUBE,i,g.a,j,g.b,k,g.c,&gridCrd[l]);
        fprintf(io,"%11.4e %11.4e %11.4e %7d %11.4e %11.4e %7d :",
            gridCrd[l][0],gridCrd[l][1],gridCrd[l][2],
            sorted[l]+1,g.grid[i][j][k],sums[l],cnts[l]);
        for(n=0;n<cnts[l];n++) {
            fprintf(io," %7d",members[l][n]+1);
        }
        fprintf(io,"\n");
    }
    fclose(io);
    return 0;
}
