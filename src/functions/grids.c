#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <fftw3.h>
#include "../../include/types.h"
#include "../../include/matvec.h"

int readNBYTE(FILE *io,int expect,char *fnGrd) {
    int nb;

    fread(&nb,sizeof(int),1,io);
    if(nb!=expect) {
        printf("ERROR: unexpected format for binary UHBD grid file in %s\n",fnGrd);
        exit(1);
    }
    return 0;
}

int allocGrd(t_grid *g) {
    int i,j,k;
    printf("allocating %d bytes for grid\n",g[0].nVoxel*4);
    g[0].grid=(float***)malloc(g[0].dim[0]*sizeof(float**));
    if(g[0].grid==NULL) {
        printf("grid memory allocation failed\n");
        exit(1);
    }
    for(i=0;i<g[0].dim[0];i++) {
        g[0].grid[i]=(float**)malloc(g[0].dim[1]*sizeof(float*));
        if(g[0].grid[i]==NULL) {
            printf("grid memory allocation failed\n");
            exit(1);
        }
        for(j=0;j<g[0].dim[1];j++) {
            g[0].grid[i][j]=(float*)malloc(g[0].dim[2]*sizeof(float));
            if(g[0].grid[i][j]==NULL) {
                printf("grid memory allocation failed\n");
                exit(1);
            }
            for(k=0;k<g[0].dim[2];k++) {
                g[0].grid[i][j][k]=0.0;
            }
        }
    }
    return 0;
}

int readUHBDGridASCII(char *fnGrd,t_grid *grid,float scale) {
    FILE *io;
    float factor;
    float fdum;
    int idum[5];
    int i,j,k;
    int ni,nj,nk;
    char buffer[300];

    printf("opening file %s for reading\n",fnGrd);
    io=fopen(fnGrd,"r");
    if(io==NULL) {
        printf("ERROR: input grid file not found: %s\n",fnGrd);
        exit(1);
    }
    grid[0].aligned=1;
    grid[0].orthorhombic=1;
    grid[0].cubic=1;
    grid[0].nAtoms=0;
    fgets(buffer,300,io);
    sscanf(buffer,"%s",grid[0].title);
    fgets(buffer,300,io);
    sscanf(buffer,"%f %f %d %d %d %d %d",
        &factor,&fdum,&idum[0],&idum[1],&idum[2],&idum[3],&idum[4]
    );
    fgets(buffer,300,io);
    sscanf(buffer,"%d %d %d %lf %f %f %f",
        &grid[0].dim[0],
        &grid[0].dim[1],
        &grid[0].dim[2],
        &grid[0].dg,
        &grid[0].oriUHBD[0],
        &grid[0].oriUHBD[1],
        &grid[0].oriUHBD[2]
    );
    if(grid[0].dg<=0) {
        printf("negative or 0 grid constant %f in %s\n",grid[0].dg,fnGrd);
        exit(1);
    }
     /*define the vectors of the (implied) cubic voxels*/
    for(i=0;i<3;i++) {
        grid[0].a[i]=0.0;
        grid[0].b[i]=0.0;
        grid[0].c[i]=0.0;
    }
    grid[0].a[0]=grid[0].dg;
    grid[0].b[1]=grid[0].dg;
    grid[0].c[2]=grid[0].dg;

    ni=grid[0].dim[0];
    nj=grid[0].dim[1];
    nk=grid[0].dim[2];
    grid[0].nVoxel=ni*nj*nk;

    for(i=0;i<3;i++) {
        grid[0].oriMH[i]=grid[0].oriUHBD[i]+0.5*grid[0].dg;
        grid[0].oriCUBE[i]=grid[0].oriUHBD[i]+grid[0].dg;
    }
    fgets(buffer,300,io);
    fgets(buffer,300,io);

    allocGrd(grid);
    for(k=0;k<nk;k++) {
        fgets(buffer,300,io);
        for(j=0;j<nj;j++) {
            for(i=0;i<ni;i++) {
                fscanf(io,"%f",&grid[0].grid[i][j][k]);
                grid[0].grid[i][j][k]*=scale;
            }
        }
        fgets(buffer,300,io);
    }
    fclose(io);
    printf("closed file %s\n",fnGrd);
    return 0;
}

int readUHBDGrid(char *fnGrd,t_grid *grid,float scale) {
    FILE *io;
    char title[75];
    float factor;
    float fdum;
    int idum;
    int i,j,k;
    int ni,nj,nk;

    printf("opening file %s for reading\n",fnGrd);
    io=fopen(fnGrd,"rb");
    if(io==NULL) {
        printf("ERROR: input grid file not found: %s\n",fnGrd);
        exit(1);
    }
    grid[0].aligned=1;
    grid[0].orthorhombic=1;
    grid[0].cubic=1;
    grid[0].nAtoms=0;
    readNBYTE(io,160,fnGrd);
    fread(title,sizeof(char),72,io);
    fread(&factor,sizeof(float),1,io);
    fread(&fdum,sizeof(float),1,io);
    for(i=0;i<5;i++) {
        fread(&idum,sizeof(int),1,io);
    }
    fread(grid[0].dim,sizeof(int),3,io);
    ni=grid[0].dim[0];
    nj=grid[0].dim[1];
    nk=grid[0].dim[2];
    grid[0].nVoxel=ni*nj*nk;
    allocGrd(grid);

    fread(&grid[0].dg,sizeof(float),1,io);
    if(grid[0].dg<=0) {
        printf("negative or 0 grid constant %f in %s\n",grid[0].dg,fnGrd);
        exit(1);
    }
    /*define the vectors of the (implied) cubic voxels*/
    for(i=0;i<3;i++) {
        grid[0].a[i]=0.0;
        grid[0].b[i]=0.0;
        grid[0].c[i]=0.0;
    }
    grid[0].a[0]=grid[0].dg;
    grid[0].b[1]=grid[0].dg;
    grid[0].c[2]=grid[0].dg;

    fread(grid[0].oriUHBD,sizeof(float),3,io);
    /*shift origin from center of voxel [-1][-1][-1] to corner of voxel [0][0][0]*/
    /*this makes it easy to determine the position of something within the grid in C*/
    for(i=0;i<3;i++) {
        grid[0].oriMH[i]=grid[0].oriUHBD[i]+0.5*grid[0].dg;
        grid[0].oriCUBE[i]=grid[0].oriUHBD[i]+grid[0].dg;
    }
    for(i=0;i<4;i++) {
        fread(&fdum,sizeof(float),1,io);
    }
    for(i=0;i<2;i++) {
        fread(&fdum,sizeof(float),1,io);
    }
    for(i=0;i<2;i++) {
        fread(&idum,sizeof(int),1,io);
    }
    readNBYTE(io,160,fnGrd);
    for(k=0;k<nk;k++) {
        readNBYTE(io,12,fnGrd);
        for(i=0;i<3;i++) {
            fread(&idum,sizeof(int),1,io);
        }
        readNBYTE(io,12,fnGrd);
        readNBYTE(io,nj*ni*4,fnGrd);
        for(j=0;j<nj;j++) {
            for(i=0;i<ni;i++) {
                fread(&grid[0].grid[i][j][k],sizeof(float),1,io);
                grid[0].grid[i][j][k]*=scale;
            }
        }
        readNBYTE(io,nj*ni*4,fnGrd);
    }
    fclose(io);
    printf("closed file %s\n",fnGrd);
    return 0;
}

int writeUHBDGrid(char *fnGrd,t_grid grid,float scale) {
    FILE *io;
    int magic1,magic2,magic3;
    float factor=1.0;
    float fdum=0.0;
    int idum[5];
    int i,j,k,l;
    float tmp;
    char title[75];

    if(grid.aligned!=1 || grid.orthorhombic!=1 || grid.cubic!=1) {
        printf("ERROR: grid with non-cubic/non-aligned voxels\n");
        printf("ERROR: cannot write in UHBD format\n");
        exit(1);
    }
    magic1=160;
    magic2=12;
    magic3=grid.dim[0]*grid.dim[1]*4;
    idum[0]=1;
    idum[1]=0;
    idum[2]=grid.dim[2];
    idum[3]=1;
    idum[4]=grid.dim[2];
    for(i=0;i<75;i++) title[i]=(char)0;
    sprintf(title,"%s",grid.title);

    printf("opening file %s for writing\n",fnGrd);
    io=fopen(fnGrd,"wb");
    fwrite(&magic1,sizeof(int),1,io);
    fwrite(title,sizeof(char),72,io);
    fwrite(&factor,sizeof(float),1,io);
    fwrite(&fdum,sizeof(float),1,io);
    for(i=0;i<5;i++) fwrite(&idum[i],sizeof(int),1,io);
    fwrite(&grid.dim[0],sizeof(int),1,io);
    fwrite(&grid.dim[1],sizeof(int),1,io);
    fwrite(&grid.dim[2],sizeof(int),1,io);
    fwrite(&grid.dg,sizeof(float),1,io);
    fwrite(&grid.oriUHBD[0],sizeof(float),1,io);
    fwrite(&grid.oriUHBD[1],sizeof(float),1,io);
    fwrite(&grid.oriUHBD[2],sizeof(float),1,io);
    for(i=0;i<6;i++) fwrite(&fdum,sizeof(float),1,io);
    for(i=0;i<2;i++) fwrite(&idum[1],sizeof(int),1,io);
    fwrite(&magic1,sizeof(int),1,io);
    for(k=0;k<grid.dim[2];k++) {
        l=k+1;
        fwrite(&magic2,sizeof(int),1,io);
        fwrite(&l,sizeof(int),1,io);
        fwrite(&grid.dim[0],sizeof(int),1,io);
        fwrite(&grid.dim[1],sizeof(int),1,io);
        fwrite(&magic2,sizeof(int),1,io);
        fwrite(&magic3,sizeof(int),1,io);
        for(j=0;j<grid.dim[1];j++) {
            for(i=0;i<grid.dim[0];i++) {
                tmp=scale*grid.grid[i][j][k];
                fwrite(&tmp,sizeof(float),1,io);
            }
        }
        fwrite(&magic3,sizeof(int),1,io);
    }
    fclose(io);
    printf("closed file %s\n",fnGrd);
    return 0;
}

int readCUBE(char *fnGrd,t_grid *grid,float scale,int oriType) {
    FILE *io;
    char buffer[300];
    float tmp;
    float toA=0.529177;
    int i,j,k;
    int ni,nj,nk;
    t_vec a,b,c;
    t_vec crd;

    /*oriType=1 non-standard 3D-2PT internal CUBE format: origin describes corner of voxel [0][0][0]*/
    /*oriType!=1 standard CUBE format: origin describes center of voxel [0][0][0]*/

    printf("opening file %s for reading\n",fnGrd);
    io=fopen(fnGrd,"r");
    if(io==NULL) {
        printf("ERROR: grid file %s not found\n",fnGrd);
        exit(1);
    }
    fgets(buffer,300,io);
    for(i=0;i<75;i++) grid[0].title[i]=(char)0;
    buffer[72]=(char)0;
    sscanf(buffer,"%s",grid[0].title);
    fgets(buffer,300,io);
    fgets(buffer,300,io);
    sscanf(buffer,"%d %f %f %f\n",&grid[0].nAtoms,&grid[0].oriMH[0],&grid[0].oriMH[1],&grid[0].oriMH[2]);

    fgets(buffer,300,io);
    sscanf(buffer,"%d %f %f %f",&ni,&a[0],&a[1],&a[2]);
    if(ni<0) {
        ni*=-1;
        toA=1.0;
        printf("NOTE: coordinates in CUBE file: %s in Angstrom\n",fnGrd);
    }
    vecScale(toA,a,&a);
    vecCpy(a,&grid[0].a);

    fgets(buffer,300,io);
    sscanf(buffer,"%d %f %f %f",&nj,&b[0],&b[1],&b[2]);
    vecScale(toA,b,&b);
    vecCpy(b,&grid[0].b);
    if(nj<0) nj*=-1;

    fgets(buffer,300,io);
    sscanf(buffer,"%d %f %f %f",&nk,&c[0],&c[1],&c[2]);
    vecScale(toA,c,&c);
    vecCpy(c,&grid[0].c);
    if(nk<0) nk*=-1;

    grid[0].aligned=1;
    if(a[1]!=0.0 || a[2]!=0.0) grid[0].aligned=0;
    if(b[0]!=0.0 || b[2]!=0.0) grid[0].aligned=0;
    if(c[0]!=0.0 || c[1]!=0.0) grid[0].aligned=0;
    if(grid[0].aligned!=1) {
        printf("NOTE: grid voxels not aligned with axes\n");
    }

    if(vecOrtho(a,b)==1 && vecOrtho(a,c)==1 && vecOrtho(b,c)==1) {
        grid[0].orthorhombic=1;
        if(vecLenEq(a,b)==1 && vecLenEq(a,c)==1 && vecLenEq(b,c)==1) {
            grid[0].cubic=1;
            vecNorm(a,&grid[0].dg);
        } else {
            grid[0].cubic=0;
            grid[0].dg=0.0;
            printf("NOTE: grid voxels orthorhombic but not cubic\n");
        }
    } else {
        grid[0].orthorhombic=0;
        grid[0].cubic=0;
        grid[0].dg=0.0;
        printf("NOTE: grid voxels not orthorhombic\n");
    }
    
    grid[0].dim[0]=ni;
    grid[0].dim[1]=nj;
    grid[0].dim[2]=nk;
    grid[0].nVoxel=ni*nj*nk;
    for(i=0;i<3;i++) grid[0].oriMH[i]*=toA;
    if(oriType!=1) {
        /*if standard CUBE format, i.e. origin in file is center of voxel [0][0][0]*/
        vecCpy(grid[0].oriMH,&grid[0].oriCUBE);
        vecSub3(grid[0].oriMH,a,b,c,&grid[0].oriUHBD);
        vecSub3scale(grid[0].oriMH,0.5,a,b,c,&grid[0].oriMH);
    } else {
        /*if 3D-2PT format, i.e. origin in file is corner of voxel [0][0][0]*/
        vecAdd3scale(grid[0].oriMH,0.5,a,b,c,&grid[0].oriCUBE);
        vecSub3scale(grid[0].oriMH,0.5,a,b,c,&grid[0].oriUHBD);
    }

    grid[0].atoms=(t_atom*)malloc(grid[0].nAtoms*sizeof(t_atom));
    for(i=0;i<grid[0].nAtoms;i++) {
        fgets(buffer,300,io);
        sscanf(buffer,"%d %f %f %f %f",
            &grid[0].atoms[i].elNum,
            &grid[0].atoms[i].charge,
            &crd[0],
            &crd[1],
            &crd[2]
        );
        vecScale(toA,crd,&crd);
        vecCpy(crd,&grid[0].atoms[i].crd);
    }

    allocGrd(grid);
    for(i=0;i<ni;i++) {
        for(j=0;j<nj;j++) {
            for(k=0;k<nk;k++) {
                fscanf(io,"%f",&tmp);
                if(scale!=1.0) printf("NOTE: grid values scled by %f\n",scale);
                grid[0].grid[i][j][k]=tmp*scale;
            }
        }
    }
    fclose(io);
    printf("closed file %s\n",fnGrd);
    return 0;
}

int writeCUBE(char *fnGrd,t_grid grid,float scale,int oriType) {
    FILE *io;
    int i,j,k;
    int ni,nj,nk;
    float toA=0.529177;

    ni=grid.dim[0];
    nj=grid.dim[1];
    nk=grid.dim[2];

    io=fopen(fnGrd,"w");
    fprintf(io,"%s\n",grid.title);
    if(oriType==1) {
        fprintf(io,"non-standard format: origin = corner of voxel [0][0][0]\n");
        fprintf(io,"%5d%12.6f%12.6f%12.6f\n",grid.nAtoms,grid.oriMH[0]/toA,grid.oriMH[1]/toA,grid.oriMH[2]/toA);
    } else {
        fprintf(io,"Gaussian cube format\n");
        fprintf(io,"%5d%12.6f%12.6f%12.6f\n",grid.nAtoms,grid.oriCUBE[0]/toA,grid.oriCUBE[1]/toA,grid.oriCUBE[2]/toA);
    }
    fprintf(io,"%5d%12.6f%12.6f%12.6f\n",ni,grid.a[0]/toA,grid.a[1]/toA,grid.a[2]/toA);
    fprintf(io,"%5d%12.6f%12.6f%12.6f\n",ni,grid.b[0]/toA,grid.b[1]/toA,grid.b[2]/toA);
    fprintf(io,"%5d%12.6f%12.6f%12.6f\n",ni,grid.c[0]/toA,grid.c[1]/toA,grid.c[2]/toA);

    for(i=0;i<grid.nAtoms;i++) {
        fprintf(io,"%5d%12.6f%12.6f%12.6f%12.6f\n",
            grid.atoms[i].elNum,
            grid.atoms[i].charge,
            grid.atoms[i].crd[0],
            grid.atoms[i].crd[1],
            grid.atoms[i].crd[2]
        );
    }

    for(i=0;i<ni;i++) {
        for(j=0;j<nj;j++) {
            for(k=0;k<nk;k++) {
                fprintf(io,"%13.5e",grid.grid[i][j][k]);
                if(k%6==5) fprintf(io,"\n");
            }
            fprintf(io,"\n");
        }
    }
    fclose(io);
    return 0;
}

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
        printf("ERROR: function \'smoothGrid\' only implemented for cubic voxels\n");
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

int setCUBEtitle(t_grid *grid,char *title) {
    strncpy(grid[0].title,title,72);
    grid[0].title[72]=(char)0;
    return 0;
}

int eqCUBEformat(t_grid gA,t_grid gB) {
    int m;
    int equal=1;

    if(gA.nVoxel!=gB.nVoxel) equal=0;
    if(gA.dg!=gB.dg) equal=0;
    for(m=0;m<3;m++) {
        if(gA.dim[m]!=gB.dim[m]) equal=0;
        if(gA.oriCUBE[m]!=gB.oriCUBE[m]) equal=0;
        if(gA.a[m]!=gB.a[m]) equal=0;
        if(gA.b[m]!=gB.b[m]) equal=0;
        if(gA.c[m]!=gB.c[m]) equal=0;
    }
    return equal;
}

int cpyCUBEformat(t_grid gA,t_grid *gB) {
    int m;

    gB[0].nVoxel=gA.nVoxel;
    gB[0].dg=gA.dg;
    gB[0].aligned=gA.aligned;
    gB[0].orthorhombic=gA.orthorhombic;
    gB[0].cubic=gA.cubic;
    strcpy(gB[0].title,gA.title);
    for(m=0;m<3;m++) {
        gB[0].dim[m]=gA.dim[m];
        gB[0].oriUHBD[m]=gA.oriUHBD[m];
        gB[0].oriMH[m]=gA.oriMH[m];
        gB[0].oriCUBE[m]=gA.oriCUBE[m];
        gB[0].a[m]=gA.a[m];
        gB[0].b[m]=gA.b[m];
        gB[0].c[m]=gA.c[m];
    }
    gB[0].nAtoms=gA.nAtoms;
    gB[0].atoms=gA.atoms;
    return 0;
}
