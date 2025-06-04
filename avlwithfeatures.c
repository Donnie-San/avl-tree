#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct TreeNode {
    char username[101];
    double score;
    struct TreeNode* left;
    struct TreeNode* right;
    int height;
} TreeNode;

typedef struct QueueNode {
    TreeNode* treeNode;
    struct QueueNode* next;
} QueueNode;

typedef struct Queue {
    QueueNode* head;
    QueueNode* tail;
} Queue;

Queue* createQueue();
void enqueue(Queue* q, TreeNode* treeNode);
TreeNode* dequeue(Queue* q);
void freeQueue(Queue* q);

TreeNode* createTreeNode(const char* username, double score);
int getHeight(TreeNode* node);
int max(int a, int b);
void updateHeight(TreeNode* node);
int getBalanceFactor(TreeNode* node);
TreeNode* rightRotate(TreeNode* y);
TreeNode* leftRotate(TreeNode* x);
TreeNode* insertNode(TreeNode* node, const char* username, double score);
TreeNode* minValueNode(TreeNode* node);
TreeNode* deleteNode(TreeNode* root, double score);
void preOrder(TreeNode* root);
void inOrder(TreeNode* root);
void postOrder(TreeNode* root);
void levelOrder(TreeNode* root);
TreeNode* searchNode(TreeNode* root, double score);
void freeTree(TreeNode* root);

int main() {
    TreeNode* root = NULL;
    root = insertNode(root, "ShadowX", 842);
    root = insertNode(root, "ByteKnight", 655);
    root = insertNode(root, "QuantumD", 923);
    root = insertNode(root, "NeoMatrix", 712);
    root = insertNode(root, "Phantom99", 501);
    root = insertNode(root, "CyberWolf", 768);
    root = insertNode(root, "ZetaCode", 879);
    root = insertNode(root, "PixelFiend", 634);
    root = insertNode(root, "AI_Guru", 967);
    root = insertNode(root, "RogueDev", 589);
    root = deleteNode(root, 501);

    printf("Pre Order Traversal: ");
    preOrder(root);
    printf("\n");

    printf("In Order Traversal: ");
    inOrder(root);
    printf("\n");

    printf("Post Order Traversal: ");
    postOrder(root);
    printf("\n");

    printf("Level Order Traversal: ");
    levelOrder(root);
    printf("\n");

    double searchScore = 842;
    TreeNode* found = searchNode(root, searchScore);
    if (found) {
        printf("\nFound user with score %.2f: %s\n", searchScore, found->username);
    } else {
        printf("\nNo user found with score %.2f\n", searchScore);
    }

    freeTree(root);
    return 0;
}

Queue* createQueue() {
    Queue* newQueue = (Queue*)malloc(sizeof(Queue));
    if (!newQueue) {
        fprintf(stderr, "Error: Memory allocation failed for queue!\n");
        exit(EXIT_FAILURE);
    }
    newQueue->head = NULL;
    newQueue->tail = NULL;
    return newQueue;
}

void enqueue(Queue* q, TreeNode* treeNode) {
    if (!q) return;
    
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    if (!newNode) {
        fprintf(stderr, "Error: Memory allocation failed for queue node!\n");
        exit(EXIT_FAILURE);
    }
    newNode->treeNode = treeNode;
    newNode->next = NULL;

    if (!q->head) {
        q->head = q->tail = newNode;
    } else {
        q->tail->next = newNode;
        q->tail = newNode;
    }
}

TreeNode* dequeue(Queue* q) {
    if (!q || !q->head) return NULL;
    
    QueueNode* temp = q->head;
    TreeNode* treeNode = temp->treeNode;
    q->head = q->head->next;
    
    if (!q->head) {
        q->tail = NULL;
    }
    
    free(temp);
    return treeNode;
}

void freeQueue(Queue* q) {
if (!q) return;
    while (q->head) {
        QueueNode* temp = q->head;
        q->head = q->head->next;
        free(temp);
    }
    free(q);
}

TreeNode* createTreeNode(const char* username, double score) {
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    if (!newNode) {
        fprintf(stderr, "Error: Memory allocation failed for tree node!\n");
        exit(EXIT_FAILURE);
    }
    
    strncpy(newNode->username, username, 100);
    newNode->username[100] = '\0';
    newNode->score = score;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->height = 1;
    return newNode;
}

int getHeight(TreeNode* node) {
    return node ? node->height : 0;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

void updateHeight(TreeNode* node) {
    if (node) {
        node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    }
}

int getBalanceFactor(TreeNode* node) {
    return node ? getHeight(node->left) - getHeight(node->right) : 0;
}

TreeNode* rightRotate(TreeNode* y) {
    TreeNode* x = y->left;
    TreeNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    updateHeight(y);
    updateHeight(x);

    return x;
}

TreeNode* leftRotate(TreeNode* x) {
    TreeNode* y = x->right;
    TreeNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    updateHeight(x);
    updateHeight(y);

    return y;
}

TreeNode* insertNode(TreeNode* node, const char* username, double score) {
    if (!node) return createTreeNode(username, score);

    if (score < node->score) {
        node->left = insertNode(node->left, username, score);
    } else if (score > node->score) {
        node->right = insertNode(node->right, username, score);
    } else {
        return node;
    }

    updateHeight(node);

    int balance = getBalanceFactor(node);
    if (balance > 1 && score < node->left->score) {
        return rightRotate(node);
    }
    if (balance < -1 && score > node->right->score) {
        return leftRotate(node);
    }
    if (balance > 1 && score > node->left->score) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    if (balance < -1 && score < node->right->score) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

TreeNode* minValueNode(TreeNode* node) {
    TreeNode* current = node;
    while (current && current->left) {
        current = current->left;
    }
    return current;
}

TreeNode* deleteNode(TreeNode* root, double score) {
    if (!root) return root;

    if (score < root->score) {
        root->left = deleteNode(root->left, score);
    } else if (score > root->score) {
        root->right = deleteNode(root->right, score);
    } else {
        if (!root->left || !root->right) {
            TreeNode* temp = root->left ? root->left : root->right;

            if (!temp) {
                temp = root;
                root = NULL;
            } else {
                *root = *temp;
            }
            free(temp);
        } else {
            TreeNode* temp = minValueNode(root->right);
            root->score = temp->score;
            strncpy(root->username, temp->username, 100);
            root->right = deleteNode(root->right, temp->score);
        }
    }

    if (!root) return root;

    updateHeight(root);

    int balance = getBalanceFactor(root);
    if (balance > 1 && getBalanceFactor(root->left) >= 0) {
        return rightRotate(root);
    }
    if (balance > 1 && getBalanceFactor(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
    if (balance < -1 && getBalanceFactor(root->right) <= 0) {
        return leftRotate(root);
    }
    if (balance < -1 && getBalanceFactor(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

void preOrder(TreeNode* root) {
    if (root) {
        printf("%s ", root->username);
        preOrder(root->left);
        preOrder(root->right);
    }
}

void inOrder(TreeNode* root) {
    if (root) {
        inOrder(root->left);
        printf("%s ", root->username);
        inOrder(root->right);
    }
}

void postOrder(TreeNode* root) {
    if (root) {
        postOrder(root->left);
        postOrder(root->right);
        printf("%s ", root->username);
    }
}

void levelOrder(TreeNode* root) {
    if (!root) return;

    Queue* q = createQueue();
    enqueue(q, root);

    while (q->head) {
        TreeNode* node = dequeue(q);
        printf("%s ", node->username);

        if (node->left) enqueue(q, node->left);
        if (node->right) enqueue(q, node->right);
    }

    freeQueue(q);
}

TreeNode* searchNode(TreeNode* root, double score) {
    while (root) {
        if (score < root->score) {
            root = root->left;
        } else if (score > root->score) {
            root = root->right;
        } else {
            return root;
        }
    }
    return NULL;
}

void freeTree(TreeNode* root) {
    if (root) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}