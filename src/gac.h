#ifndef GAC_HEADER_INCLUDED
#define GAC_HEADER_INCLUDED

#include <stddef.h>

#define GAC_INIT(config, err)                   \
    do {                                        \
        volatile char s;                        \
        (err) = gac_init((config), (void*) &s); \
    } while (0);                                \

// ------------ Public Structs --------------
typedef struct {
    size_t alloc_est;
    size_t interval;
} gac_cfg_t;

// -------------- GAC configuration ---------
gac_cfg_t gac_cfg_make(void);

void gac_cfg_set_interval(gac_cfg_t *cfg, size_t interval);

void gac_cfg_set_allocation_esitmate(gac_cfg_t *cfg, size_t estimate);

// ------------- Gac Initialization ---------

int gac_init(gac_cfg_t cfg, void*);

void gac_exit(void);

// -------------- Allocation ----------------

void *galloc(size_t size);

#ifdef GAC_DEBUG
// ------------- Debug Functions ------------
void gac_print_allocations(void);
#endif // GAC_DEBUG
// ------------------------------------------

#ifdef GAC_PRIVATE_HEADER

#include <stdint.h>
#include <signal.h>

// config defaults
#define CFG_ESTIMATE_DEF 1000
#define CFG_INTERVAL_DEF 1000

typedef enum _gacmark {
    GAC_MARKED = 2349872350,
} gacmark;

typedef uintptr_t gacptr;

void *gac_sweeper_main(void *p);

void mark_from_roots(gacptr start, gacptr end);

void gac_sweep(void);

void segfault_handler(int signum);

void segfault_handler_i(int sig, siginfo_t *info, void *ucontext);

void print_bits(void *e, size_t len);

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
