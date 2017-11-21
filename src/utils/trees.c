#include <stdlib.h>
#include <stdio.h>
#include "trees.h"

void free_tree(node* root){
    if(root->right!=NULL) free_tree(root->right);
    if(root->left!=NULL) free_tree(root->left);
    free(root);
}

node** build_node_array(unsigned long* occurences,int arr_size){
    node** arr=(node**)malloc(arr_size*sizeof(node*));
    unsigned short i;
    for(i=0;i<arr_size;i++) {
        *(arr+i)=(node*)malloc(sizeof(node));
        (*(arr+i))->value=i;
        (*(arr+i))->frequency=*(occurences+i);
        (*(arr+i))->left=NULL;
        (*(arr+i))->right=NULL;
    }
    return arr;
}

node* build_node_tree(node** arr,int arr_size){
    node* newnode;
    char* used=(char*)calloc(256,sizeof(char));
    while(arr_size>1){
        int lowestindex,secondlowestindex;
        //initializing lowest and secondlowest with the more frequent one of the first two
        lowestindex=(*(arr))->frequency>(*(arr+1))->frequency?0:1;
        secondlowestindex=lowestindex?0:1;
        //finding the two nodes with the lowest frequency
        for(int i=0;i<256;i++){
            if(!used[i]&&(*(arr+i))->frequency<(*(arr+secondlowestindex))->frequency){
                if((*(arr+i))->frequency<(*(arr+lowestindex))->frequency) {
                    secondlowestindex=lowestindex;
                    lowestindex=i;
                }else {
                    secondlowestindex=i;
                }
            }
        }
        newnode=(node*) malloc(sizeof(node));
        newnode->left=*(arr+lowestindex);
        newnode->right=*(arr+secondlowestindex);
        newnode->frequency=(*(arr+lowestindex))->frequency+(*(arr+secondlowestindex))->frequency;
        arr_size--;
        *(arr+secondlowestindex)=newnode;
        used[lowestindex]=1;
    }
    free(used);
    return *arr;
}
