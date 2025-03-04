#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define GAC_PRIVATE_HEADER
#define GAC_DEBUG
#include "../src/gac.h"

void test(uintptr_t s)
{
    volatile uintptr_t e;

    volatile void *t = galloc(40);
    printf("&t: %ld\n", (gacptr) &t);

    mark_from_roots((gacptr) s, (gacptr) &e);
    gac_print_allocations();

    gac_sweep();

}

int main(void)
{
    volatile uintptr_t sp;

    GAC_INIT(2000);

    printf("------------------\n\n");

    volatile int *x = galloc(3);

    for (int i = 0; i < 5; i++) {
        *x = 3;
        int rand_int = rand();
        x = galloc(rand_int);
    }
    x = NULL;

    test((gacptr) &sp);

    printf("\n");
    gac_print_allocations();
}
