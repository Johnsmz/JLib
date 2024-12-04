#ifndef __JOHNSMZ_LIST_H__
#define __JOHNSMZ_LIST_H__

#include <stddef.h>
#include <stdlib.h>
#define null NULL

typedef int err;
typedef int size_l;
typedef void *element;
typedef struct {
    element data;
    void *next, *prev, *head;
} * linode;
typedef struct {
    size_l size;
    linode head, tail;
} * list;
err listerr;
linode linode_create(list list, element data) {
    linode node = (linode)malloc(sizeof(*node));
    node->data = data;
    node->head = list;
    return node;
}
list list_create(void) {
    list li = (list)malloc(sizeof(*li));
    li->size = 0;
    linode head = linode_create(li, null), tail = linode_create(li, null);
    li->head = head;
    li->tail = tail;
    head->next = tail;
    tail->prev = head;
    return li;
}
size_l list_size(list list) { return list->size; }
linode linode_get_node(linode node, int index) {
    if (node == null) {
        listerr = 2;
        exit(listerr);
        return null;
    }
    if (!index) {
        return node;
    }
    if (index > 0) {
        while (index--) {
            node = (linode)node->next;
        }
        return node;
    }
    while (index++) {
        node = (linode)node->prev;
    }
    return node;
}
element linode_get(linode node, int index) {
    if (node == null) {
        listerr = 2;
        exit(listerr);
        return null;
    }
    if (!index) {
        return node;
    }
    if (index > 0) {
        while (index--) {
            node = (linode)node->next;
        }
        return node->data;
    }
    while (index++) {
        node = (linode)node->prev;
    }
    return node->data;
}
element list_get(list list, int index) {
    if (list == null) {
        listerr = 1;
        exit(listerr);
        return null;
    }
    if ((index >= 0 && index >= list->size) || (index < 0 && -index > list->size)) {
        listerr = 2;
        exit(listerr);
        return null;
    }
    if (index >= 0) {
        return linode_get(list->head, index + 1);
    }
    return linode_get(list->tail, index);
}
void list_add(list list, int index, element data) {
    if (index < 0 || index > list->size) {
        listerr = 2;
        exit(listerr);
        return;
    }
    linode left = linode_get_node(list->head, index), right = (linode)left->next, new_node = linode_create(list, data);
    new_node->prev = left;
    left->next = new_node;
    new_node->next = right;
    right->prev = new_node;
    list->size++;
    return;
}
void list_remove(list list, int index) {
    if ((index >= 0 && index >= list->size) || (index < 0 && -index > list->size)) {
        listerr = 2;
        exit(listerr);
        return;
    }
    linode node = linode_get_node(list->head, index + 1), left = (linode)node->prev, right = (linode)node->next;
    left->next = right;
    right->prev = left;
    list->size--;
    return;
}
void list_set(list list, int index, element data) {
    if ((index >= 0 && index >= list->size) || (index < 0 && -index > list->size)) {
        listerr = 2;
        exit(listerr);
        return;
    }
    linode node = linode_get_node(list->head, index + 1), left = (linode)node->prev, right = (linode)node->next, new_node = linode_create(list, data);
    new_node->prev = left;
    left->next = new_node;
    new_node->next = right;
    right->prev = new_node;
    return;
}

struct{
    list    (*create)   (void);
    size_l  (*size)     (list list);
    element (*get)      (list list, int index);
    void    (*add)      (list list, int index, element data);
    void    (*remove)   (list list, int index);
    void    (*set)      (list list, int index, element data);
} List = {list_create,list_size,list_get,list_add,list_remove,list_set};


#endif
