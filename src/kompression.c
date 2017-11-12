#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils/files.h"
#include "utils/trees.h"

int main(int argc,char** argv){
    clock_t begin = clock();
    if(argc>1){
        FILE* fp=open_file(*(argv+1));
        if(fp==NULL) printf("Could not open file!\n");
        int* occurences=read_num_of_occurences(fp,argc>2?atoi(*(argv+2)):8);
        /*for(int i=0;i<256;i++){
            printf("%c: %d\n",i,*(occurences+i));
        }*/
        close_file(fp);
    }
    clock_t end = clock();
    double time_elapsed = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Time elapsed: %.2lfs\n",time_elapsed);
    return 0;
}