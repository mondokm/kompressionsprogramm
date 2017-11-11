#ifndef FILES_H
#define FILES_H

#include <stdio.h>

FILE* open_file(char*);
FILE* create_output_file(char*);

void close_file(FILE*); 
char* create_filename(char*);
int* read_probabilities(FILE*, char);

#endif