#ifndef LIST_H
#define LIST_H

#include "../defs.h"

#include <string.h>

struct _list_node_ {
  byte *data;
  struct _list_node_ *prev;
  struct _list_node_ *next;
};

typedef struct {
  size_t len;
  size_t element_size;
  struct _list_node_ *head;
  struct _list_node_ *tail;
} list;

int list_init(list *list, size_t element_size) {
  if (!list)
    return EXIT_FAILURE;

  list->len = 0x0;
  list->element_size = element_size;

  list->head = (struct _list_node_ *)malloc(sizeof(struct _list_node_));
  list->tail = (struct _list_node_ *)malloc(sizeof(struct _list_node_));

  if (!(list->head) || !(list->tail))
    return EXIT_FAILURE;

  list->head->data = null;
  list->head->next = list->tail;
  list->head->prev = null;

  list->tail->data = null;
  list->tail->next = null;
  list->tail->prev = list->head;

  return EXIT_SUCCESS;
}

ptr list_read(list *list, size_t index) {
  if (!list || index >= list->len)
    return null;

  struct _list_node_ *node = list->head->next;
  while (index--) {
    if (!node)
      return null;

    node = node->next;
  }

  return node->data;
}

int list_write(list *list, size_t index, ptr data) {
  if (!list || index >= list->len)
    return EXIT_FAILURE;

  struct _list_node_ *node = list->head->next;
  while (index--) {
    if (!node)
      return EXIT_FAILURE;

    node = node->next;
  }

  memcpy(node->data, data, list->element_size);

  return EXIT_SUCCESS;
}

int list_insert(list *list, size_t index, ptr data) {
  if (!list || index > list->len)
    return EXIT_FAILURE;

  struct _list_node_ *node = list->head;
  while (index--) {
    if (!node)
      return EXIT_FAILURE;

    node = node->next;
  }

  struct _list_node_ *new_node = (struct _list_node_ *)malloc(sizeof(struct _list_node_));

  if (!node || !(node->next) || !new_node)
    return EXIT_FAILURE;

  new_node->data = (byte *)malloc(list->element_size);
  memcpy(new_node->data, data, list->element_size);

  node->next->prev = new_node;
  new_node->next = node->next;

  node->next = new_node;
  new_node->prev = node;

  list->len++;

  return EXIT_SUCCESS;
}

int list_delete(list *list, size_t index) {
  if (!list || index >= list->len)
    return EXIT_FAILURE;

  struct _list_node_ *node = list->head;
  while (index--) {
    if (!node)
      return EXIT_FAILURE;

    node = node->next;
  }

  struct _list_node_ *del_node = node->next;
  if (!node || !del_node || !(del_node->next))
    return EXIT_FAILURE;

  node->next = del_node->next;
  del_node->next->prev = node;

  free(del_node->data);
  free(del_node);

  list->len--;

  return EXIT_SUCCESS;
}

#endif