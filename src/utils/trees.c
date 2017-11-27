#include <stdlib.h>
#include <stdio.h>
#include "trees.h"



int compareNodes(const void*, const void*);

void free_tree(node* root){
    if(root->right!=NULL) free_tree(root->right);
    if(root->left!=NULL) free_tree(root->left);
    free(root);
}

node* build_nodeptr_list(unsigned long* occurences,int arr_size){
    node* list;
    unsigned short i;
    for(i=0;i<arr_size;i++) {
        list=add_list_node(list,newnode(i,*(occurences+i)));
    }
    return list;
}

node* build_node_tree(list_node* list,int arr_size){
    unsigned char i;
    for(i=0;i<arr_size;i++){
        node* newnode=(node*) malloc(sizeof(node));
        newnode->left=remove_least_frequent(list);
        newnode->right=remove_least_frequent(list);
        newnode->frequency=newnode->left->frequency+newnode->right->frequency;
        add_list_node(list,newnode);
    }
    return list->nodeptr;
}

node* add_list_node(list_node* list,node* nodeptr){
    list_node* newhead=(list_node*) malloc(sizeof(list_node));
    newhead->next=list;
    newhead->nodeptr=nodeptr;
    return newhead;
}

node* newnode(unsigned short value,unsigned long frequency){
    node* newnode=(node*)malloc(sizeof(node));
    newnode->value=value;
    newnode->frequency=frequency;
}

node* remove_least_frequent(list_node* list){

}

/*int compareNodes(const void* a,const void* b){
    return ((node*)b)->frequency-((node*)a)->frequency;
}*/