#include <stdlib.h>
#include <stdio.h>
#include "huffman.h"

int compare_codelengths(const void*, const void*);

void free_tree(node* root){
    if(root->right!=NULL) free_tree(root->right);
    if(root->left!=NULL) free_tree(root->left);
    free(root);
}

list_node* build_nodeptr_list(unsigned long* occurences,int arr_size){
    printf("Building list of nodes.\n");
    list_node* list=NULL;
    int i;
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
    newnode->left=NULL;
    newnode->right=NULL;
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

int** build_codelength_array(node* tree,int arr_size){
    int** codelengths=(int**) malloc(arr_size*sizeof(int*));
    for(int i=0;i<arr_size;i++){
        *(codelengths+i)=(int*) malloc(2*sizeof(int));
        *(*(codelengths+i)+1)=i;
    }
    populate_codelength_array(tree,codelengths,0);
    return codelengths;
}

void populate_codelength_array(node* tree,int** arr,int length){
    if(tree->left==NULL&&tree->right==NULL){
        **(arr+(tree->value))=length;
    }else{
        populate_codelength_array(tree->left,arr,length+1);
        populate_codelength_array(tree->right,arr,length+1);
    }
}

mpz_t* build_dictionary(int** arr,int arr_size){
    mpz_t* dictionary=(mpz_t*) malloc(arr_size*sizeof(mpz_t));
    qsort(arr,arr_size,sizeof(int*),compare_codelengths);
    mpz_init(dictionary[0]);
    for(int i=1;i<arr_size;i++){
        mpz_init(dictionary[i]);
        mpz_add_ui(dictionary[i],dictionary[i-1],1);
        mpz_mul_2exp(dictionary[i],dictionary[i],**(arr+i)-**(arr+i-1));
        //mpz_out_str(stdout,2,dictionary[i]);
        //printf(" %d\n", **(arr+i));
    }
    return dictionary;
}

int compare_codelengths(const void* a, const void* b){
    int length1=*(*((int**)a)),length2=*(*((int**)b)),value1=*(*((int**)a)+1),value2=*(*((int**)b)+1);
    if(length1<length2) return -1;
    else if(length1==length2){
        return value1-value2;
    } else return 1;
}

int maxcodelength(int** arr,int arr_size){
    int max=**arr;
    for(int i=0;i<arr_size;i++){
        //if()
    }
    return max;
}