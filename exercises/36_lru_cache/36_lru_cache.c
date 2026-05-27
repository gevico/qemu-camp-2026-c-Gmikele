#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * 16 LRU 缓存淘汰算法（哈希表 + 双向链表）
 *  - put(k,v)、get(k) 均为 O(1)
 *  - 容量满时淘汰最久未使用（LRU）的元素
 */

typedef struct LRUNode {
    int key;
    int value;
    struct LRUNode* prev;
    struct LRUNode* next;
} LRUNode;

typedef struct HashEntry {
    int key;
    LRUNode* node;
    struct HashEntry* next;
} HashEntry;

typedef struct {
    int capacity;
    int size;
    LRUNode* head; /* 最近使用（MRU） */
    LRUNode* tail; /* 最久未使用（LRU） */
    /* 简单链式哈希表 */
    size_t bucket_count;
    HashEntry** buckets;
} LRUCache;

static unsigned hash_int(LRUCache* c, int key) {
    // 简单的哈希函数：使用乘法和取模
    return (unsigned)(key * 2654435761u) & (c->bucket_count - 1);
}

static HashEntry* hash_find(LRUCache* c, int key, HashEntry*** pprev_next) {
    unsigned idx = hash_int(c, key);
    HashEntry* cur = c->buckets[idx];
    *pprev_next = &c->buckets[idx];
    while (cur) {
        if (cur->key == key) {
            return cur;
        }
        *pprev_next = &cur->next;
        cur = cur->next;
    }
    return NULL;
}

static void list_add_to_head(LRUCache* c, LRUNode* node) {
    node->prev = NULL;
    node->next = c->head;
    if (c->head) {
        c->head->prev = node;
    } else {
        // 空链表，尾部也是该节点
        c->tail = node;
    }
    c->head = node;
}

static void list_remove(LRUCache* c, LRUNode* node) {
    if (node->prev) {
        node->prev->next = node->next;
    } else {
        // node is head
        c->head = node->next;
    }
    if (node->next) {
        node->next->prev = node->prev;
    } else {
        // node is tail
        c->tail = node->prev;
    }
    node->prev = node->next = NULL;
}

static void list_move_to_head(LRUCache* c, LRUNode* node) {
    list_remove(c, node);
    list_add_to_head(c, node);
}

static LRUNode* list_pop_tail(LRUCache* c) {
    if (!c->tail) return NULL;
    LRUNode* tail = c->tail;
    list_remove(c, tail);
    return tail;
}

/* LRU 接口实现 */
static LRUCache* lru_create(int capacity) {
    LRUCache* c = malloc(sizeof(LRUCache));
    if (!c) return NULL;
    c->capacity = capacity;
    c->size = 0;
    c->head = c->tail = NULL;
    c->bucket_count = 16; // 2的幂
    c->buckets = calloc(c->bucket_count, sizeof(HashEntry*));
    if (!c->buckets) {
        free(c);
        return NULL;
    }
    return c;
}

static void lru_free(LRUCache* c) {
    // 释放所有哈希条目
    for (size_t i = 0; i < c->bucket_count; i++) {
        HashEntry* entry = c->buckets[i];
        while (entry) {
            HashEntry* next = entry->next;
            free(entry);
            entry = next;
        }
    }
    free(c->buckets);
    // 释放链表节点
    LRUNode* node = c->head;
    while (node) {
        LRUNode* next = node->next;
        free(node);
        node = next;
    }
    free(c);
}

static int lru_get(LRUCache* c, int key, int* out_value) {
    HashEntry** pprev_next;
    HashEntry* entry = hash_find(c, key, &pprev_next);
    if (!entry) {
        return 0; // not found
    }
    // move node to head
    list_move_to_head(c, entry->node);
    *out_value = entry->node->value;
    return 1;
}

static void lru_put(LRUCache* c, int key, int value) {
    HashEntry** pprev_next;
    HashEntry* entry = hash_find(c, key, &pprev_next);
    if (entry) {
        // key already exists, update value and move to head
        entry->node->value = value;
        list_move_to_head(c, entry->node);
        return;
    }
    
    // key not exist, create new node
    if (c->size >= c->capacity) {
        // evict LRU node from tail
        LRUNode* tail = list_pop_tail(c);
        if (tail) {
            // remove from hash table
            HashEntry** pprev_next_evict;
            HashEntry* evict_entry = hash_find(c, tail->key, &pprev_next_evict);
            if (evict_entry) {
                *pprev_next_evict = evict_entry->next;
                free(evict_entry);
            }
            free(tail);
            c->size--;
        }
    }
    
    // create new node
    LRUNode* node = malloc(sizeof(LRUNode));
    if (!node) return;
    node->key = key;
    node->value = value;
    node->prev = node->next = NULL;
    
    // create hash entry
    HashEntry* new_entry = malloc(sizeof(HashEntry));
    if (!new_entry) {
        free(node);
        return;
    }
    new_entry->key = key;
    new_entry->node = node;
    new_entry->next = NULL;
    
    // insert into hash table
    unsigned idx = hash_int(c, key);
    new_entry->next = c->buckets[idx];
    c->buckets[idx] = new_entry;
    
    // insert into list at head
    list_add_to_head(c, node);
    c->size++;
}

/* 打印当前缓存内容（从头到尾） */
static void lru_print(LRUCache* c) {
    LRUNode* p = c->head;
    int first = 1;
    while (p) {
        if (!first) printf(", ");
        first = 0;
        printf("%d:%d", p->key, p->value);
        p = p->next;
    }
    printf("\n");
}

int main(void) {
    /* 容量 3：put(1,1), put(2,2), put(3,3), put(4,4), get(2), put(5,5) */
    LRUCache* c = lru_create(3);
    if (!c) {
        fprintf(stderr, "创建 LRU 失败\n");
        return 1;
    }

    lru_put(c, 1, 1); /* 缓存：1 */
    lru_put(c, 2, 2); /* 缓存：2,1 */
    lru_put(c, 3, 3); /* 缓存：3,2,1 (满) */
    lru_put(c, 4, 4); /* 淘汰 LRU(1)，缓存：4,3,2 */

    int val;
    if (lru_get(c, 2, &val)) {
        /* 访问 2：缓存：2,4,3 */
        (void)val; /* 演示无需使用 */
    }

    lru_put(c, 5, 5); /* 淘汰 LRU(3)，缓存：5,2,4 */

    /* 期望最终键集合：{2,4,5}，顺序无关。此处按最近->最久打印：5:5, 2:2, 4:4 */
    lru_print(c);

    lru_free(c);
    return 0;
}
