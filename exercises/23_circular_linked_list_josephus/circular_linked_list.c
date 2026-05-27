#include "circular_linked_list.h"

#include <stdio.h>
#include <stdlib.h>

Node* create_circular_list(int n) {
    if (n <= 0) {
        return NULL;
    }
    
    Node* head = NULL;
    Node* tail = NULL;
    
    for (int i = 1; i <= n; i++) {
        Node* new_node = malloc(sizeof(Node));
        if (!new_node) {
            // 内存分配失败，释放已分配的内存
            while (head) {
                Node* temp = head;
                head = head->next;
                free(temp);
            }
            return NULL;
        }
        
        new_node->id = i;
        new_node->next = NULL;
        
        if (!head) {
            head = new_node;
            tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }
    }
    
    // 将尾节点指向头节点，形成环形链表
    if (tail) {
        tail->next = head;
    }
    
    return head;
}

void free_list(Node* head) {
    if (!head) {
        return;
    }
    
    Node* current = head;
    Node* next;
    
    // 先断开环形连接
    Node* temp = head;
    while (temp->next != head) {
        temp = temp->next;
    }
    temp->next = NULL;
    
    // 释放所有节点
    while (current) {
        next = current->next;
        free(current);
        current = next;
    }
}
