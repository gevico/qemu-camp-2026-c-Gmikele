#include <stdio.h>
#include <stdbool.h>

#define MAX_ROW 5
#define MAX_COL 5
#define MAX_QUEUE_SIZE 100

int maze[MAX_ROW][MAX_COL] = {
    {0, 1, 0, 0, 0},
    {0, 1, 0, 1, 0},
    {0, 0, 0, 0, 0},
    {0, 1, 1, 1, 0},
    {0, 0, 0, 1, 0},
};

typedef struct {
    int row;
    int col;
} Point;

typedef struct {
    Point data[MAX_QUEUE_SIZE];
    int front;
    int rear;
} Queue;

// 记录每个节点的父节点
Point parent[MAX_ROW][MAX_COL];

void init_queue(Queue *q) {
    q->front = 0;
    q->rear = 0;
}

bool is_empty(Queue *q) {
    return q->front == q->rear;
}

bool is_full(Queue *q) {
    return (q->rear + 1) % MAX_QUEUE_SIZE == q->front;
}

void enqueue(Queue *q, Point p) {
    if (!is_full(q)) {
        q->data[q->rear] = p;
        q->rear = (q->rear + 1) % MAX_QUEUE_SIZE;
    }
}

Point dequeue(Queue *q) {
    if (!is_empty(q)) {
        Point p = q->data[q->front];
        q->front = (q->front + 1) % MAX_QUEUE_SIZE;
        return p;
    }
    Point p = {-1, -1};
    return p;
}

bool is_valid(int row, int col) {
    return row >= 0 && row < MAX_ROW && col >= 0 && col < MAX_COL && maze[row][col] == 0;
}

void print_path(Point end) {
    // 回溯路径
    Point path[MAX_QUEUE_SIZE];
    int path_len = 0;
    
    // 从终点开始回溯
    Point current = end;
    path[path_len++] = current;
    
    // 回溯直到起点（起点的父节点是(-1, -1)）
    while (parent[current.row][current.col].row != -1) {
        current = parent[current.row][current.col];
        path[path_len++] = current;
    }
    
    // 输出路径（从终点到起点）
    for (int i = 0; i < path_len; i++) {
        printf("(%d,%d)\n", path[i].row, path[i].col);
    }
}

int main(void)
{
    Queue q;
    init_queue(&q);
    
    Point start = {0, 0};
    Point end = {4, 4};
    
    // 初始化父节点数组
    for (int i = 0; i < MAX_ROW; i++) {
        for (int j = 0; j < MAX_COL; j++) {
            parent[i][j].row = -1;
            parent[i][j].col = -1;
        }
    }
    
    // 标记起点为已访问
    maze[start.row][start.col] = 2;
    enqueue(&q, start);
    
    // 方向数组：上、右、下、左
    int dir_row[] = {-1, 0, 1, 0};
    int dir_col[] = {0, 1, 0, -1};
    
    while (!is_empty(&q)) {
        Point current = dequeue(&q);
        
        // 如果到达终点
        if (current.row == end.row && current.col == end.col) {
            print_path(end);
            return 0;
        }
        
        // 探索所有方向
        for (int i = 0; i < 4; i++) {
            int new_row = current.row + dir_row[i];
            int new_col = current.col + dir_col[i];
            
            if (is_valid(new_row, new_col)) {
                maze[new_row][new_col] = 2; // 标记为已访问
                parent[new_row][new_col] = current; // 记录父节点
                Point next = {new_row, new_col};
                enqueue(&q, next);
            }
        }
    }
    
    printf("没有找到路径！\n");
    
	return 0;
}