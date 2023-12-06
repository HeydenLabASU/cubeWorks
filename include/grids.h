int readNBYTE(FILE *io,int expect,char *fnGrd);
int allocGrd(t_grid *g);
int readUHBDGridASCII(char *fnGrd,t_grid *grid,float scale); 
int readUHBDGrid(char *fnGrd,t_grid *grid,float scale);
int writeUHBDGrid(char *fnGrd,t_grid grid,float scale);
int readCUBE(char *fnGrd,t_grid *grid,float scale,int oriType);
int writeCUBE(char *fnGrd,t_grid grid,float scale,int oriType);
int smoothGrid(t_grid g,float sigma,t_grid *res);
