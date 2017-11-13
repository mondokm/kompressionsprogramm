#include "files.h"
#include <string.h>
#include <stdlib.h>

extern void print_num(long,char*,int);

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
    //allocating memory
    unsigned int* occurences=(unsigned int*) calloc((numofbits==8?256:65536),sizeof(unsigned int));
    //calculating filesize in MBytes
    long filesize_MB=get_file_size(fp)/1000000;
    char str[12];
    sprintf(str,"KB/%ldKB",filesize_MB);
    //initializing progress counter
    unsigned long cnt=0, numofmbytes=cnt/10;
    //k is used to limit the number of printfs called
    int k=0;
    
    printf("Reading number of occurences. This may take a while!\n");
    printf("[status]");
    //8 bit mode
    if(numofbits==8){
        int c;
        while((c=fgetc(fp))!=EOF){
          k++;
          (*(occurences+c))++;
          //updating status every 100 KBytes
          if(k>=100000){
              cnt+=1;
              numofmbytes=cnt/10;
              //printf("\r[status] %luMB/%ldMB %2.2lf%%",numofmbytes,filesize_MB,((double)numofmbytes/filesize_MB)*100);
              print_num(numofmbytes,str,12);
              k=0;
          }
        }
    //16 bit mode    
    }else{
        int upper,lower;
        //reading upper 8 bits
        while((upper=fgetc(fp))!=EOF){
            unsigned short num;
            //reading lower 8 bits
            if((lower=fgetc(fp))!=EOF){
                num=upper*256+lower;
            }else{
                num=upper;
            }
            k++;
            (*(occurences+num))++;
            //updating status every 100 KBytes
            if(k>=50000){
                cnt+=1;
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
