#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "huffman.h"
#include "files.h"

int compare_codelengths(const void*, const void*);
char* create_code(char*,int);
unsigned char binary_to_decimal(char*);
void add_to_tree(node*, node*, char*, unsigned short);

void free_tree(node* root){
    if(root->right!=NULL&&root->left!=NULL) {
        free_tree(root->right);
        free_tree(root->left);
    }
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

node* build_node_tree(list_node* list,int arr_size){
    printf("Building tree.\n");
    int remaining=arr_size;
    printf("\e[?25l[status] %d/%d remaining, %.2f%%",remaining,arr_size,((double) 0/arr_size)*100);
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
        printf("\r[status] %d/%d remaining, %.2f%% ",--remaining,arr_size,((double) (arr_size-remaining)/arr_size)*100);
    }
    printf("\r\33[2K\e[?25hDone.\n");
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

unsigned short** build_codelength_array(node* tree,int arr_size){
    unsigned short** codelengths=(unsigned short**) malloc(arr_size*sizeof(int*));
    for(int i=0;i<arr_size;i++){
        *(codelengths+i)=(unsigned short*) malloc(2*sizeof(unsigned short));
        *(*(codelengths+i)+1)=i;
    }
    populate_codelength_array(tree,codelengths,0);
    return codelengths;
}

void populate_codelength_array(node* tree,unsigned short** arr,int length){
    if(tree->left==NULL&&tree->right==NULL){
        **(arr+(tree->value))=length;
    }else{
        populate_codelength_array(tree->left,arr,length+1);
        populate_codelength_array(tree->right,arr,length+1);
    }
}

mpz_t* build_codes(unsigned short** arr,int arr_size){
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

char** build_dictionary(mpz_t* codes, unsigned short** codelengths, int arr_size){
    char** dictionary=(char**) malloc(arr_size*sizeof(char*));
    for(int i=0;i<arr_size;i++){
        char* str=mpz_get_str(NULL,2,codes[i]);
        *(dictionary+*(*(codelengths+i)+1))=create_code(str,**(codelengths+i));
    }
    return dictionary;
}

int compare_codelengths(const void* a, const void* b){
    int length1=*(*((unsigned short**)a)),length2=*(*((unsigned short**)b)),value1=*(*((unsigned short**)a)+1),value2=*(*((unsigned short**)b)+1);
    if(length1<length2) return -1;
    else if(length1==length2){
        return value1-value2;
    } else return 1;
}

char* create_code(char* str,int len){
     char* code=(char*) malloc((len+1)*sizeof(int));
     int n=len-strlen(str);
     for(int i=0;i<n;i++){
         *(code+i)='0';
     }
     for(int j=n;j<len;j++){
         *(code+j)=*(str+j-n);
     }
     *(code+len)='\0';
     return code;
}

node* build_tree_from_codes(char** codes,int arr_size){
    node* head=(node*) malloc(sizeof(node));
    head->left=NULL;
    head->right=NULL;
    head->value=-1;
    for(int i=0;i<arr_size;i++){
        add_to_tree(head,head,*(codes+i),i);
    }
    return head;
}

void add_to_tree(node* tree, node* head, char* code, unsigned short value){
    if(strlen(code)==1){
        if(tree->left==NULL) tree->left=(node*) malloc(sizeof(node));
        if(tree->right==NULL) tree->right=(node*) malloc(sizeof(node));
        if(*code=='0'){
            tree->left=(node*) malloc(sizeof(node));
            tree=tree->left;
        }else{
            tree->right=(node*) malloc(sizeof(node));
            tree=tree->right;
        }
        tree->value=value;
        tree->left=head;
        tree->right=NULL;
        
    }else{
        if(tree->left==NULL) tree->left=(node*) malloc(sizeof(node));
        if(tree->right==NULL) tree->right=(node*) malloc(sizeof(node));
        if(*(code++)=='0'){
            add_to_tree(tree->left,head,code,value);            
        }else{
            add_to_tree(tree->right,head,code,value);
        }
    }
}

int search_in_tree(node** tree, char code){
    if(code==0) *tree=(*tree)->left;
    else *tree=(*tree)->right;
    if((*tree)->right==NULL) {
        unsigned short val=(*tree)->value;    
        *tree=(*tree)->left;
        return val;
    }
    else return -1;  
}