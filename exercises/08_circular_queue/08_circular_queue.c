#include <stdio.h>
#include <stdbool.h>

#define MAX_PEOPLE 50

typedef struct {
    int id;
} People;

typedef struct {
    People data[MAX_PEOPLE];
    int head;
    int tail;
    int count;
} Queue;

void init_queue(Queue *q) {
    q->head = 0;
    q->tail = 0;
    q->count = 0;
}

bool is_empty(Queue *q) {
    return q->count == 0;
}

bool is_full(Queue *q) {
    return q->count == MAX_PEOPLE;
}

void enqueue(Queue *q, People p) {
    if (!is_full(q)) {
        q->data[q->tail] = p;
        q->tail = (q->tail + 1) % MAX_PEOPLE;
        q->count++;
    }
}

People dequeue(Queue *q) {
    if (!is_empty(q)) {
        People p = q->data[q->head];
        q->head = (q->head + 1) % MAX_PEOPLE;
        q->count--;
        return p;
    }
    People p = {-1};
    return p;
}

int main() {
    Queue q;
    int total_people = 50;
    int report_interval = 5;

    init_queue(&q);
    
    // 初始化队列，加入50个人
    for (int i = 1; i <= total_people; i++) {
        People p = {i};
        enqueue(&q, p);
    }
    
    // 约瑟夫环算法
    int count = 0;
    while (q.count > 1) {
        People p = dequeue(&q);
        count++;
        
        if (count == report_interval) {
            // 报到5的人出列
            printf("淘汰: %d\n", p.id);
            count = 0; // 重置计数器
        } else {
            // 重新入列
            enqueue(&q, p);
        }
    }
    
    printf("最后剩下的人是: %d\n", q.data[q.head].id);

    return 0;
}
