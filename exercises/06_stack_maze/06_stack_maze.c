#include <stdio.h>
#include <stdbool.h>

#define MAX_ROW 5
#define MAX_COL 5
#define MAX_STACK_SIZE 100

int maze[MAX_ROW][MAX_COL] = {
	0, 1, 0, 0, 0,
	0, 1, 0, 1, 0,
	0, 0, 0, 0, 0,
	0, 1, 1, 1, 0,
	0, 0, 0, 1, 0,
};

typedef struct {
    int row;
    int col;
} Point;

typedef struct {
    Point data[MAX_STACK_SIZE];
    int top;
} Stack;

void init_stack(Stack *s) {
    s->top = -1;
}

bool is_empty(Stack *s) {
    return s->top == -1;
}

bool is_full(Stack *s) {
    return s->top == MAX_STACK_SIZE - 1;
}

void push(Stack *s, Point p) {
    if (!is_full(s)) {
        s->data[++(s->top)] = p;
    }
}

Point pop(Stack *s) {
    if (!is_empty(s)) {
        return s->data[(s->top)--];
    }
    Point p = {-1, -1};
    return p;
}

Point peek(Stack *s) {
    if (!is_empty(s)) {
        return s->data[s->top];
    }
    Point p = {-1, -1};
    return p;
}

bool is_valid(int row, int col) {
    return row >= 0 && row < MAX_ROW && col >= 0 && col < MAX_COL && maze[row][col] == 0;
}

void print_maze() {
    printf("迷宫地图:\n");
    for (int i = 0; i < MAX_ROW; i++) {
        for (int j = 0; j < MAX_COL; j++) {
            printf("%d ", maze[i][j]);
        }
        printf("\n");
    }
}

void print_path(Stack *s) {
    // 从栈顶到栈底输出（从终点到起点）
    for (int i = s->top; i >= 0; i--) {
        printf("(%d, %d)\n", s->data[i].row, s->data[i].col);
    }
}

int main(void)
{
    Stack s;
    init_stack(&s);
    
    Point start = {0, 0};
    Point end = {4, 4};
    
    // 标记起点为已访问
    maze[start.row][start.col] = 2;
    push(&s, start);
    
    // 方向数组：上、右、下、左
    int dir_row[] = {-1, 0, 1, 0};
    int dir_col[] = {0, 1, 0, -1};
    
    while (!is_empty(&s)) {
        Point current = peek(&s);
        
        // 如果到达终点
        if (current.row == end.row && current.col == end.col) {
            print_path(&s);
            return 0;
        }
        
        bool found = false;
        // 尝试所有方向
        for (int i = 0; i < 4; i++) {
            int new_row = current.row + dir_row[i];
            int new_col = current.col + dir_col[i];
            
            if (is_valid(new_row, new_col)) {
                maze[new_row][new_col] = 2; // 标记为已访问
                Point next = {new_row, new_col};
                push(&s, next);
                found = true;
                break;
            }
        }
        
        // 如果没有可走的方向，回溯
        if (!found) {
            // 将当前位置标记为死路
            maze[current.row][current.col] = 3;
            pop(&s);
        }
    }
    
    printf("没有找到路径！\n");
    print_maze();
    
	return 0;
}
