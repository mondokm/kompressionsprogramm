#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils/files.h"
#include "utils/trees.h"

int main(int argc,char** argv){
    clock_t begin = clock();
    
    //no arguments received
    if(argc<=1){
        printf("[error] No file specified!\n");
        return 0;
    }
    FILE* fp=open_file(*(argv+1));
    
    //could not open file
    if(fp==NULL) {
        printf("Could not open file!\n");
        return 0;
    }

    char mode=argc>2?atoi(*(argv+2)):8;
    unsigned long* occurences=read_num_of_occurences(fp,mode);
    if(fp!=NULL) close_file(fp);
    
    unsigned long sum=0;
    for(int i=0;i<(mode==8?256:65536);i++){
        sum+=*(occurences+i);
    }
    printf("%ld\n",sum);

    list_node* node_list=build_nodeptr_list(occurences,mode==8?256:65536);
    node* head=build_node_tree(node_list);
    int* codelengths=build_codelength_array(head,mode==8?256:65536);
    int min=*codelengths;
    for(int i=0;i<(mode==8?256:65536);i++){
        printf("%d: %d\n",i,*(codelengths+i));
        if(*(codelengths+i)<min) min=*(codelengths+i);
    }
    printf("Min: %d\n",min);

    printf("Freq: %ld\n",head->frequency);

    clock_t end = clock();
    double time_elapsed = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Time elapsed: %.2lfs\n",time_elapsed);
    return 0;
}