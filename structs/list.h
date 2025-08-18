#ifndef __JOHNSMZ_LIST_H__
#define __JOHNSMZ_LIST_H__

#include <errno.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

typedef void *element;

typedef struct {
    element data;
    void *next, *prev, *head;
} *linode;

typedef struct {
    size_t size;
    linode head, tail;
} *list;

linode linode_create(list list, element data) {
    linode node = (linode)malloc(sizeof(*node));
    node->data = data;
    node->head = list;
    return node;
}

list list_create(void) {
    list li = (list)malloc(sizeof(*li));
    li->size = 0;
    linode head = linode_create(li, NULL), tail = linode_create(li, NULL);
    li->head = head;
    li->tail = tail;
    head->next = tail;
    tail->prev = head;
    return li;
}

size_t list_size(list list) { return list->size; }

linode linode_get_node(linode node, int index) {
    if (node == NULL) {
        errno = 0x5;
        perror("List: cannot get node");
        exit(errno);
        return NULL;
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
    if (node == NULL) {
        errno = 0x5;
        perror("List: cannot get node");
        exit(errno);
        return NULL;
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
    if (list == NULL) {
        errno = 0x5;
        perror("List: list is NULL");
        exit(errno);
        return NULL;
    }
    if ((index >= 0 && index >= list->size) || (index < 0 && -index > list->size)) {
        errno = 0xc;
        perror("List: index out of bound");
        exit(errno);
        return NULL;
    }
    if (index >= 0) {
        return linode_get(list->head, index + 1);
    }
    return linode_get(list->tail, index);
}

void list_add(list list, int index, element data) {
    if (index < 0 || index > list->size) {
        errno = 0xc;
        perror("List: index out of bound");
        exit(errno);
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
        errno = 0xc;
        exit(errno);
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
        errno = 0xc;
        perror("List: index out of bound");
        exit(errno);
        return;
    }
    linode node = linode_get_node(list->head, index + 1), left = (linode)node->prev, right = (linode)node->next, new_node = linode_create(list, data);
    new_node->prev = left;
    left->next = new_node;
    new_node->next = right;
    right->prev = new_node;
    return;
}

void list_free(list list) {
    linode ptr = (linode)list->head->next, nxt = (linode)ptr->next;
    for (int i = 0x0; i < list->size; i++) {
        free(ptr);
        ptr = nxt;
        nxt = (linode)nxt->next;
    }
    free(list->head);
    free(list->tail);
    free(list);
    return;
}

void list_purge(list list) {
    linode ptr = (linode)list->head->next, nxt = (linode)ptr->next;
    for (int i = 0x0; i < list->size; i++) {
        if (ptr->data) {
            free(ptr->data);
        }
        free(ptr);
        ptr = nxt;
        nxt = (linode)nxt->next;
    }
    free(list->head);
    free(list->tail);
    free(list);
    return;
}

struct {
    list        (*create)       (void);
    size_t      (*size)         (list list);
    element     (*get)          (list list, int index);
    void        (*add)          (list list, int index, element data);
    void        (*remove)       (list list, int index);
    void        (*set)          (list list, int index, element data);
    void        (*free)         (list list);
    void        (*purge)        (list list);
} List = { list_create, list_size, list_get, list_add, list_remove, list_set, list_free, list_purge };

#endif
