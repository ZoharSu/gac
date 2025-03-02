#ifndef bloom_filter_h_INCLUDED
#define bloom_filter_h_INCLUDED

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

struct BF_BitMap {
    uint64_t *arr;
    size_t size;
};

struct BF_BitMap bf_bm_new(size_t bit_num);

void bf_bm_set(struct BF_BitMap *bm, size_t bit_index);

bool bf_bm_test(const struct BF_BitMap *bm, size_t bit_i);

void bf_bm_clear_all(struct BF_BitMap *bm);

typedef size_t (*BF_nth_hash)(size_t n, void *e, size_t len);

typedef struct {
    struct BF_BitMap arr;
    BF_nth_hash hash;
    float P;
    size_t n, k, m;
    size_t count;
} BloomFilter;

BloomFilter *bf_new(float P, size_t n, BF_nth_hash hash);

bool bf_contains(const BloomFilter *bf, void *e, size_t len);

void bf_add(BloomFilter *bf, void *e, size_t len);

void bf_reset(BloomFilter *bf);

void bf_destroy(BloomFilter *bf);

#endif // bloom_filter_h_INCLUDED
