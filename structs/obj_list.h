#include <stdlib.h>
#define null NULL

typedef void *element;

typedef int err;
typedef int size_l;
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
    element tmp;
    linode head = linode_create(li, tmp), tail = linode_create(li, tmp);
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
        element tmp;
        return tmp;
    }
    if (!index) {
        element tmp;
        return tmp;
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
        element tmp;
        return tmp;
    }
    if ((index >= 0 && index >= list->size) || (index < 0 && -index > list->size)) {
        listerr = 2;
        exit(listerr);
        element tmp;
        return tmp;
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
void list_del(linode node) {
    linode left = (linode)node->prev, right = (linode)node->next;
    left->next = right;
    right->prev = left;
    list li = (list)(node->head);
    li->size--;
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
void list_change(linode node, element data) {
    linode left = (linode)node->prev, right = (linode)node->next, new_node = linode_create((list)node->head, data);
    new_node->prev = left;
    left->next = new_node;
    new_node->next = right;
    right->prev = new_node;
    return;
}
void list_append(list list, element data) {
    if (list == null) {
        return;
    }
    linode node = linode_create(list, data), next = list->tail, prev = (linode)next->prev;
    prev->next = node;
    node->prev = prev;
    node->next = next;
    next->prev = node;
    list->size++;
    return;
}
void list_prepend(list list, element data) {
    if (list == null) {
        return;
    }
    linode node = linode_create(list, data), prev = list->head, next = (linode)prev->next;
    prev->next = node;
    node->prev = prev;
    node->next = next;
    next->prev = node;
    list->size++;
    return;
}
void list_popback(list list) {
    if (list == null || !list->size) {
        return;
    }
    linode next = list->tail, prev = (linode)next->prev;
    prev = (linode)prev->prev;
    prev->next = next;
    next->prev = prev;
    list->size--;
    return;
}
void list_popfront(list list) {
    if (list == null || !list->size) {
        return;
    }
    linode prev = list->head, next = (linode)prev->next;
    next = (linode)next->next;
    prev->next = next;
    next->prev = prev;
    list->size--;
    return;
}
