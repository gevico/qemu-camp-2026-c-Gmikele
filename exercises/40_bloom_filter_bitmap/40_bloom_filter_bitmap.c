#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * 20 位图实现 Bloom 过滤器
 * 需求：m=100 位图，k=3 个哈希函数；插入 "apple"、"banana"，查询示例
 */

typedef struct {
    unsigned char *bits;
    size_t m; /* 位图大小（比特） */
} Bloom;

static Bloom *bloom_create(size_t m) {
    Bloom *bf = malloc(sizeof(Bloom));
    if (!bf) return NULL;
    
    size_t bytes = (m + 7) / 8; // ceil(m/8)
    bf->bits = calloc(bytes, 1);
    if (!bf->bits) {
        free(bf);
        return NULL;
    }
    bf->m = m;
    return bf;
}

static void bloom_free(Bloom *bf) {
    if (!bf) return;
    free(bf->bits);
    free(bf);
}

/* 位操作 */
static void set_bit(unsigned char *bm, size_t idx) {
    size_t byte_idx = idx / 8;
    size_t bit_idx = idx % 8;
    bm[byte_idx] |= (1 << bit_idx);
}
static int test_bit(const unsigned char *bm, size_t idx) {
    size_t byte_idx = idx / 8;
    size_t bit_idx = idx % 8;
    return (bm[byte_idx] >> bit_idx) & 1;
}

/* 三个简单哈希：sum(c*k) % m */
static size_t hash_k(const char *s, size_t m, int k) {
    size_t sum = 0;
    for (const char *p = s; *p; p++) {
        sum += (unsigned char)(*p) * k;
    }
    return sum % m;
}

static void bloom_add(Bloom *bf, const char *s) {
    for (int k = 1; k <= 3; k++) {
        size_t idx = hash_k(s, bf->m, k);
        set_bit(bf->bits, idx);
    }
}

static int bloom_maybe_contains(Bloom *bf, const char *s) {
    for (int k = 1; k <= 3; k++) {
        size_t idx = hash_k(s, bf->m, k);
        if (!test_bit(bf->bits, idx)) {
            return 0;
        }
    }
    return 1;
}

int main(void) {
    const size_t m = 100; /* 位图大小 */
    Bloom *bf = bloom_create(m);
    if (!bf) {
        fprintf(stderr, "bloom create failed\n");
        return 1;
    }

    /* 插入元素："apple"、"banana" */
    bloom_add(bf, "apple");
    bloom_add(bf, "banana");

    /* 查询元素："apple"（应存在）、"orange"（可能存在误判）、"grape"（未插入） */
    int apple = bloom_maybe_contains(bf, "apple");
    int orange = bloom_maybe_contains(bf, "orange"); /* 未插入，可能误判 */
    int grape = bloom_maybe_contains(bf, "grape");   /* 未插入，可能误判 */

    printf("apple exists: %d\n", apple); /* 必须为 1（无假阴性） */
    printf("orange exists: %d\n", orange); /* 允许 0 或 1（可能误判） */
    printf("grape exists: %d\n", grape);   /* 允许 0 或 1（可能误判） */

    bloom_free(bf);
    return 0;
}
