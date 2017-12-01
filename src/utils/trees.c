#include <stdlib.h>
#include <stdio.h>
#include "trees.h"

int compareNodes(const void*, const void*);

void free_tree(node* root){
    if(root->right!=NULL) free_tree(root->right);
    if(root->left!=NULL) free_tree(root->left);
    free(root);
}

list_node* build_nodeptr_list(unsigned long* occurences,int arr_size){
    printf("Building list of nodes.\n");
    list_node* list=NULL;
    unsigned short i;
    for(i=0;i<arr_size;i++) {
        list=add_list_node(list,newnode(i,*(occurences+i)));
    }
    printf("Done.\n");
    return list;
}

node* build_node_tree(list_node* list){
    printf("Building tree.\n");
    while(list->next!=NULL){
        node* newnode=(node*) malloc(sizeof(node));

        list_node* least_frequent=find_least_frequent(list);
        newnode->left=least_frequent->nodeptr;
        list=remove_from_list(list,least_frequent);

        least_frequent=find_least_frequent(list);
        newnode->right=least_frequent->nodeptr;
        list=remove_from_list(list,least_frequent);

        newnode->frequency=newnode->left->frequency+newnode->right->frequency;
        list=add_list_node(list,newnode);
    }
    printf("Done.\n");
    return list->nodeptr;
}

list_node* add_list_node(list_node* list,node* nodeptr){
    if(list==NULL) {
        list=(list_node*) malloc(sizeof(list_node));
        list->next=NULL;
        list->nodeptr=nodeptr;
        return list;
    }else{
        list_node* newhead=(list_node*) malloc(sizeof(list_node));
        newhead->next=list;
        newhead->nodeptr=nodeptr;
        return newhead;
    }
}

node* newnode(unsigned short value,unsigned long frequency){
    node* newnode=(node*)malloc(sizeof(node));
    newnode->value=value;
    newnode->frequency=frequency;
}

list_node* find_least_frequent(list_node* list){
    if(list->next==NULL){
        return list;
    }else{
        list_node* least_frequent=find_least_frequent(list->next);
        if(list->nodeptr->frequency<least_frequent->nodeptr->frequency) return list;
        else return least_frequent;
    }
}

list_node* remove_from_list(list_node* list,list_node* element){
    if(list==NULL) return NULL;
    if(list==element) return list->next;
    if(list->next!=NULL){
        if(list->next==element){
            list->next=list->next->next;
        } else remove_from_list(list->next,element);   
    }
    return list;
}

void print_list(list_node* list){
    printf("nodeptr: %d\n",list->nodeptr);
    if(list->next!=NULL)print_list(list->next);
}