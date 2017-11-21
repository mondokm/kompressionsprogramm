#ifndef TREES_H
#define TREES_H

typedef struct node{
    struct node* left;
    struct node* right;
    unsigned short value;
    unsigned long frequency;
    
}node;

void free_tree(node* leaf);
node** build_node_array(unsigned long*,int);
node* build_node_tree(node**,int);

#endif