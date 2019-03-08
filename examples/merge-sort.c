#include <assert.h>
#include <stdlib.h>
#include "list.h"

#include "common.h"

static uint16_t values[256];

#define list_get_middle(head, mid)                                   \
    do {                                                             \
        struct list_head *fast, *slow;                               \
        fast = head->next;                                           \
        slow = head->next;                                           \
        while ((fast->next->next != head) && (fast->next != head)) { \
            slow = slow->next;                                       \
            fast = fast->next->next;                                 \
        }                                                            \
        mid = slow;                                                  \
    } while (0);

static void list_split(struct list_head *left,
                       struct list_head *right,
                       struct list_head *head)
{
    if (list_empty(head) || list_is_singular(head))
        return;
    struct list_head *mid;
    list_get_middle(head, mid);
    /* Cut From middle point to tail node*/
    list_cut_position(right, mid, head->prev);
    list_cut_position(left, head, mid);
}

static void list_merge(struct list_head *left,
                       struct list_head *right,
                       struct list_head *head)
{
    struct listitem *l = NULL, *r = NULL;

    while (1) {
        if (list_empty(left)) {
            list_splice_tail(right, head);
            break;
        }
        if (list_empty(right)) {
            list_splice_tail(left, head);
            break;
        }
        l = list_first_entry(left, struct listitem, list);
        r = list_first_entry(right, struct listitem, list);
        if (l->i < r->i) {
            list_move_tail(left->next, head);
        } else {
            list_move_tail(right->next, head);
        }
    }
}

static void list_mergesort(struct list_head *head)
{
    if (list_empty(head) || list_is_singular(head)) {
        return;
    }

    struct list_head left, right;
    INIT_LIST_HEAD(&left);
    INIT_LIST_HEAD(&right);
    list_split(&left, &right, head);
    list_mergesort(&left);
    list_mergesort(&right);
    list_merge(&left, &right, head);
}

int main(void)
{
    struct list_head testlist;
    struct listitem *item = NULL, *is = NULL;
    size_t i;

    random_shuffle_array(values, (uint16_t) ARRAY_SIZE(values));

    INIT_LIST_HEAD(&testlist);

    assert(list_empty(&testlist));

    for (i = 0; i < ARRAY_SIZE(values); i++) {
        item = (struct listitem *) malloc(sizeof(*item));
        assert(item);
        item->i = values[i];
        list_add_tail(&item->list, &testlist);
    }

    assert(!list_empty(&testlist));

    qsort(values, ARRAY_SIZE(values), sizeof(values[0]), cmpint);
    list_mergesort(&testlist);

    i = 0;
    list_for_each_entry_safe (item, is, &testlist, list) {
        assert(item->i == values[i]);
        list_del(&item->list);
        free(item);
        i++;
    }

    assert(i == ARRAY_SIZE(values));
    assert(list_empty(&testlist));

    return 0;
}
