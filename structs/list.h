#ifndef __LIST_H__
#define __LIST_H__

#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef void * element;

struct __LIST_H_NODE__ {
    element data;
    struct __LIST_H_NODE__ * prev;
    struct __LIST_H_NODE__ * next;
};

typedef struct {
    size_t size;
    struct __LIST_H_NODE__ * head;
    struct __LIST_H_NODE__ * tail;
} * list;

list __LIST_H_CREATE__ (void) {
    list out = (list)malloc(sizeof(*out));
    out->size = 0x0;
    out->head = (struct __LIST_H_NODE__*)malloc(sizeof(*(out->head)));
    out->tail = (struct __LIST_H_NODE__*)malloc(sizeof(*(out->tail)));
    out->head->data = NULL; out->head->prev = NULL; out->head->next = out->tail;
    out->tail->data = NULL; out->tail->prev = out->head; out->tail->next = NULL;
    return out;
}

size_t __LIST_H_SIZE__ (list l) { return l->size; }

element __LIST_H_GET__ (list l, size_t i) {
    if ( i < 0x0 || i >= l->size ) { errno = EINVAL; perror("List.get() => index out of bound"); exit(errno); }
    struct __LIST_H_NODE__ * ptr = l->head->next; while (i--) { ptr = ptr->next; } return ptr->data;
}

void __LIST_H_SET__ (list l, size_t i, element d) {
    if ( i < 0x0 || i >= l->size ) { errno = EINVAL; perror("List.set() => index out of bound"); exit(errno); }
    struct __LIST_H_NODE__ * ptr = l->head->next; while (i--) { ptr = ptr->next; } ptr->data = d;
}

void __LIST_H_INSERT__ (list l, size_t i, element data) {
    if ( i < 0x0 || i > l->size ) { errno = EINVAL; perror("List.insert() => index out of bound"); exit(errno); }
    struct __LIST_H_NODE__ * ptr = l->head; while (i--) { ptr = ptr->next; }
    struct __LIST_H_NODE__ * nw  = (struct __LIST_H_NODE__ *) malloc(sizeof(*nw));
    nw->data = data; nw->prev = ptr; nw->next = ptr->next; ptr->next = nw; nw->next->prev = nw;
    l->size++;
}

void __LIST_H_REMOVE__ (list l, size_t i) {
    if ( i < 0x0 || i >= l->size ) { errno = EINVAL; perror("List.remove() => index out of bound"); exit(errno); }
    struct __LIST_H_NODE__ * ptr = l->head; while (i--) { ptr = ptr->next; }
    struct __LIST_H_NODE__ * tar = ptr->next; ptr->next = tar->next; ptr->next->prev = ptr;
    free(tar); l->size--;
}

void __LIST_H_DESTROY__ (list l) {
    struct __LIST_H_NODE__ * ptr_1 = l->head, *ptr_2 = ptr_1->next;
    while ((l->size)--) { free(ptr_1); ptr_1 = ptr_2; ptr_2 = ptr_2?ptr_2->next:NULL; }
    if (ptr_2) { free(ptr_2); }
    free(l);
}

struct {
    list    (*create)   (void);
    size_t  (*size)     (list list);
    element (*get)      (list list, size_t index);
    void    (*set)      (list list, size_t index, element data);
    void    (*insert)   (list list, size_t index, element data);
    void    (*remove)   (list list, size_t index);
    void    (*destroy)  (list list);
} List = { __LIST_H_CREATE__, __LIST_H_SIZE__, __LIST_H_GET__, __LIST_H_SET__, __LIST_H_INSERT__, __LIST_H_REMOVE__, __LIST_H_DESTROY__ };

#endif
