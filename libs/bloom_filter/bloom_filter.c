#include "bloom_filter.h"
#include <assert.h>
#include <stdlib.h>
#include <math.h>

struct BF_BitMap bf_bm_new(size_t bit_num)
{
    size_t size = bit_num / (8 * sizeof(uint64_t));
    uint64_t *alloc = calloc(sizeof(uint64_t), size);

    assert(alloc != NULL);

    return (struct BF_BitMap) {
        alloc,
        size,
    };
}

void bf_bm_set(struct BF_BitMap *bm, size_t bit_index)
{
    size_t ai = bit_index / 64,
           bi = bit_index % 64;

    assert(ai < bm->size);

    bm->arr[ai] |= (uint64_t)1 << bi;
}

bool bf_bm_test(const struct BF_BitMap *bm, size_t bit_index)
{
    size_t ai = bit_index / 64,
           bi = bit_index % 64;

    assert(ai < bm->size);

    return (bm->arr[ai] >> bi) & 1;
}

void bf_bm_clear_all(struct BF_BitMap *bm)
{
    memset(bm->arr, 0, sizeof(*bm->arr) * bm->size);
}

BloomFilter *bf_new(float P, size_t n, BF_nth_hash hash)
{
    BloomFilter *bf = malloc(sizeof(BloomFilter));

    size_t m = ceil(-(n * logf(P)) / (logf(2)*logf(2)));

    size_t k = ceil(m / n * log(2));

    *bf = (BloomFilter) {
        bf_bm_new(m),
        hash,
        .n = n,
        .k = k,
        .m = m,
        .count = 0,
    };

    return bf;
}

bool bf_contains(const BloomFilter *bf, void *e, size_t len)
{
    for (int i = 0; i < bf->k; i++) {
        size_t hashed = bf->hash(i, e, len) % bf->n;

        if (!bf_bm_test(&bf->arr, hashed))
            return false;
    }

    return true;

}

void bf_add(BloomFilter *bf, void *e, size_t len)
{
    for (int i = 0; i < bf->k; i++) {
        size_t hashed = bf->hash(i, e, len) % bf->n;

        bf_bm_set(&bf->arr, hashed);
    }

    bf->count++;
}

void bf_destroy(BloomFilter *bf)
{
    free(bf->arr.arr);

}
