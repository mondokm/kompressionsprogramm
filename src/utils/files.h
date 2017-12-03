#ifndef FILES_H
#define FILES_H

#include <stdio.h>
#include <gmp.h>

FILE* write_file(char*);
FILE* read_file(char*);
FILE* create_output_file(char*);

void close_file(FILE*); 

char* create_filename(char*);

unsigned long* read_num_of_occurences(FILE*, char);
void compress_file(char**,int**, char, FILE*, FILE*);
long get_file_size(FILE*);

#endif