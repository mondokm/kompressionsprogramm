#include "files.h"
#include <string.h>
#include <stdlib.h>

extern void print_num(long,char*,int,int);
extern int read_byte(int,char*);
unsigned char binary_to_decimal(char*);
int write_to_file(FILE*,char*, int, char*, int);
int maxcodelength(int**,int);

FILE* read_file(char* name){
    return fopen(name,"r");
}

FILE* write_file(char* name){
    return fopen(name,"w");
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
    //if it has an extension, we cut it off
    if(i==0) k=strlen(input);
    char* first_n=(char*)malloc(sizeof(char)*k);
    strncpy(first_n,input,k);
    //we append .komp
    return strcat(first_n,".komp");
}

unsigned long* read_num_of_occurences(FILE* fp, char numofbits){
    if(numofbits!=8&&numofbits!=16){
        printf("[error] Unsupported number of bits!");
        return NULL;
    }
    //allocating memory
    unsigned long* occurences=(unsigned long*) calloc((numofbits==8?256:65536),sizeof(unsigned long));
    //calculating filesize in MBytes
    long filesize_MB=get_file_size(fp)/1000000;
    char str[12];
    sprintf(str,"MB/%ldMB ",filesize_MB);
    //initializing progress counter
    unsigned long cnt=0, numofmbytes=cnt/10;
    //k is used to limit the number of printfs called
    int k=0;
    
    printf("Reading number of occurences. This may take a while!\n");
    printf("\e[?25l[status]");

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
              print_num(numofmbytes,str,12,(int)(((double)numofmbytes/filesize_MB)*100));
              k=0;
          }
        }
    //16 bit mode    
    }else{
        int upper,lower;
        int num;
        //reading upper 8 bits
        while((upper=fgetc(fp))!=EOF){
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
                //printf("\r[status] %luMB/%ldMB %2.2lf%%",numofmbytes,filesize_MB,((double)numofmbytes/filesize_MB)*100);
                print_num(numofmbytes,str,12,(int)(((double)numofmbytes/filesize_MB)*100));
                k=0;    
            }
        }
    }
    printf("\r\33[2K\e[?25hFinished reading.\n");
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

void compress_file(char** dictionary, int** codelengths, char numofbits, FILE* file_in, FILE* file_out){
    if(numofbits!=8&&numofbits!=16){
        printf("[error] Unsupported number of bits!");
        return;
    }
    //calculating filesize in MBytes
    long filesize_MB=get_file_size(file_in)/1000000;
    char str[12];
    sprintf(str,"MB/%ldMB ",filesize_MB);
    //initializing progress counter
    unsigned long cnt=0, numofmbytes=cnt/10;
    //k is used to limit the number of printfs called
    int k=0;
    
    char* queue=(char*) malloc((maxcodelength(codelengths,numofbits==8?256:65536)+8)*sizeof(char));
    int size_of_queue=0;

    printf("Compresing file. This may take a while!\n");
    printf("\e[?25l[status]");

    //8 bit mode
    if(numofbits==8){
        int c;
        while((c=fgetc(file_in))!=EOF){
          k++;
          size_of_queue=write_to_file(file_out,queue,size_of_queue,*(dictionary+c),**(codelengths+c));
          //updating status every 100 KBytes
          if(k>=100000){
              cnt+=1;
              numofmbytes=cnt/10;
              //printf("\r[status] %luMB/%ldMB %2.2lf%%",numofmbytes,filesize_MB,((double)numofmbytes/filesize_MB)*100);
              print_num(numofmbytes,str,12,(int)(((double)numofmbytes/filesize_MB)*100));
              k=0;
          }
        }
    //16 bit mode    
    }else{
        int upper,lower;
        int num;
        //reading upper 8 bits
        while((upper=fgetc(file_in))!=EOF){
            //reading lower 8 bits
            if((lower=fgetc(file_in))!=EOF){
                num=upper*256+lower;
            }else{
                num=upper;
            }
            k++;
            size_of_queue=write_to_file(file_out,queue,size_of_queue,*(dictionary+num),**(codelengths+num));
            //updating status every 100 KBytes
            if(k>=50000){
                cnt+=1;
                numofmbytes=cnt/10;
                //printf("\r[status] %luMB/%ldMB %2.2lf%%",numofmbytes,filesize_MB,((double)numofmbytes/filesize_MB)*100);
                print_num(numofmbytes,str,12,(int)(((double)numofmbytes/filesize_MB)*100));
                k=0;    
            }
        }
    }
    printf("\r\33[2K\e[?25hFinished compressing.\n");
    free(queue);
}

int write_to_file(FILE* fp,char* queue, int size_of_queue, char* data, int size){
    for(int i=0;i<size;i++){
        *(queue+size_of_queue+i)=*(data+i);
    }
    size_of_queue+=size;
    int bytes_written=0;
    while(size_of_queue>=8){
        fputc(binary_to_decimal(queue+bytes_written*8),fp);
        size_of_queue-=8;
    }
    for(int i=0;i<size_of_queue;i++){
        *(queue+i)=*(queue+i+bytes_written*8);
    }
    return size_of_queue;
}

unsigned char binary_to_decimal(char* binary){
    char num=0;
    for(int i=0;i<8;i++){
        num+=(*(binary+i)-'0')*(1<<8-i-1);
    }
    return num;
}

int maxcodelength(int** arr,int arr_size){
    int max=**arr;
    for(int i=0;i<arr_size;i++){
        if(**(arr+i)>max) max=**(arr+i);
    }
    return max;
}
