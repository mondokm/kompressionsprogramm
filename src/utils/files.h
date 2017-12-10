#ifndef FILES_H
#define FILES_H

#include <stdio.h>
#include <gmp.h>
#include "huffman.h"

typedef enum compression_mode {COMPRESSION,DECOMPRESSION} compression_mode;
typedef enum compression_bitlength {BYTE, WORD} compression_bitlength;

FILE* write_file(char*);
FILE* read_file(char*);

void close_file(FILE*); 

char* create_filename(char*,compression_mode);

unsigned long* read_num_of_occurences(FILE*, char,short*);
void write_codelengths(FILE*,unsigned short**, char,unsigned char, long);
unsigned short** read_codelengths(FILE*,char*,short*,long*);
void compress_file(char**,unsigned short**, char, FILE*, FILE*, char, int);
void decompress_file(node*,FILE*, FILE*, char, unsigned char, int, long);
long get_file_size(FILE*);
int maxcodelength(unsigned short**,int);
char create_header_byte(compression_bitlength,compression_bitlength,int);

#endif