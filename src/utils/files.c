#include "files.h"
#include <string.h>
#include <stdlib.h>

#if defined(__linux__) && defined(__amd64__)
extern void print_num(long,char*,int,int);
#endif

extern int read_byte(int,char*);
unsigned char binary_to_decimal(char*);
int write_to_file(FILE*,char*, int, char*, int);
char read_bit(FILE*,char*);

FILE* read_file(char* name){
    return fopen(name,"rb");
}

FILE* write_file(char* name){
    return fopen(name,"wb");
}

char* create_filename(char* input,compression_mode mode){
    
    if(mode==COMPRESSION){
        char* name=(char*)malloc((strlen(input)+5)*sizeof(char));
        strcpy(name,input);
        return strcat(name,".komp");
    }else{
        //check if input has an extension
        int i,k;
        for(i=0;i<strlen(input);i++){
            if(*(input+i)=='.') k=i;
        }
        //if it has an extension, we cut it off
        if(i==0) k=strlen(input);
        char* first_n=(char*)malloc(sizeof(char)*k);
        strncpy(first_n,input,k);
        return first_n;
    }
}

unsigned long* read_num_of_occurences(FILE* fp, char numofbits,char* leftover){
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
          //printf("%d\n",c);
          //updating status every 100 KBytes
          if(k>=100000){
              cnt+=1;
              numofmbytes=cnt/10;
              #if defined(__linux__) && defined(__amd64__)
              print_num(numofmbytes,str,12,(int)(((double)numofmbytes/filesize_MB)*100));
              #else
              printf("\r[status] %luMB/%ldMB %2.2lf%%",numofmbytes,filesize_MB,((double)numofmbytes/filesize_MB)*100);
              #endif
              k=0;
          }
        }
    //16 bit mode    
    }else{
        unsigned short* num;
        int bytes_read;
        //reading upper 8 bits
        while(bytes_read=fread(num,1,2,fp)){
            k++;
            if(bytes_read==2) (*(occurences+*num))++;
            else *leftover=*num;
            //updating status every 100 KBytes
            if(k>=50000){
                cnt+=1;
                numofmbytes=cnt/10;
                #if defined(__linux__) && defined(__amd64__)
                print_num(numofmbytes,str,12,(int)(((double)numofmbytes/filesize_MB)*100));
                #else
                printf("\r[status] %luMB/%ldMB %2.2lf%%",numofmbytes,filesize_MB,((double)numofmbytes/filesize_MB)*100);
                #endif
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

void write_codelengths(FILE* fp, unsigned short** codelengths, char header, char leftover){
    printf("Writing codelengths.\n");
    printf("\e[?25l[status]");
    fputc(header,fp);
    int arr_size=(!header&1)?256:65536;
    if(!header&2){
        for(int i=0;i<arr_size;i++){
            fputc(**(codelengths+i),fp);
            printf("\r[status] %d/%d %.2f%%",i,arr_size,((double)i/arr_size)*100);
            //printf("%d\n",**(codelengths+i));
        }
    }else{
        for(int i=0;i<arr_size;i++){
            fwrite(*(codelengths+i),2,1,fp);
            printf("\r[status] %d/%d %.2f%%",i,arr_size,((double)i/arr_size)*100);
            //printf("%d\n",**(codelengths+i));
        }
    }
    if(header&4)fwrite(&leftover,1,1,fp);
    printf("\r\33[2KDone.\n");

}

void compress_file(char** dictionary, unsigned short** codelengths, char numofbits, FILE* file_in, FILE* file_out){
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
          //printf("%s %d\n",*(dictionary+c),c);
          size_of_queue=write_to_file(file_out,queue,size_of_queue,*(dictionary+c),**(codelengths+c));
          //updating status every 100 KBytes
          if(k>=100000){
              cnt+=1;
              numofmbytes=cnt/10;
              #if defined(__linux__) && defined(__amd64__)
              print_num(numofmbytes,str,12,(int)(((double)numofmbytes/filesize_MB)*100));
              #else
              printf("\r[status] %luMB/%ldMB %2.2lf%%",numofmbytes,filesize_MB,((double)numofmbytes/filesize_MB)*100);
              #endif
              k=0;
          }
        }
    //16 bit mode    
    }else{
        unsigned short* num;
        //reading upper 8 bits
        while(fread(num,sizeof(unsigned short),1,file_in)){
            //reading lower 8 bits
            k++;
            size_of_queue=write_to_file(file_out,queue,size_of_queue,*(dictionary+*num),**(codelengths+*num));
            //updating status every 100 KBytes
            if(k>=50000){
                cnt+=1;
                numofmbytes=cnt/10;
                #if defined(__linux__) && defined(__amd64__)
                print_num(numofmbytes,str,12,(int)(((double)numofmbytes/filesize_MB)*100));
                #else
                printf("\r[status] %luMB/%ldMB %2.2lf%%",numofmbytes,filesize_MB,((double)numofmbytes/filesize_MB)*100);
                #endif
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
        bytes_written++;
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

int maxcodelength(unsigned short** arr,int arr_size){
    int max=**arr;
    for(int i=0;i<arr_size;i++){
        if(**(arr+i)>max) max=**(arr+i);
    }
    return max;
}

unsigned short** read_codelengths(FILE* fp,char* header,char* leftover){
    *header=fgetc(fp);
    int arr_size=((!header&1)?256:65536);
    unsigned short** codelengths=(unsigned short**) malloc(arr_size*sizeof(unsigned short*));
    printf("Reading codelengths.\n");
    if(!header&2){
        int c;
        for(int i=0;i<arr_size;i++){
            c=fgetc(fp);
            *(codelengths+i)=(unsigned short*) malloc(2*sizeof(unsigned short));
            **(codelengths+i)=c;
            *(*(codelengths+i)+1)=i;
        }
    }else{
        for(int i=0;i<arr_size;i++){
            *(codelengths+i)=(unsigned short*) malloc(2*sizeof(unsigned short));
            fread(*(codelengths+i),sizeof(unsigned short),1,fp);
            *(*(codelengths+i)+1)=i;
            printf("%ud ",**(codelengths+i));
        }
    }
    if(header&4) fread(leftover,1,1,fp);
    printf("Done.\n");
    return codelengths;

}

void decompress_file(node* tree,FILE* file_in, FILE* file_out, char numofbits){
    char c;
    char* buffer=(char*)malloc(sizeof(char));
    int num;
    printf("Decompressing file.\n");
    while((c=read_bit(file_in,buffer))!=EOF){
        //printf("%d",c);
        if((num=search_in_tree(&tree,c))!=-1){
            //printf(" num: %d\n",num);
            fwrite(&num,numofbits/8,1,file_out);
        } 
    }
    printf("Finished decompressing.\n");
}

char read_bit(FILE* fp,char* buffer){
    static char buffer_size=0;
    if(buffer_size==0){
        if(fread(buffer,sizeof(char),1,fp)){
            buffer_size=8;
        }
    }
    if(buffer_size==0) return EOF;
    else {
        buffer_size--;
        return ((*(buffer)&(1<<buffer_size)))>>(buffer_size);
    }
}

char create_header_byte(compression_bitlength data_bitlength,compression_bitlength codelength_length,int leftover){
    char header=0;
    if(data_bitlength==WORD) header|=1;
    if(codelength_length==WORD) header|=2;
    if(leftover) header|=4;
    return header;
}