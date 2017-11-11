#ifndef TREES_H
#define TREES_H

typedef struct node{
    struct node* left;
    struct node* right;
    char value;
}node;

void free_tree(node* leaf);

#endif