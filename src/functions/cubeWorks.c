#include <stdio.h>

int printName(char *name) {
    printf("  ___________                            ___________\n");
    printf(" |_|_|_|_|_|_| cubeWorks 1.0            |_|_|_|_|_|_|\n");
    printf(" |_|_|_|_|_|_| -> %-21s |_|_|_|_|_|_|\n",name);
    printf(" |_|_|_|_|_|_|                          |_|_|_|_|_|_|\n");
    printf(" |_|_|_|_|_|_| Author: Matthias Heyden  |_|_|_|_|_|_|\n");
    printf(" |_|_|_|_|_|_| email:  mheyden1@asu.edu |_|_|_|_|_|_|\n");
    printf(" |_|_|_|_|_|_|         December 2023    |_|_|_|_|_|_|\n");
    printf("                                                     \n");
    return 0;
}