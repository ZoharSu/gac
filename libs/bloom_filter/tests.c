#include "bloom_filter.h"
#include "./murmur3/murmur3.h"
#include <stdlib.h>
#include <stdio.h>

size_t murmur(size_t n, void *e, size_t len)
{
    size_t out;

    MurmurHash3_x86_32(e, len, n, &out);

    return out;
}

int main(void)
{
    BloomFilter *bf = bf_new(0.1, 10000, murmur);


    int x = random();
    bf_add(bf, (void*) &x, sizeof(int));

    for (int i = 0; i < 40; i++) {
        int t = random();
        bf_add(bf, (void*) &t, sizeof(int));
        printf("added: %d\n", t);
    }

    for (int i = 0; i < 1000000000; i++) {
        int t = random();
        if (bf_contains(bf, (void *) &t, sizeof(int)))
            printf("bf contains %d\n", t);
    }

    printf("bf contains x ? %d\n", bf_contains(bf, (void *) &x, sizeof(int)));
}
