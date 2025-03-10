#ifndef LIST_H
#define LIST_H

#include <stddef.h>

typedef enum {
    METHOD_TYPE_PUBLIC,
    METHOD_TYPE_PRIVATE,
} MemberType;

typedef struct {
    MemberType type;
    char *name;
    size_t name_len;
    char *ret;
    size_t ret_len;
} MemberNode;

typedef struct {
    MemberNode *items;
    size_t len;
    size_t cap;
} MemberList;

MemberList *create_member_list(size_t capacity);
void free_member_list(MemberList *list);
void member_list_append(MemberList *list, MemberNode item);

const char *member_type_str(MemberType type);

#endif //LIST_H
