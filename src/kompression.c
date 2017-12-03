#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "utils/files.h"
#include "utils/huffman.h"

int main(int argc,char** argv){
    clock_t begin = clock();
    
    //no arguments received
    if(argc<=1){
        printf("[error] No file specified!\n");
        return 0;
    }
    FILE* file_in=read_file(*(argv+1));
    
    //could not open file
    if(file_in==NULL) {
        printf("Could not open file!\n");
        return 0;
    }

    char mode=argc>2?atoi(*(argv+2)):8;
    unsigned long* occurences=read_num_of_occurences(file_in,mode);
    if(file_in!=NULL) close_file(file_in);

    unsigned long sum=0;
    for(int i=0;i<(mode==8?256:65536);i++){
        sum+=*(occurences+i);
    }
    printf("%ld\n",sum);

    list_node* node_list=build_nodeptr_list(occurences,mode==8?256:65536);
    node* head=build_node_tree(node_list,mode==8?256:65536);
    unsigned short** codelengths=build_codelength_array(head,mode==8?256:65536);
    unsigned short** codelengths_dup=(unsigned short**)malloc((mode==8?256:65536)*sizeof(unsigned short*));
    memcpy(codelengths_dup,codelengths,(mode==8?256:65536)*sizeof(unsigned short*));
    
    unsigned long filesize=0;
    for(int i=0;i<(mode==8?256:65536);i++){
        filesize+=(**(codelengths+i))*(*(occurences+i));
    }
    printf("New filesize: %lu\n",filesize/8);
    mpz_t* codes=build_codes(codelengths,mode==8?256:65536);
    char** dictionary=build_dictionary(codes,codelengths_dup,mode==8?256:65536);

    file_in=read_file(*(argv+1));
    FILE* file_out=write_file(create_filename(*(argv+1)));
    write_codelengths(file_out,codelengths_dup,mode);
    compress_file(dictionary,codelengths_dup,mode,file_in,file_out);
    if(file_out!=NULL) close_file(file_out);
    if(file_in!=NULL) close_file(file_in);
    
    free_tree(head);
    free(dictionary);
    free(codelengths);
    free(occurences);
    
    clock_t end = clock();
    double time_elapsed = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Time elapsed: %.2lfs\n",time_elapsed);
    return 0;
}