#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "t_stdio.h"
int main(int argc,char *argv[]){
    FILE *fp =fopen(argv[1],"r");
    if(fp==NULL){
#if 0
        printf("fopen failed %d \n",errno);
        printf("fopen failed %s \n", strerror(errno));
        perror("fopen");
        return -1;
#endif
		E_MSG("fopen",-1);
    }
    printf("succed \n");
    fclose(fp);
    return 0;
}
