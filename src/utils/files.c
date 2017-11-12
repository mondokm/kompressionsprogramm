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
    long filesize_MB=get_file_size(fp)/1000000;
    printf("Reading number of occurences. This may take a while!\n");
    unsigned long cnt=0;
    int k=0;
    unsigned long numofmbytes=cnt/10;
    printf("[status]");
    if(numofbits==8){
        int c;
        while((c=fgetc(fp))!=EOF){
          k++;
          (*(occurences+c))++;
          if(k>=100000){
              cnt+=1;
              numofmbytes=cnt/10;
              printf("\r[status] %luMB/%ldMB %2.2lf%%",numofmbytes,filesize_MB,((double)numofmbytes/filesize_MB)*100);
              k=0;
          }
        }
    }else{
        int upper,lower;
        while((upper=fgetc(fp))!=EOF){
            unsigned short num;
            if((lower=fgetc(fp))!=EOF){
                num=upper*256+lower;
            }else{
                num=upper;
            }
            k++;
            (*(occurences+num))++;
            if(k>=100000){
                cnt+=2;
                numofmbytes=cnt/10;
                printf("\r[status] %luMB/%ldMB %2.2lf%%",numofmbytes,filesize_MB,((double)numofmbytes/filesize_MB)*100);
                k=0;    
            }
        }
    }
    printf("\nFinished reading.\n");
    return occurences;
}

long get_file_size(FILE* fp){
    long filesize=0;
    if(fp){
        fseek(fp,0,SEEK_END);
        filesize = ftell(fp);
        fseek(fp,0,SEEK_SET);
    }
    return filesize;
}

void close_file(FILE* fp){
    fclose(fp);
}
