#ifndef GAC_HEADER_INCLUDED
#define GAC_HEADER_INCLUDED

// ------------------------------------------

#ifdef GAC_PRIVATE_HEADER

typedef gacptr uintptr_t;

typedef struct gac_alloc_t {
    gacptr ptr;
    size_t len;
} gac_alloc_t;

/* --------------- Linked List ----------------- */

typedef struct LinkedList {
    gac_alloc_t elem;

    struct LL *next;
} LL;

typedef struct {
    LL *head;
} LL_head;

size_t ll_length(const LL_head h);

void ll_push_back(LL_head *h, LL_T elem);

void ll_destroy(LL_head *h);

void ll_remove(LL_head *h, size_t i);

LL *ll_get(LL_head *h, size_t i);

#endif // GAC_PRIVATE_HEADER
#endif // GAC_HEADER_INCLUDED
