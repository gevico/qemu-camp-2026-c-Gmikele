#include <stdio.h>
#include <stdlib.h>
#include "singly_linked_list.h"

// 打印节点的值
void print_item(link p) { printf("%d ", p->item); }

// 通过遍历拿到当前链表的头结点
static link g_first_node = NULL;
static void capture_first(link p) {
    if (g_first_node == NULL) {
        g_first_node = p;
    }
}

static link get_head_node(void) {
    g_first_node = NULL;
    traverse(capture_first);
    return g_first_node;
}

// 获取下一个节点；若到达尾部则回绕到头结点
static inline link next_wrap(link p) {
    if (p == NULL) return get_head_node();
    return p->next ? p->next : get_head_node();
}

// 创建单链表（使用push，所以顺序是n, n-1, ..., 1）
void create_list(int n) {
    // 参数校验
    if (n <= 0) return;

    destroy();
    for (int i = n; i >= 1; i--) {
        link new_node = make_node(i);
        push(new_node);
    }
}

// 用单链表模拟约瑟夫环问题（使用next_wrap模拟环形）
// n: 总人数
// k: 起始位置（从 1 开始计数）
// m: 报数阈值（数到 m 的人出列）
void josephus_problem(int n, int k, int m) {
    // 参数校验
    if (n <= 0 || k <= 0 || m <= 0) {
        printf("参数错误：n, k, m 都必须大于 0\n");
        return;
    }

    // 创建链表
    create_list(n);
    
    // 获取头节点
    link current = get_head_node();
    if (!current) {
        printf("\n");
        return;
    }

    // 找到第k个位置作为起点（k从1开始）
    // 注意：由于链表是逆序的（n, n-1, ..., 1），我们需要调整
    // 实际上，对于约瑟夫环来说，节点的具体值不重要，重要的是相对位置
    // 我们只需要正确找到第k个位置即可
    for (int i = 1; i < k; ++i) {
        current = next_wrap(current);
    }

    // 打印标题
    printf("约瑟夫环(n=%d, k=%d, m=%d): ", n, k, m);
    
    // 开始约瑟夫环算法
    while (n > 0) {
        if (m == 1) {
            // 特殊情况：m=1，直接删除当前节点
            print_item(current);
            link to_delete = current;
            current = next_wrap(current); // 移动到下一个节点
            delete(to_delete); // 使用库函数删除节点
        } else {
            // 一般情况：找到要删除节点的前一个节点
            // 我们需要走m-1步，停在要删除节点的前一个位置
            link prev = current;
            for (int i = 1; i < m - 1; ++i) {
                prev = next_wrap(prev);
            }
            
            // 要删除的节点是prev的下一个节点
            link to_delete = next_wrap(prev);
            
            // 打印要出列的人
            print_item(to_delete);
            
            // 删除节点
            delete(to_delete); // 使用库函数删除节点
            
            // current指向被删除节点的下一个节点
            current = next_wrap(prev);
        }
        
        n--;
        
        // 如果链表为空，退出循环
        if (get_head_node() == NULL) {
            break;
        }
    }

    printf("\n");
}

int main(void) {
    printf("测试约瑟夫环问题:\n");
    
    printf("测试1: ");
    josephus_problem(5, 1, 2);  // 预期输出：2 4 1 5 3
    
    printf("测试2: ");
    josephus_problem(7, 3, 1);  // 预期输出：3 4 5 6 7 1 2
    
    printf("测试3: ");
    josephus_problem(9, 1, 8);  // 预期输出：8 7 9 2 5 4 1 6 3
    
    return 0;
}
