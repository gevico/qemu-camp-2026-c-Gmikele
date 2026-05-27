#include "doubly_circular_queue.h"

#include <stdlib.h>

// 头尾哨兵
struct node tailsentinel;
struct node headsentinel = {0, NULL, &tailsentinel};
struct node tailsentinel = {0, &headsentinel, NULL};

link head = &headsentinel;
link tail = &tailsentinel;

link make_node(int data) {
    link p = malloc(sizeof(*p));
    if (p) {
        p->data = data;
        p->prev = NULL;
        p->next = NULL;
    }
    return p;
}

void free_node(link p) {
    free(p);
}

link search(int key) {
    for (link p = head->next; p != tail; p = p->next) {
        if (p->data == key) {
            return p;
        }
    }
    return NULL;
}

void insert(link p) {
    // 尾插：在tail之前插入（实现FIFO队列的入队）
    p->prev = tail->prev;
    p->next = tail;
    tail->prev->next = p;
    tail->prev = p;
}

void delete(link p) {
    if (p == NULL || p == head || p == tail) {
        return;
    }
    p->prev->next = p->next;
    p->next->prev = p->prev;
}

void traverse(void (*visit)(link)) {
    for (link p = head->next; p != tail; p = p->next) {
        visit(p);
    }
}

void destroy(void) {
    link p = head->next;
    while (p != tail) {
        link next = p->next;
        free(p);
        p = next;
    }
    head->next = tail;
    tail->prev = head;
}

int is_empty(void) {
    return head->next == tail;
}

void enqueue(int data) {
    link new_node = make_node(data);
    if (new_node) {
        insert(new_node);
    }
}

int dequeue(void) {
    if (is_empty()) {
        return -1;  // 队列为空
    }
    // 从队首取出元素（head之后第一个节点）
    link p = head->next;
    int data = p->data;
    delete(p);
    free(p);
    return data;
}