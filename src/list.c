#include "list.h"
#include <stdlib.h>
#include <string.h>

MemberList *create_member_list(size_t capacity) {
    MemberList *list = (MemberList *)malloc(sizeof(MemberList));
    list->items = (MemberNode *)malloc(capacity * sizeof(MemberNode));
    list->cap = capacity;
    list->len = 0;

    memset(list->items, 0, capacity * sizeof(MemberNode));
    return list;
}

void free_member_list(MemberList *list) {
    free(list->items);
    free(list);
}

void member_list_append(MemberList *list, MemberNode item) {
    if (list->len >= list->cap) {
        list->cap *= 2;
        list->items = realloc(list->items, list->cap * sizeof(MemberNode));
    }

    list->items[list->len++] = item;
}

const char *member_type_str(MemberType type) {
    switch (type) {
        case METHOD_TYPE_PRIVATE:
            return "private";
        case METHOD_TYPE_PUBLIC:
            return "public";
    }
}
