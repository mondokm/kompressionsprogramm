#ifndef FILES_H
#define FILES_H

#include <stdio.h>

FILE* open_file(char*);
FILE* create_output_file(char*);

void close_file(FILE*); 

char* create_filename(char*);

unsigned int* read_num_of_occurences(FILE*, char);
long get_file_size(FILE*);

#endif