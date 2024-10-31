#ifndef GAC_HEADER_INCLUDED
#define GAC_HEADER_INCLUDED

#include <stddef.h>

// ------------------------------------------

#ifdef GAC_PRIVATE_HEADER

#include <stdint.h>

typedef uintptr_t gacptr;

typedef struct gac_alloc_t {
    gacptr ptr;
    size_t len;
} gac_alloc_t;

/* --------------- Linked List ----------------- */

typedef struct LinkedList {
    gac_alloc_t elem;

    struct LinkedList *next;
} LL;

typedef struct {
    LL *head;
} LL_head;

size_t ll_length(const LL_head h);

void ll_push(LL_head *h, gac_alloc_t elem);

void ll_destroy(LL_head *h);

void ll_remove(LL_head *h, size_t i);

LL *ll_get(LL_head *h, size_t i);

LL *ll_find(LL_head h, gacptr p);

#endif // GAC_PRIVATE_HEADER
#endif // GAC_HEADER_INCLUDED
