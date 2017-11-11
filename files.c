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

void close_file(FILE* fp){
    fclose(fp);
}
