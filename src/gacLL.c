#define GAC_PRIVATE_HEADER
#include "gac.h"

size_t ll_length(const LL_head h)
{
    size_t count = 0;

    for (LL *l = h.head; l != NULL; p = l->next)
        count++;

    return count;
}

void ll_push(LL_head *h, LL_T elem)
{
    if (h.head == NULL) return;

    for (LL *last = h.head; last->next != NULL; last = last->next);

    last->next = malloc(sizeof(LL));
    last->next->elem = elem;
}

void ll_destroy(LL_head *h)
{
    LL *next = h->head->next,
       *curr = h->head;

    while (curr != NULL) {
        free(curr);
        curr = next;

        if (next != NULL) next = next->next;
    }

    h->head = NULL;
}

void ll_remove(LL_head *h, size_t pos)
{
    if (pos == 0) {

        LL *next = h->head->next;
        free(h->head);
        h->head = next;

        return;
    }

    size_t i = 0;
    LL *prev = h->head;

    while (i + 1 != pos && prev->next != NULL) {
        i++;
        prev = prev->next;
    }

    if (prev->next != NULL) {
        LL *curr = prev->next;
        prev = curr->next;
        free(curr);
    }
}

LL *ll_get(LL_head *h, size_t pos)
{
    size_t i = 0;
    LL *curr = h->head;

    while (i != pos && curr != NULL) {
        i++;
        curr = curr->next;
    }

    return curr;
}

LL *ll_find(LL_head h, gacptr p)
{
    LL *curr = h.head;
    while (curr != NULL && curr->elem.ptr != p)
        curr = curr->next;

    return curr;

}
