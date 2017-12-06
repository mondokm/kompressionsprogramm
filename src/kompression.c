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
        printf("[error] Not enough arguments!\n");
        return 0;
    }
    compression_mode mode=COMPRESSION;
    compression_bitlength bitlength=BYTE;
    char flags=0;

    if(**(argv+1)=='-'){
        flags=1;
        for(int i=1;i<strlen(*(argv+1));i++){
            switch(*(*(argv+1)+i)){
                case 'k':
                    mode=COMPRESSION;
                    break;
                case 'd':
                    mode=DECOMPRESSION;
                    break;
                case '8':
                    bitlength=BYTE;
                    break;
                case '1':
                    if(i<strlen(*(argv+1))-1){
                        if(*(*(argv+1)+i+1)=='6'){
                            bitlength=WORD;
                            i++;
                        }
                    }
            }
        }
    }
    if(mode==COMPRESSION){
        int numofbits=bitlength==BYTE?8:16;
        int arr_size=bitlength==BYTE?256:65537;
    
       FILE* file_in=read_file(*(argv+1+flags));

        //could not open file
        if(file_in==NULL) {
            printf("Could not open file!\n");
            return 0;
        }

        unsigned short leftover;
        unsigned long* occurences=read_num_of_occurences(file_in,numofbits,&leftover);
        if(file_in!=NULL) close_file(file_in);

        list_node* node_list=build_nodeptr_list(occurences,arr_size);
        node* head=build_node_tree(node_list,arr_size);
        unsigned short** codelengths=build_codelength_array(head,arr_size);
        unsigned short** codelengths_dup=(unsigned short**)malloc((arr_size)*sizeof(unsigned short*));
        memcpy(codelengths_dup,codelengths,(arr_size)*sizeof(unsigned short*));
        
        mpz_t* codes=build_codes(codelengths,arr_size);
        char** dictionary=build_dictionary(codes,codelengths,arr_size);


        file_in=read_file(*(argv+1+flags));
        FILE* file_out=write_file(create_filename(*(argv+1+flags),COMPRESSION));
        write_codelengths(file_out,codelengths_dup,numofbits,leftover);
        compress_file(dictionary,codelengths_dup,numofbits,file_in,file_out,leftover);
        if(file_out!=NULL) close_file(file_out);
        if(file_in!=NULL) close_file(file_in);
        
        free_tree(head);
        free(dictionary);
        free(codelengths);
        free(occurences);
    }else{
        FILE* file_in=read_file(*(argv+1+flags));

        //could not open file
        if(file_in==NULL) {
            printf("Could not open file!\n");
            return 0;
        }

        int numofbits;
        unsigned short leftover;
        unsigned short** codelengths=read_codelengths(file_in,&numofbits,&leftover);
        int arr_size=(numofbits==8?256:65536);
        unsigned short** codelengths_dup=(unsigned short**)malloc((arr_size)*sizeof(unsigned short*));
        memcpy(codelengths_dup,codelengths,(arr_size)*sizeof(unsigned short*));
        mpz_t* codes=build_codes(codelengths,arr_size);
        char** dictionary=build_dictionary(codes,codelengths,arr_size);

        node* tree=build_tree_from_codes(dictionary,arr_size,leftover);

        FILE* file_out=write_file(create_filename(*(argv+1+flags),DECOMPRESSION));
        decompress_file(tree,file_in,file_out,numofbits);

        close_file(file_in);
        close_file(file_out);

        free_tree(tree);
        free(codelengths);
        free(codelengths_dup);
        free(dictionary);
    }
    
    clock_t end = clock();
    double time_elapsed = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Time elapsed: %.2lfs\n",time_elapsed);
    return 0;
}