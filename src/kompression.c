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
    char filenum=0;
    for(int i=1;i<argc;i++){
        if(**(argv+i)=='-'){
            for(int j=1;j<strlen(*(argv+i));j++){
                switch(*(*(argv+i)+j)){
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
                        if(j<strlen(*(argv+i))-1){
                            if(*(*(argv+i)+j+1)=='6'){
                                bitlength=WORD;
                                j++;
                            }
                        }
                }
            }
        }
        else {
            if(filenum) printf("Too many arguments.\n");
            else filenum=i;
        }
    }
    
    if(mode==COMPRESSION){
        int numofbits=bitlength==BYTE?8:16;
        int arr_size=bitlength==BYTE?256:65536;
    
       FILE* file_in=read_file(*(argv+filenum));

        //could not open file
        if(file_in==NULL) {
            printf("Could not open file!\n");
            return 0;
        }

        long filesize=get_file_size(file_in);

        short leftover=-1;
        unsigned long* occurences=read_num_of_occurences(file_in,numofbits,&leftover);
        if(file_in!=NULL) close_file(file_in);

        list_node* node_list=build_nodeptr_list(occurences,arr_size);
        node* head=build_node_tree(node_list,arr_size);
        unsigned short** codelengths=build_codelength_array(head,arr_size);

        int maxlen=maxcodelength(codelengths,arr_size);
        compression_bitlength codelength_length=BYTE;
        if(maxlen>255) codelength_length=WORD;
        unsigned short** codelengths_dup=(unsigned short**)malloc((arr_size)*sizeof(unsigned short*));
        memcpy(codelengths_dup,codelengths,(arr_size)*sizeof(unsigned short*));
        
        mpz_t* codes=build_codes(codelengths,arr_size);
        char** dictionary=build_dictionary(codes,codelengths,arr_size);

        int leftover_exists=(leftover==-1)?0:1;
        char header=create_header_byte(bitlength,codelength_length,leftover_exists);

        file_in=read_file(*(argv+filenum));
        FILE* file_out=write_file(create_filename(*(argv+filenum),COMPRESSION));
        write_codelengths(file_out,codelengths_dup,header,leftover,filesize);
        compress_file(dictionary,codelengths_dup,numofbits,file_in,file_out,leftover, leftover_exists);
        if(file_out!=NULL) close_file(file_out);
        if(file_in!=NULL) close_file(file_in);
        
        free_tree(head);
        free(dictionary);
        free(codelengths);
        free(occurences);
    }else{
        FILE* file_in=read_file(*(argv+filenum));

        //could not open file
        if(file_in==NULL) {
            printf("Could not open file!\n");
            return 0;
        }

        short leftover=-1;
        char header;
        long filesize=0;
        
        unsigned short** codelengths=read_codelengths(file_in,&header,&leftover,&filesize);
        if(codelengths==NULL) return 1;
        int numofbits=(!header&1)?8:16;
        int arr_size=(!header&1)?256:65536;
        int leftover_exists=(leftover==-1)?0:1;

        unsigned short** codelengths_dup=(unsigned short**)malloc((arr_size)*sizeof(unsigned short*));
        memcpy(codelengths_dup,codelengths,(arr_size)*sizeof(unsigned short*));
        mpz_t* codes=build_codes(codelengths,arr_size);

        char** dictionary=build_dictionary(codes,codelengths,arr_size);
        node* tree=build_tree_from_codes(dictionary,arr_size);

        FILE* file_out=write_file(create_filename(*(argv+filenum),DECOMPRESSION));
        decompress_file(tree,file_in,file_out,numofbits, leftover, leftover_exists, filesize);
        
        close_file(file_in);
        close_file(file_out);

        free_search_tree(tree);
        free(codelengths);
        free(codelengths_dup);
        free(dictionary);
    }
    
    clock_t end = clock();
    double time_elapsed = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Time elapsed: %.2lfs\n",time_elapsed);
    return 0;
}