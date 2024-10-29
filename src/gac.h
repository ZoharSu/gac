#ifndef GAC_HEADER_INCLUDED
#define GAC_HEADER_INCLUDED

// ------------------------------------------

#ifdef GAC_PRIVATE_HEADER

typedef gacptr uintptr_t;

typedef struct gac_alloc_t {
    gacptr ptr;
    size_t len;
} gac_alloc_t;

#endif // GAC_PRIVATE_HEADER
#endif // GAC_HEADER_INCLUDED
