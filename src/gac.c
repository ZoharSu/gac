#define GAC_PRIVATE_HEADER
#include "gac.h"
#include "../libs/headers/bloom_filter.h"
#include "../libs/headers/murmur3.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <signal.h>
#include <stdbool.h>
#include <assert.h>
#include <setjmp.h>

#ifdef GAC_DEBUG
#include <stdarg.h>
void dbg_print(const char *format, ...)
{
    va_list args;
    va_start(args, format);

    vprintf(format, args);

    va_end(args);
}
#else
void dbg_print(const char *, ...) {}
#endif

BloomFilter *bf;
LL_head allocs = { NULL };

gacptr stack_start;

jmp_buf segfault_env;

struct sigaction segfault_oldsig;

struct sigaction segfault_action = {
    .sa_handler = segfault_handler,
    .sa_flags = SA_RESTART | SA_SIGINFO,
};

gac_cfg_t config;

static void config_init_defaults(gac_cfg_t *cfg)
{
    if (cfg->alloc_est == 0)
        cfg->alloc_est = CFG_ESTIMATE_DEF;

    if (cfg->interval == 0)
        cfg->interval = CFG_INTERVAL_DEF;
}

static void mark(void *p)
{
    *(gacmark*)(p - 1) = GAC_MARKED;
}

static void unmark(void *p)
{
    *(gacmark*)(p - 1) = GAC_MARKED - 1;
}

static bool is_marked(void *p)
{
    return *(gacmark*)(p - 1) == GAC_MARKED;
}

size_t murmur3(size_t i, const void *e, size_t len)
{
    size_t out;
    MurmurHash3_x86_32(e, len, i, &out);

    return out;
}

void segfault_handler(int signum)
{
    assert(signum == SIGSEGV
           && "segfault handler not from segfault");

    siglongjmp(segfault_env, true);
}

static void set_segfault_guard(void)
{
    sigaction(SIGSEGV, &segfault_action, &segfault_oldsig);
}

static void rem_segfault_guard(void)
{
    sigaction(SIGSEGV, &segfault_oldsig, NULL);
}

int gac_init(gac_cfg_t cfg, void *s)
{
    stack_start = (gacptr) s;

    config = cfg;
    config_init_defaults(&config);

    if (sigemptyset(&segfault_action.sa_mask) != 0)
        return -1;

    bf = bf_new(0.1, config.alloc_est, murmur3);

    return 0;
}

void marking_phase(void)
{
    volatile uint64_t loc = 0;
    mark_from_roots(stack_start, (gacptr) &loc);
}

void mark_from_roots(gacptr start, gacptr /* non-inclusive */ end)
{
    assert(start > end); // stack grows downwards

    set_segfault_guard();
    for (volatile gacptr i = start; i > end; i -= 1) {

        if (sigsetjmp(segfault_env, true))
            continue; // means program segfaulted

        void *ptr = *(void **)i;
        bool contained = bf_contains(bf, &ptr, sizeof(void *));

        bool marked = contained ? is_marked(ptr) : false;
        // Possible segfault <-   ~~~~~~~~~~~~~~

        if (contained && !marked) {
            rem_segfault_guard();

            // TODO: add config option to disable printing
            dbg_print("marking %p\n", ptr);

            LL *node = ll_find(allocs, (gacptr) ptr);

            if (node == NULL)
                continue; // false positive

            gac_alloc_t alloc = node->elem;

            mark(ptr);

            mark_from_roots(i + alloc.len, i);

            set_segfault_guard();
        }
    }
    rem_segfault_guard();
}

void *galloc(size_t size)
{
    void *ptr = malloc(size);
    assert(ptr != NULL && "malloc == 0");

    gac_alloc_t alloc;
    alloc.len = size;
    alloc.ptr = (gacptr) ptr;

    bf_add(bf, (void *) &ptr, sizeof(void *));

    ll_push(&allocs, alloc);

    return ptr;
}

static void gfree(LL *target)
{
    dbg_print("freeing %zu\n", target->elem.ptr);
    free((void *) target->elem.ptr);
    free(target);
}

void gac_print_allocations(void)
{
    for (LL *l = allocs.head; l != NULL; l = l->next) {
        dbg_print("%c: %zu : %zu\n",
                is_marked((void*) l->elem.ptr) ? 'Y' : 'N',
                l->elem.ptr,
                l->elem.len);
    }
}

void print_bits(void *e, size_t len)
{
    for (size_t i = 0; i < bf->k; i++) {
        size_t hashed = bf->hash(i, e, len) % bf->n;
        char on[3] = "on";
        char off[4] = "off";
        char *state = bf_bm_test(&bf->arr, hashed) ? on : off;

        dbg_print("%zu. %zu %s\n", i + 1, hashed, state);

    }

}

void gac_sweep(void)
{
    LL *prev = NULL, *curr = allocs.head;

    dbg_print("\n");
    while (curr != NULL) {
        if (!is_marked((void*) curr->elem.ptr)) {
            LL *next = curr->next;
            gfree(curr);
            if (prev == NULL) {
                allocs.head = next;
                curr = next;
            } else {
                prev->next = next;
                curr = next;
            }
        } else {
            unmark((void *) curr->elem.ptr);

            prev = (prev == NULL) ? allocs.head : prev->next;
            curr = curr->next;
        }
    }
}

