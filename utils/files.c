#include "files.h"
#include <string.h>
#include <stdlib.h>

FILE* open_file(char* name){
    return fopen(name,"r");
}

FILE* create_output_file(char* name){
    return fopen(create_filename(name),"w");
}

char* create_filename(char* input){
    //check if input has an extension
    int i,k;
    for(i=0;i<strlen(input);i++){
        if(*(input+i)=='.') k=i;
    }
    //if it has an extension, then we cut it off
    if(i==0) k=strlen(input);
    char* first_n=(char*)malloc(sizeof(char)*k);
    strncpy(first_n,input,k);
    //we append .komp
    return strcat(first_n,".komp");
}

unsigned int* read_num_of_occurences(FILE* fp, char numofbits){
    if(numofbits!=8&&numofbits!=16){
        printf("[error] Unsupported number of bits!");
        return NULL;
    }
    unsigned int* occurences=(unsigned int*) calloc((numofbits==8?256:65536),sizeof(unsigned int));
    if(numofbits==8){
        unsigned char c;
        while(fscanf(fp,"%hhu",&c)!=EOF){
            (*(occurences+c))++;
        }
    }else{
        unsigned short s;
        while(fscanf(fp,"%hu",&s)!=EOF){
            (*(occurences+s))++;
        }
    }
    return occurences;
}

long get_file_size(FILE* fp){
    long filesize=0;
    if(fp){
        fseek(fp,0,SEEK_END);
        filesize = ftell(fp);
        fseek(fp,0,SEEK_SET);
    }
}

void close_file(FILE* fp){
    fclose(fp);
}
