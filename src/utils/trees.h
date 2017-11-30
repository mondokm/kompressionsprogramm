#ifndef TREES_H
#define TREES_H

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

#endif