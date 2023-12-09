#include <stdio.h>
#include <stdlib.h>

int getString(char *input,char *store) {
    if(sscanf(input,"%s",store)!=1) {
        printf("ERROR: expected string but read '%s'\n",input);
        exit(1);
    }
    return 0;
}

int getFloat(char *input,float *store) {
    if(sscanf(input,"%f",&store)!=1) {
        printf("ERROR: expected float but read '%s'\n",input);
        exit(1);
    }
    return 0;
}

int getInt(char *input,int *store) {
    if(sscanf(input,"%d",&store)!=1) {
        printf("ERROR: expected int but read '%s'\n",input);
        exit(1);
    }
    return 0;
}