#include <assert.h>
#include <stdlib.h>
#include "list.h"

#include "common.h"

int main(void)
{
    struct list_head testlist, testlist2;
    struct listitem *item, *is = NULL;
    size_t i;

    INIT_LIST_HEAD(&testlist);
    INIT_LIST_HEAD(&testlist2);

    assert(list_empty(&testlist));
    assert(list_empty(&testlist2));

    for (i = 5; i < 10; i++) {
        item = (struct listitem *) malloc(sizeof(*item));
        assert(item);
        item->i = i;
        list_add_tail(&item->list, &testlist);
    }

    assert(!list_empty(&testlist));

    for (i = 0; i < 5; i++) {
        item = (struct listitem *) malloc(sizeof(*item));
        assert(item);
        item->i = i;
        list_add(&item->list, &testlist2);
    }

    assert(!list_empty(&testlist2));

    list_for_each_entry_safe (item, is, &testlist2, list) {
        list_move(&item->list, &testlist);
    }

    assert(list_empty(&testlist2));

    i = 0;
    list_for_each_entry_safe (item, is, &testlist, list) {
        assert(item->i == i);
        list_del(&item->list);
        free(item);
        i++;
    }

    assert(i == 10);
    assert(list_empty(&testlist));

    return 0;
}
