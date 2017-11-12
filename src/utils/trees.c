#include <stdlib.h>
#include "trees.h"

void free_tree(node* root){
    if(root->right!=NULL) free_tree(root->right);
    if(root->left!=NULL) free_tree(root->left);
    free(root);
}