#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct TreeNode {
  char name[101];
  int month;
  struct TreeNode* left;
  struct TreeNode* right;
  int height;
} TreeNode;

int getHeight(TreeNode* root) {
  return root ? root->height : -1;
}

int max(int a, int b) {
  return (a > b) ? a : b;
}

void updateHeight(TreeNode* root) {
  if (root) {
    root->height = 1 + max(getHeight(root->left), getHeight(root->right));
  }
}

int getBalanceFactor(TreeNode* root) {
  return root ? getHeight(root->left) - getHeight(root->right) : 0;
}

TreeNode* rightRotate(TreeNode* root) {
  TreeNode* parent = root->left;
  TreeNode* child = parent->right;
  parent->right = root;
  root->left = child;
  updateHeight(parent);
  updateHeight(root);
  return parent;
}

TreeNode* leftRotate(TreeNode* root) {
  TreeNode* parent = root->right;
  TreeNode* child = parent->left;
  parent->left = root;
  root->right = child;
  updateHeight(parent);
  updateHeight(root);
  return parent;
}

TreeNode* insertNode(TreeNode* root, const char* name, int month) {
  if (!root) {
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    if (!newNode) {
      fprintf(stderr, "Error: Memory allocation failed!\n");
      exit(EXIT_FAILURE);
    }
    strcpy(newNode->name, name);
    newNode->month = month;
    newNode->left = newNode->right = NULL;
    newNode->height = 1;
    return newNode;
  }

  if (month < root->month) {
    root->left = insertNode(root->left, name, month);
  } else if (month > root->month) {
    root->right = insertNode(root->right, name, month);
  } else {
    return root;
  }
  updateHeight(root);

  int bf = getBalanceFactor(root);
  if (bf > 1 && getBalanceFactor(root->left) >= 0) {
    return rightRotate(root);
  }
  if (bf > 1 && getBalanceFactor(root->left) < 0 ) {
    root->left = leftRotate(root->left);
    return rightRotate(root);
  }
  if (bf < -1 && getBalanceFactor(root->right) <= 0 ) {
    return leftRotate(root);
  }
  if (bf <-1 && getBalanceFactor(root->right) > 0) {
    root->right = rightRotate(root->right);
    return leftRotate(root);
  }

  return root;
}

TreeNode* getSuccessor(TreeNode* root) {
  while (root && root->left) {
    root = root->left;
  }
  return root;
}

TreeNode* deleteNode(TreeNode* root, int month) {
  if (!root) return root;

  if (month < root->month) {
    root->left = deleteNode(root->left, month);
  } else if (month > root->month) {
    root->right = deleteNode(root->right, month);
  } else {
    if (!root->left || !root->right) {
      TreeNode* temp = (root->left) ? root->left : root->right;
      if (!temp) {
        temp = root;
        root = NULL;
      } else {
        root = temp;
      }
      free(temp);
    } else {
      TreeNode* temp = getSuccessor(root);
      strcpy(root->name, temp->name);
      root->month = temp->month;
      root->right = deleteNode(root->right, temp->month);
    }
  }
  if (!root) return root;
  updateHeight(root);

  int bf = getBalanceFactor(root);
  if (bf > 1 && getBalanceFactor(root->left) >= 0) {
    return rightRotate(root);
  }
  if (bf > 1 && getBalanceFactor(root->left) < 0 ) {
    root->left = leftRotate(root->left);
    return rightRotate(root);
  }
  if (bf < -1 && getBalanceFactor(root->right) <= 0 ) {
    return leftRotate(root);
  }
  if (bf <-1 && getBalanceFactor(root->right) > 0) {
    root->right = rightRotate(root->right);
    return leftRotate(root);
  }

  return root;
}

int getMonth(TreeNode* root, const char* name) {
  if (!root) return -1;
  getMonth(root->left, name);
  if (strcmp(name, root->name) == 0) {
    return root->month;
  }
  getMonth(root->right, name);
}

void preOrder(TreeNode* root, int counter) {
  if (!root) return;
  if (counter == 0) {
    printf("Vice Chairman: %s %d month\n", root->name, root->month);
  } else {
    printf("- Member: %s %d month\n", root->name, root->month);
  }
  preOrder(root->left, counter++);
  preOrder(root->right, counter++);
}

// Chairman
// Vice chairman of the western region
// Vice chairman of the eastern region
// Others are mmebers
// The org only accept 1 member each month
// Every year, chairman will switch
int main() {
  TreeNode* root = NULL;
  int x;
  scanf("%d", &x); getchar();
  char name[101];
  int year;
  int month;
  int totalMonth;
  for (int i = 0; i < x; i++) {
    scanf("%s %d-%s", name, &year, &month); getchar();
    totalMonth = (2023 - year) * 12 + month - 12;
    insertNode(root, name, totalMonth);
  }
  printf("West\n");
  preOrder(root->left, 0);
  
  printf("East\n");
  preOrder(root->right, 0);

  return 0;
}