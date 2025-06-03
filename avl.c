#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
  char name[101];
  double score;
  struct Node* left;
  struct Node* right;
  int height;
} Node;

Node* createNode(const char* name, double score) {
  Node* newNode = (Node*)malloc(sizeof(Node));
  if (!newNode) {
    fprintf(stderr, "Memory allocation failed.\n");
    return NULL;
  }
  strcpy(newNode->name, name);
  newNode->score = score;
  newNode->left = NULL;
  newNode->right = NULL;
  newNode->height = 1;
  return newNode;
}

double max(double a, double b) {
  return (a > b) ? a : b;
}

int getHeight(Node* root) {
  if (!root) return -1;
  return root->height;
}

void updateHeight(Node* root) {
  if (root)
    root->height = 1 + max(getHeight(root->left), getHeight(root->right));
}

int getBalanceFactor(Node* root) {
  if (!root) return 0;
  return getHeight(root->left) - getHeight(root->right);
}

Node* leftRotate(Node* node) {
  Node* B = node->right;
  Node* Y = B->left;

  B->left = node;
  node->right = Y;

  updateHeight(node);
  updateHeight(B);

  return B;
}

Node* rightRotate(Node* node) {
  Node* B = node->left;
  Node* Y = B->right;

  B->right = node;
  node->left = Y;

  updateHeight(node);
  updateHeight(B);

  return B;
}

Node* search(Node* root, double score) {
  while (root != NULL && score != root->score) {
    if (score < root->score) root = root->left;
    else root = root->right;
  }
  return root;
}

Node* insert(Node* root, const char* name, double score) {
  if (!root) return createNode(name, score);

  if (score < root->score) root->left = insert(root->left, name, score);
  else if (score > root->score) root->right = insert(root->right, name, score);
  else return root;

  updateHeight(root);
  int bf = getBalanceFactor(root);

  if (bf > 1 && getBalanceFactor(root->left) >= 0)
    return rightRotate(root);
  if (bf < -1 && getBalanceFactor(root->right) <= 0)
    return leftRotate(root);
  if (bf > 1 && getBalanceFactor(root->left) < 0) {
    root->left = leftRotate(root->left);
    return rightRotate(root);
  }
  if (bf < -1 && getBalanceFactor(root->right) > 0) {
    root->right = rightRotate(root->right);
    return leftRotate(root);
  }

  return root;
}

Node* findSuccessor(Node* root) {
  while (root && root->left)
    root = root->left;
  return root;
}

Node* delete(Node* root, double score) {
  if (!root) return root;

  if (score < root->score)
    root->left = delete(root->left, score);
  else if (score > root->score)
    root->right = delete(root->right, score);
  else {
    if (!root->left || !root->right) {
      Node* temp = (root->left) ? root->left : root->right;
      free(root);
      return temp;
    }

    Node* temp = findSuccessor(root->right);
    strcpy(root->name, temp->name);
    root->score = temp->score;
    root->right = delete(root->right, temp->score);
  }

  updateHeight(root);
  int bf = getBalanceFactor(root);

  if (root) {
    if (bf > 1 && getBalanceFactor(root->left) >= 0)
      return rightRotate(root);
    if (bf < -1 && getBalanceFactor(root->right) <= 0)
      return leftRotate(root);
    if (bf > 1 && getBalanceFactor(root->left) < 0) {
      root->left = leftRotate(root->left);
      return rightRotate(root);
    }
    if (bf < -1 && getBalanceFactor(root->right) > 0) {
      root->right = rightRotate(root->right);
      return leftRotate(root);
    }
  }

  return root;
}

void inOrder(Node* root) {
  if (!root) return;
  inOrder(root->left);
  printf("Name: %s, Score: %.2f\n", root->name, root->score);
  inOrder(root->right);
}

void preOrder(Node* root) {
  if (!root) return;
  printf("Name: %s, Score: %.2f\n", root->name, root->score);
  preOrder(root->left);
  preOrder(root->right);
}

void postOrder(Node* root) {
  if (!root) return;
  postOrder(root->left);
  postOrder(root->right);
  printf("Name: %s, Score: %.2f\n", root->name, root->score);
}

int main() {
  Node* root = NULL;

  root = insert(root, "Alice", 40.0);
  root = insert(root, "Bob", 30.0);
  root = insert(root, "Charlie", 50.0);
  root = insert(root, "Dave", 25.0);
  root = insert(root, "Eve", 35.0);

  printf("\nAVL Tree After Insertions (In-Order Traversal):\n");
  inOrder(root);

  printf("\nAVL Tree (Pre-Order Traversal):\n");
  preOrder(root);

  printf("\nAVL Tree (Post-Order Traversal):\n");
  postOrder(root);

  root = delete(root, 30.0);
  printf("\nAVL Tree After Deleting Bob (30.0):\n");
  inOrder(root);

  Node* found = search(root, 50.0);
  if (found)
    printf("\nSearch result: Found %s with score %.2f\n", found->name, found->score);
  else
    printf("\nScore not found\n");

  return 0;
}