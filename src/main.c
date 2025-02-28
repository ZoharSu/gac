#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "gac.h"

void test(uintptr_t s)
{
    volatile uintptr_t e;

    volatile void *t = galloc(40);
    printf("&t: %ld\n", (gacptr) &t);

    mark_from_roots((gacptr) s, (gacptr) &e);
    print_allocs();

    gac_sweep();

}

int main(void)
{
    volatile uintptr_t sp;

    GAC_INIT(2000);

    volatile uintptr_t k = 3;

    volatile char *h = galloc(13);
    h[12] = '\0';

    for (int i = 0; i < 3; i++) {
        volatile int *x = galloc(40);
        *x = 234;
        k += (uintptr_t) x;
    }
    // volatile int loc = 3;
    fprintf(stderr, "%zu\n", k);
    printf("h is %s\n", h);
    // mark_from_roots((gacptr) &sp, (gacptr) &loc);
//    print_allocs();
//
//    gac_sweep();
    test((gacptr) &sp);
}
