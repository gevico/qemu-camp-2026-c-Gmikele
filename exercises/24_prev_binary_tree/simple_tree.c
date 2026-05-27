#include "simple_tree.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

Queue* create_queue() {
    Queue *q = (Queue*)malloc(sizeof(Queue));
    q->front = q->rear = NULL;
    return q;
}

void enqueue(Queue *q, TreeNode *tree_node) {
    QueueNode *new_node = (QueueNode*)malloc(sizeof(QueueNode));
    new_node->tree_node = tree_node;
    new_node->next = NULL;
    
    if (q->rear == NULL) {
        q->front = q->rear = new_node;
    } else {
        q->rear->next = new_node;
        q->rear = new_node;
    }
}

TreeNode* dequeue(Queue *q) {
    if (is_empty(q)) {
        return NULL;
    }
    
    QueueNode *temp = q->front;
    TreeNode *tree_node = temp->tree_node;
    q->front = q->front->next;
    
    if (q->front == NULL) {
        q->rear = NULL;
    }
    
    free(temp);
    return tree_node;
}

bool is_empty(Queue *q) {
    return q->front == NULL;
}

void free_queue(Queue *q) {
    while (!is_empty(q)) {
        dequeue(q);
    }
    free(q);
}

TreeNode* build_tree_by_level(int *level_order, int size) {
    if (size == 0 || level_order[0] == INT_MIN) {
        return NULL;
    }
    
    Queue *q = create_queue();
    TreeNode *root = (TreeNode*)malloc(sizeof(TreeNode));
    root->val = level_order[0];
    root->left = root->right = NULL;
    enqueue(q, root);
    
    int i = 1;
    while (i < size && !is_empty(q)) {
        TreeNode *current = dequeue(q);
        
        // 处理左子节点
        if (i < size && level_order[i] != INT_MIN) {
            TreeNode *left_node = (TreeNode*)malloc(sizeof(TreeNode));
            left_node->val = level_order[i];
            left_node->left = left_node->right = NULL;
            current->left = left_node;
            enqueue(q, left_node);
        }
        i++;
        
        // 处理右子节点
        if (i < size && level_order[i] != INT_MIN) {
            TreeNode *right_node = (TreeNode*)malloc(sizeof(TreeNode));
            right_node->val = level_order[i];
            right_node->left = right_node->right = NULL;
            current->right = right_node;
            enqueue(q, right_node);
        }
        i++;
    }
    
    free_queue(q);
    return root;
}

void preorder_traversal(TreeNode *root) {
    if (root == NULL) {
        return;
    }
    printf("%d ", root->val);
    preorder_traversal(root->left);
    preorder_traversal(root->right);
}

void preorder_traversal_iterative(TreeNode *root) {
    if (root == NULL) {
        return;
    }
    
    // 使用栈实现迭代前序遍历
    TreeNode *stack[100];
    int top = -1;
    
    stack[++top] = root;
    
    while (top >= 0) {
        TreeNode *current = stack[top--];
        printf("%d ", current->val);
        
        // 先右后左，这样出栈时就是先左后右
        if (current->right != NULL) {
            stack[++top] = current->right;
        }
        if (current->left != NULL) {
            stack[++top] = current->left;
        }
    }
}

void free_tree(TreeNode *root) {
    if (root == NULL) {
        return;
    }
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}
