#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fftw3.h>
#include "../../include/types.h"
#include "../../include/matvec.h"
#include "../../include/grids.h"

/*requires FFTW3f (single precision) */
int smoothGrid(t_grid g,float sigma,t_grid *res) {
    fftwf_complex *data,*filter;
    fftwf_plan ftData,ftFilter,invftData;
    float step;
    int i,j,k;
    int i2,j2,k2;
    int pos;
    int ni,nj,nk;
    double dsq;
    double gaussNorm;

    if(g.cubic!=1) {
        fprintf(stderr,"ERROR: function \'smoothGrid\' only implemented for cubic voxels\n");
        exit(1);
    }

    ni=g.dim[0];
    nj=g.dim[1];
    nk=g.dim[2];

    res[0].dim[0]=ni;
    res[0].dim[1]=nj;
    res[0].dim[2]=nk;
    vecCpy(g.oriUHBD,&res[0].oriUHBD);
    vecCpy(g.oriMH,&res[0].oriMH);
    vecCpy(g.oriCUBE,&res[0].oriCUBE);
    res[0].dg=g.dg;
    vecCpy(g.a,&res[0].a);
    vecCpy(g.b,&res[0].b);
    vecCpy(g.c,&res[0].c);
    res[0].nVoxel=g.nVoxel;
    allocGrd(res);
    for(i=0;i<75;i++) res[0].title[i]=(char)0;
    sprintf(res[0].title,"resolution %6.3f A",sigma);

    data=(fftwf_complex*)malloc(ni*nj*nk*sizeof(fftwf_complex));
    filter=(fftwf_complex*)malloc(ni*nj*nk*sizeof(fftwf_complex));
    ftData=fftwf_plan_dft_3d(ni,nj,nk,data,data,FFTW_FORWARD,FFTW_ESTIMATE);
    invftData=fftwf_plan_dft_3d(ni,nj,nk,data,data,FFTW_BACKWARD,FFTW_ESTIMATE);
    ftFilter=fftwf_plan_dft_3d(ni,nj,nk,filter,filter,FFTW_FORWARD,FFTW_ESTIMATE);

    gaussNorm=1.0/pow(2.0*3.14159265359*sigma*sigma,1.5);
    step=g.dg/sigma;
    for(i=0;i<ni;i++) {
        for(j=0;j<nj;j++) {
            for(k=0;k<nk;k++) {
                i2=i;
                j2=j;
                k2=k;
                if(i2>ni/2) i2-=ni;
                if(j2>nj/2) j2-=nj;
                if(k2>nk/2) k2-=nk;
                dsq=(i2*i2+j2*j2+k2*k2)*step*step;
                pos=k+nk*(j+nj*i);
                filter[pos][0]=gaussNorm*exp(-0.5*dsq);
                filter[pos][1]=0.0;
            }
        }
    }
    fftwf_execute(ftFilter);
    for(i=0;i<ni;i++) {
        for(j=0;j<nj;j++) {
            for(k=0;k<nk;k++) {
                pos=k+nk*(j+nj*i);
                data[pos][0]=g.grid[i][j][k];
                data[pos][1]=0.0;
            }
        }
    }
    fftwf_execute(ftData);
    for(i=0;i<ni;i++) {
        for(j=0;j<nj;j++) {
            for(k=0;k<nk;k++) {
                pos=k+nk*(j+nj*i);
                data[pos][0]*=filter[pos][0];
                data[pos][1]*=filter[pos][0];
            }
        }
    }
    fftwf_execute(invftData);
    for(i=0;i<ni;i++) {
        for(j=0;j<nj;j++) {
            for(k=0;k<nk;k++) {
                pos=k+nk*(j+nj*i);
                res[0].grid[i][j][k]=data[pos][0]/(ni*nj*nk);
            }
        }
    }
    fftwf_destroy_plan(ftData);
    fftwf_destroy_plan(ftFilter);
    fftwf_destroy_plan(invftData);
    fftwf_free(filter);
    fftwf_free(data);
    return 0;
}
