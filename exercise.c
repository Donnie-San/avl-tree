#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct TreeNode {
    char name[101];
    int months;
    int join_year;
    int join_month;
    struct TreeNode* left;
    struct TreeNode* right;
    int height;
} TreeNode;

int max(int a, int b) { return (a > b) ? a : b; }

int calculate_months(int year, int month) { return (year - 2023) * 12 + (month - 1); }

int height(TreeNode* node) { return node ? node->height : -1; }

void update_height(TreeNode* node) {
    if (node) node->height = 1 + max(height(node->left), height(node->right));
}

int balance_factor(TreeNode* node) {
    return node ? height(node->left) - height(node->right) : 0;
}

TreeNode* rotate_right(TreeNode* y) {
    TreeNode* x = y->left;
    TreeNode* T2 = x->right;
    
    x->right = y;
    y->left = T2;
    
    update_height(y);
    update_height(x);
    
    return x;
}

TreeNode* rotate_left(TreeNode* x) {
    TreeNode* y = x->right;
    TreeNode* T2 = y->left;
    
    y->left = x;
    x->right = T2;
    
    update_height(x);
    update_height(y);
    
    return y;
}

TreeNode* create_node(const char* name, int months, int year, int month) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    if (!node) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    
    strncpy(node->name, name, sizeof(node->name) - 1);
    node->name[sizeof(node->name) - 1] = '\0';
    node->months = months;
    node->join_year = year;
    node->join_month = month;
    node->left = node->right = NULL;
    node->height = 0;
    return node;
}

TreeNode* insert(TreeNode* node, const char* name, int months, int year, int month) {
    if (!node) return create_node(name, months, year, month);
    
    if (months < node->months)
        node->left = insert(node->left, name, months, year, month);
    else if (months > node->months)
        node->right = insert(node->right, name, months, year, month);
    else
        return node;
    
    update_height(node);
    
    int balance = balance_factor(node);
    
    if (balance > 1 && months < node->left->months)
        return rotate_right(node);
    
    if (balance < -1 && months > node->right->months)
        return rotate_left(node);
    
    if (balance > 1 && months > node->left->months) {
        node->left = rotate_left(node->left);
        return rotate_right(node);
    }
    
    if (balance < -1 && months < node->right->months) {
        node->right = rotate_right(node->right);
        return rotate_left(node);
    }
    
    return node;
}

TreeNode* min_value_node(TreeNode* node) {
    TreeNode* current = node;
    while (current && current->left)
        current = current->left;
    return current;
}

TreeNode* delete_node(TreeNode* root, int months) {
    if (!root) return root;
    
    if (months < root->months)
        root->left = delete_node(root->left, months);
    else if (months > root->months)
        root->right = delete_node(root->right, months);
    else {
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
            TreeNode* temp = min_value_node(root->right);
            root->months = temp->months;
            strcpy(root->name, temp->name);
            root->join_year = temp->join_year;
            root->join_month = temp->join_month;
            root->right = delete_node(root->right, temp->months);
        }
    }
    
    if (!root) return root;
    
    update_height(root);
    
    int balance = balance_factor(root);
    
    if (balance > 1 && balance_factor(root->left) >= 0)
        return rotate_right(root);
    
    if (balance > 1 && balance_factor(root->left) < 0) {
        root->left = rotate_left(root->left);
        return rotate_right(root);
    }
    
    if (balance < -1 && balance_factor(root->right) <= 0)
        return rotate_left(root);
    
    if (balance < -1 && balance_factor(root->right) > 0) {
        root->right = rotate_right(root->right);
        return rotate_left(root);
    }
    
    return root;
}

TreeNode* find_node(TreeNode* root, const char* name) {
    if (!root) return NULL;
    if (strcmp(root->name, name) == 0) return root;
    
    TreeNode* left = find_node(root->left, name);
    if (left) return left;
    
    return find_node(root->right, name);
}

int count_nodes(TreeNode* root) {
    return root ? 1 + count_nodes(root->left) + count_nodes(root->right) : 0;
}

void print_region(TreeNode* root, bool is_vice) {
    if (!root) return;
    
    if (is_vice) {
        printf("Vice Chairman : %s %d month\n", root->name, root->months);
    } else {
        printf("- Member : %s %d month\n", root->name, root->months);
    }
    
    print_region(root->left, false);
    print_region(root->right, false);
}

TreeNode* find_candidate(TreeNode* root, TreeNode* current, bool is_west) {
    if (!root) return NULL;
    
    TreeNode* candidate = NULL;
    
    if (is_west) {
        if (root->months > current->months) {
            candidate = find_candidate(root->left, current, is_west);
            return candidate ? candidate : root;
        }
        return find_candidate(root->right, current, is_west);
    } else {
        if (root->months < current->months) {
            candidate = find_candidate(root->right, current, is_west);
            return candidate ? candidate : root;
        }
        return find_candidate(root->left, current, is_west);
    }
}

void free_tree(TreeNode* root) {
    if (root) {
        free_tree(root->left);
        free_tree(root->right);
        free(root);
    }
}

int main() {
    TreeNode* root = NULL;
    int member_count;
    
    scanf("%d", &member_count); getchar();
    
    for (int i = 0; i < member_count; i++) {
        char name[101];
        int year, month;
        
        scanf("%100s %d-%d", name, &year, &month); getchar();
        
        int months = calculate_months(year, month);
        root = insert(root, name, months, year, month);
    }
    
    printf("Total Member : %d\n", count_nodes(root));
    printf("Chairman : %s %d month\n", root->name, root->months);
    
    printf("West (%d member):\n", count_nodes(root->left));
    print_region(root->left, true);
    
    printf("East (%d member):\n", count_nodes(root->right));
    print_region(root->right, true);
    
    char command[10], name[101];
    scanf("%s %s", command, name); getchar();
    
    TreeNode* to_delete = find_node(root, name);
    if (to_delete) {
        root = delete_node(root, to_delete->months);
    }
    
    printf("\nTotal Member : %d\n", count_nodes(root));
    printf("Chairman : %s %d month\n", root->name, root->months);
    
    printf("West (%d member):\n", count_nodes(root->left));
    print_region(root->left, true);
    
    printf("East (%d member):\n", count_nodes(root->right));
    print_region(root->right, true);
    
    char region[10];
    scanf("%s %s", command, region);
    
    bool is_west = strcmp(region, "WEST") == 0;
    TreeNode* vice = is_west ? root->left : root->right;
    TreeNode* candidate = find_candidate(root, vice, is_west);
    
    printf("\nVice Chairman %s Candidate : %s %d month\n", 
           region, candidate->name, candidate->months);
    
    free_tree(root);
    return 0;
}