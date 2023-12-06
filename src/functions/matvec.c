#include <stdlib.h>
#include <math.h>
#include "../../include/types.h"

int vecCpy(t_vec a,t_vec *b) {
    int i;
    for(i=0;i<3;i++) b[0][i]=a[i];
    return 0;
}

int matCpy(t_mat a,t_mat *b) {
    int i,j;
    for(i=0;i<3;i++) {
        for(j=0;j<3;j++) {
            b[0][i][j]=a[i][j];
        }
    }
    return 0;
}

int linkPBC(t_vec a,t_vec b,float box,t_vec *c) {
    int i;

    for(i=0;i<3;i++) {
        c[0][i]=b[i]-a[i];
        c[0][i]-=floor((c[0][i]/box)+0.5)*box;
    }
    return 0;
}

int vecNorm(t_vec a,double *d) {
    double dSq;
    dSq=(a[0]*a[0]+a[1]*a[1]+a[2]*a[2]);
    d[0]=sqrt(dSq);
    return 0;
}

int vecNormSq(t_vec a,double *dSq) {
    dSq[0]=a[0]*a[0]+a[1]*a[1]+a[2]*a[2];
    return 0;
}

int vecDot(t_vec a,t_vec b,double *res) {
    res[0]=a[0]*b[0]+a[1]*b[1]+a[2]*b[2];
    return 0;
}

int distPBC(t_vec a,t_vec b,float box,double *d) {
    t_vec l;
    linkPBC(a,b,box,&l);
    vecNorm(l,d);
    return 0;
}

int matvec(t_mat a,t_vec b,t_vec *c) {
    c[0][0]=a[0][0]*b[0]+a[0][1]*b[1]+a[0][2]*b[2];
    c[0][1]=a[1][0]*b[0]+a[1][1]*b[1]+a[1][2]*b[2];
    c[0][2]=a[2][0]*b[0]+a[2][1]*b[1]+a[2][2]*b[2];
    return 0;
}

int matTvec(t_mat a,t_vec b,t_vec *c) {
    c[0][0]=a[0][0]*b[0]+a[1][0]*b[1]+a[2][0]*b[2];
    c[0][1]=a[0][1]*b[0]+a[1][1]*b[1]+a[2][1]*b[2];
    c[0][2]=a[0][2]*b[0]+a[1][2]*b[1]+a[2][2]*b[2];
    return 0;
}

int matmat(t_mat a,t_mat b,t_mat *c) {
    c[0][0][0]=a[0][0]*b[0][0]+a[0][1]*b[1][0]+a[0][2]*b[2][0];
    c[0][0][1]=a[0][0]*b[0][1]+a[0][1]*b[1][1]+a[0][2]*b[2][1];
    c[0][0][2]=a[0][0]*b[0][2]+a[0][1]*b[1][2]+a[0][2]*b[2][2];
    c[0][1][0]=a[1][0]*b[0][0]+a[1][1]*b[1][0]+a[1][2]*b[2][0];
    c[0][1][1]=a[1][0]*b[0][1]+a[1][1]*b[1][1]+a[1][2]*b[2][1];
    c[0][1][2]=a[1][0]*b[0][2]+a[1][1]*b[1][2]+a[1][2]*b[2][2];
    c[0][2][0]=a[2][0]*b[0][0]+a[2][1]*b[1][0]+a[2][2]*b[2][0];
    c[0][2][1]=a[2][0]*b[0][1]+a[2][1]*b[1][1]+a[2][2]*b[2][1];
    c[0][2][2]=a[2][0]*b[0][2]+a[2][1]*b[1][2]+a[2][2]*b[2][2];
    return 0;
}

int matTmat(t_mat a,t_mat b,t_mat *c) {
    c[0][0][0]=a[0][0]*b[0][0]+a[1][0]*b[1][0]+a[2][0]*b[2][0];
    c[0][0][1]=a[0][0]*b[0][1]+a[1][0]*b[1][1]+a[2][0]*b[2][1];
    c[0][0][2]=a[0][0]*b[0][2]+a[1][0]*b[1][2]+a[2][0]*b[2][2];
    c[0][1][0]=a[0][1]*b[0][0]+a[1][1]*b[1][0]+a[2][1]*b[2][0];
    c[0][1][1]=a[0][1]*b[0][1]+a[1][1]*b[1][1]+a[2][1]*b[2][1];
    c[0][1][2]=a[0][1]*b[0][2]+a[1][1]*b[1][2]+a[2][1]*b[2][2];
    c[0][2][0]=a[0][2]*b[0][0]+a[1][2]*b[1][0]+a[2][2]*b[2][0];
    c[0][2][1]=a[0][2]*b[0][1]+a[1][2]*b[1][1]+a[2][2]*b[2][1];
    c[0][2][2]=a[0][2]*b[0][2]+a[1][2]*b[1][2]+a[2][2]*b[2][2];
    return 0;
}

int randUnitVec(t_vec *a) {
    float u1,u2;
    float th,ph;

    u1=((float)rand())/RAND_MAX;
    u2=((float)rand())/RAND_MAX;
    th=2*acos(u1)-1;
    ph=2*3.14159*u2;
    a[0][0]=sin(th)*cos(ph);
    a[0][1]=sin(th)*sin(ph);
    a[0][2]=cos(th);
    return 0;
}

int rotZ(double angle,t_mat *rz) {
    rz[0][0][0]=cos(angle);
    rz[0][0][1]=-1.0*sin(angle);
    rz[0][0][2]=0.0;
    rz[0][1][0]=sin(angle);
    rz[0][1][1]=cos(angle);
    rz[0][1][2]=0.0;
    rz[0][2][0]=0.0;
    rz[0][2][1]=0.0;
    rz[0][2][2]=1.0;
    return 0;
}

int rotX(double angle,t_mat *rx) {
    rx[0][0][0]=1.0;
    rx[0][0][1]=0.0;
    rx[0][0][2]=0.0;
    rx[0][1][0]=0.0;
    rx[0][1][1]=cos(angle);
    rx[0][1][2]=-1.0*sin(angle);
    rx[0][2][0]=0.0;
    rx[0][2][1]=sin(angle);
    rx[0][2][2]=cos(angle);
    return 0;
}

int rotAny(double angle,t_vec axis,t_mat *r) {
    t_mat rx,rz,r1,r2,r3;
    double a1,a2;
    t_vec tmp;

    if(axis[0]!=0.0) {
        if(axis[1]!=0.0) a1=atan(axis[0]/axis[1]);
        else a1=3.14159/2.0;
    } else a1=0.0;
    rotZ(a1,&rz);
    matvec(rz,axis,&tmp);
    if(tmp[1]!=0) {
        if(tmp[2]!=0) a2=atan(tmp[1]/tmp[2]);
        else a2=3.14159/2.0;
    } else a2=0.0;
    rotX(a2,&rx);

    matmat(rx,rz,&r1);
    rotZ(angle,&r2);
    matmat(r2,r1,&r3);
    matTmat(rx,r3,&r1);
    matTmat(rz,r1,r);

    return 0;
}

int vecScale(double s,t_vec a,t_vec *b) {
    b[0][0]=s*a[0];
    b[0][1]=s*a[1];
    b[0][2]=s*a[2];
    return 0;
}

int vecAdd(t_vec a,t_vec b,t_vec *c) {
    int i;
    for(i=0;i<3;i++) {
        c[0][i]=a[i]+b[i];
    }
    return 0;
}

int vecAdd3(t_vec a,t_vec b1,t_vec b2,t_vec b3,t_vec *c) {
    int i;
    for(i=0;i<3;i++) {
        c[0][i]=a[i]+b1[i]+b2[i]+b3[i];
    }
    return 0;
}

int vecAdd3scale(t_vec a,double s,t_vec b1,t_vec b2,t_vec b3,t_vec *c) {
    int i;
    for(i=0;i<3;i++) {
        c[0][i]=a[i]+s*(b1[i]+b2[i]+b3[i]);
    }
    return 0;
}

int vecAdd3idx(t_vec a,int i,t_vec bi,int j,t_vec bj,int k,t_vec bk,t_vec *c) {
    int m;
    for(m=0;m<3;m++) {
        c[0][m]=a[m]+i*bi[m]+j*bj[m]+k*bk[m];
    }
    return 0;
}

int vecSub(t_vec a,t_vec b,t_vec *c) {
    int i;
    for(i=0;i<3;i++) {
        c[0][i]=a[i]-b[i];
    }
    return 0;
}

int vecSub3(t_vec a,t_vec b1,t_vec b2,t_vec b3,t_vec *c) {
    int i;
    for(i=0;i<3;i++) {
        c[0][i]=a[i]-b1[i]-b2[i]-b3[i];
    }
    return 0;
}

int vecSub3scale(t_vec a,double s,t_vec b1,t_vec b2,t_vec b3,t_vec *c) {
    int i;
    for(i=0;i<3;i++) {
        c[0][i]=a[i]-s*(b1[i]+b2[i]+b3[i]);
    }
    return 0;
}

int vecLenEq(t_vec a,t_vec b) {
    double al,bl,dab;
    vecNorm(a,&al);
    vecNorm(b,&bl);
    dab=al-bl;
    if(dab<0.0) dab*=-1.0;
    if(dab<1.0e-8) return 1;
    else return 0;
}

int vecOrtho(t_vec a,t_vec b) {
    double dot;
    vecDot(a,b,&dot);
    if(dot<0.0) dot*=-1.0;
    if(dot<1.0e-8) return 1;
    else return 0;
}

int crossCompleteMat(t_mat *a) {
    a[0][2][0]=a[0][0][1]*a[0][1][2]-a[0][0][2]*a[0][1][1];
    a[0][2][1]=a[0][0][2]*a[0][1][0]-a[0][0][0]*a[0][1][2];
    a[0][2][2]=a[0][0][0]*a[0][1][1]-a[0][0][1]*a[0][1][0];
    return 0;
}

int getMolCrd(t_mat rot,t_vec trans,t_mol *mol) {
    int conf;
    int i,n;
    t_vec tmp;

    conf=mol[0].curConf;
    mol[0].nAtoms=mol[0].conf[conf].nAtoms;
    n=mol[0].nAtoms;
    for(i=0;i<n;i++) {
        matvec(rot,mol[0].conf[conf].atCrd[i],&tmp);
        vecAdd(tmp,trans,&mol[0].atCrd[i]);
    }

    mol[0].nQ=mol[0].conf[conf].nQ;
    n=mol[0].nQ;
    for(i=0;i<n;i++) {
        matvec(rot,mol[0].conf[conf].qCrd[i],&tmp);
        vecAdd(tmp,trans,&mol[0].qCrd[i]);
    }

    mol[0].nLJ=mol[0].conf[conf].nLJ;
    n=mol[0].nLJ;
    for(i=0;i<n;i++) {
        matvec(rot,mol[0].conf[conf].ljCrd[i],&tmp);
        vecAdd(tmp,trans,&mol[0].ljCrd[i]);
    }

    return 0;
}
