#ifndef TREES_H
#define TREES_H

#include <gmp.h>

typedef struct node{
    struct node* left;
    struct node* right;
    unsigned short value;
    unsigned long frequency;
}node;

typedef struct list_node{
    node* nodeptr;
    struct list_node* next;
}list_node;

void free_tree(node* leaf);
list_node* add_list_node(list_node*,node*);
list_node* find_least_frequent(list_node*);
list_node* remove_from_list(list_node*,list_node*);
node* newnode(unsigned short,unsigned long);
list_node* build_nodeptr_list(unsigned long*,int);
node* build_node_tree(list_node*,int);
mpz_t* build_codes(unsigned short**,int);
char** build_dictionary(mpz_t*,unsigned short**,int);
unsigned short** build_codelength_array(node*,int);
void populate_codelength_array(node*,unsigned short**,int);
void print_list(list_node*);
node* build_tree_from_codes(char**,int);
int search_in_tree(node**,char);

#endif