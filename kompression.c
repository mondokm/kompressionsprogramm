#include <stdio.h>
#include <stdlib.h>
#include "files.h"
#include "trees.h"

int main(int argc,char** argv){
    char str[50];
    scanf("%s",&str);
    printf("%s",create_filename(str));
    return 0;
}