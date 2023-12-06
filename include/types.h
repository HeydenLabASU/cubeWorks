typedef float t_vec[3];
typedef double t_mat[3][3];

typedef struct {
    int elNum;
    float charge;
    t_vec crd;
} t_atom;

typedef struct {
    int dim[3];
    int nVoxel;
    t_vec oriUHBD; /*center of voxel [-1][-1][-1] (makes sense in fortran)*/
	t_vec oriMH;   /*corner of voxel [0][0][0]*/
	t_vec oriCUBE; /*center of voxel [0][0][0]*/
    t_vec a,b,c;
	double dg;
	int aligned;
	int orthorhombic;
	int cubic;
    float ***grid;
    char title[75];
	double maxRange;
    int nAtoms;
    t_atom *atoms;
} t_grid;

typedef struct {
    int nAtoms;
    t_vec atCrd[5000];
	double Vfrac[5000];
    float atMass[5000];
    double totMass;
	double totMass2;
	double vdWvol;
    char atName[5000][10];
	char elem[5000];
    char resName[5000][10];
    int resID[5000];
    int nQ;
    t_vec qCrd[5000];
    float q[5000];
    float totQ;
    int nLJ;
    t_vec ljCrd[5000];
	double ljSigma[5000];
	double ljEpsilon[5000];
    double ljF6[5000];
	double ljF12[5000];
	t_vec COM,COM2;
    t_grid es,solv,ljR6,ljR12;
	int ****gLabel;
	int **Hsel,**Csel,**NOsel,**SPsel;
	int nHsel,nCsel,nNOsel,nSPsel;
    float priori;
	double maxDist2Cen;
} t_conf;

typedef struct {
	int type;
	int curConf;
	t_vec com;
    t_mat rot;
    int nConf;
    t_conf *conf;
	/*following ignored by cpyMol*/
    int nAtoms;
    t_vec atCrd[5000];
    int nQ;
    t_vec qCrd[5000];
    int nLJ;
    t_vec ljCrd[5000];
} t_mol;

typedef struct {
	/*Debye-Hueckel theory parameters*/
	double eps;
	double k; /* (1/A) */
	double l; /* (A) */
	double a0; /* (A) */
	double T; /* (K) */
	double ios; /* (M) */
	double a0k;
} t_dht;

typedef struct {
	double es;
	double solv;
	double lj;
	double total;
} t_ener;
