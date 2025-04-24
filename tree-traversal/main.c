//
//  main.c
//  CSO_Recitation_5
//
//  Created by Jack Yang on 10/6/22.
//

// Tree traversal in C
#include <stdio.h>
#include <stdlib.h>

struct node {
  int data;
  struct node* left;
  struct node* right;
};

// Inorder traversal
void printInorderTraversal(struct node* root) {
    if(root == NULL){
        return;
    }
    
    printInorderTraversal(root -> left);
    printf("%d ", root -> data);
    printInorderTraversal(root -> right);

    
    }
    

// Preorder traversal
void printPreorderTraversal(struct node* root) {
    if(root == NULL){
        return;
    }
    
    printf("%d ", root -> data);
    printPreorderTraversal(root -> left);
    printPreorderTraversal(root -> right);


}

// Postorder traversal
void printPostorderTraversal(struct node* root) {
    if(root == NULL){
        return;
    }
    
    printPostorderTraversal(root -> left);
    printPostorderTraversal(root -> right);
    printf("%d ", root -> data);



}

// Create a new Node
struct node* createNode(value) {

    struct node bob;
    bob.data = value;
    bob.right = NULL;
    bob.left = NULL;
    
    struct node *ptr = malloc(sizeof(struct node));
    ptr-> data = value;
    ptr-> left = NULL;
    ptr -> right = NULL;
    
    
    return ptr;
    
}

// Insert on the left of the node
struct node* insertLeft(struct node* root, int value) {
    
    struct node bob;
    struct node *ptr = malloc(sizeof(struct node));
    
    
    ptr -> data = value;
    ptr -> left = NULL;
    ptr -> right = NULL;
    root -> left = ptr;
    
    
    return NULL;
    
   
}

// Insert on the right of the node
struct node* insertRight(struct node* root, int value) {
    struct node bob;
    struct node *ptr = malloc(sizeof(struct node));
    
    ptr-> data = value;
    ptr-> left = NULL;
    ptr -> right = NULL;
    root -> right = ptr;
    
    
    
    return NULL;

    
}

int main() {
  struct node* root = createNode(1);
  insertLeft(root, 2);
    
  insertRight(root, 3);
    
  insertLeft(root->left, 4);
  insertRight(root->left, 5);
  insertLeft(root->right, 6);
  insertRight(root->right, 7);
    
    

  printf("Inorder traversal   -> ");
  printInorderTraversal(root);

  printf("\nPreorder traversal  -> ");
  printPreorderTraversal(root);

  printf("\nPostorder traversal -> ");
  printPostorderTraversal(root);
  printf("\n");
}


