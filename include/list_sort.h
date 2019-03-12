#ifndef LIST_SORT_H_7YOFEXAH
#define LIST_SORT_H_7YOFEXAH

#include "common.h"
#include "list.h"

enum { QUICK_SORT, MERGE_SORT, INSERT_SORT };
void list_sort(struct list_head *, int);

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

static void list_insert_sorted(struct listitem *entry, struct list_head *head)
{
    struct listitem *item = NULL;

    if (list_empty(head)) {
        list_add(&entry->list, head);
        return;
    }

    list_for_each_entry (item, head, list) {
        if (cmpint(&entry->i, &item->i) < 0) {
            list_add_tail(&entry->list, &item->list);
            return;
        }
    }

    list_add_tail(&entry->list, head);
}

static void list_insertsort(struct list_head *head)
{
    struct list_head list_unsorted;
    struct listitem *item = NULL, *is = NULL;

    INIT_LIST_HEAD(&list_unsorted);
    list_splice_init(head, &list_unsorted);

    list_for_each_entry_safe (item, is, &list_unsorted, list) {
        list_del(&item->list);
        list_insert_sorted(item, head);
    }
}

static void list_qsort(struct list_head *head)
{
    struct list_head list_less, list_greater;
    struct listitem *pivot;
    struct listitem *item = NULL, *is = NULL;

    if (list_empty(head) || list_is_singular(head))
        return;

    INIT_LIST_HEAD(&list_less);
    INIT_LIST_HEAD(&list_greater);

    pivot = list_first_entry(head, struct listitem, list);
    list_del(&pivot->list);

    list_for_each_entry_safe (item, is, head, list) {
        if (cmpint(&item->i, &pivot->i) < 0)
            list_move_tail(&item->list, &list_less);
        else
            list_move(&item->list, &list_greater);
    }

    list_qsort(&list_less);
    list_qsort(&list_greater);

    list_add(&pivot->list, head);
    list_splice(&list_less, head);
    list_splice_tail(&list_greater, head);
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

static void (*list_sort_table[])(struct list_head *head) = {
    list_qsort, list_mergesort, list_insertsort};

void list_sort(struct list_head *head, int sort)
{
    list_sort_table[sort](head);
}

#endif /* end of include guard: LIST_SORT_H_7YOFEXAH */
