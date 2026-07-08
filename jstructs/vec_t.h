#ifndef VEC_H
#define VEC_H

#include "../defs.h"
#include <string.h>
#include <stdlib.h>

typedef struct {
  size_t len;
  size_t element_size;
  size_t capacity;
  size_t shrink;
  size_t step;
  byte *data;
} vec_core;

static int vec_core_init(vec_core *vc, size_t elem_sz) {
  if (!vc)
    return EXIT_FAILURE;
  vc->len = 0x0;
  vc->element_size = elem_sz;
  vc->capacity = 0x0;
  vc->shrink = 0x0;
  vc->step = 0x0;
  vc->data = null;
  return EXIT_SUCCESS;
}

static ptr vec_core_read(vec_core *vc, size_t index) {
  if (!vc || index >= vc->len)
    return null;
  return vc->data + index * vc->element_size;
}

static int vec_core_write(vec_core *vc, size_t index, ptr data) {
  if (!vc || index >= vc->len)
    return EXIT_FAILURE;
  memcpy(vc->data + index * vc->element_size, data, vc->element_size);
  return EXIT_SUCCESS;
}

static int vec_core_insert(vec_core *vc, size_t index, ptr data) {
  if (!vc || index > vc->len)
    return EXIT_FAILURE;

  if (!vc->capacity) {
    vc->capacity = 0x10;
    vc->data = (byte *)malloc(0x10 * vc->element_size);
  }
  if (++(vc->len) > vc->capacity) {
    vc->shrink = vc->capacity;
    if (vc->capacity < 0x40) {
      vc->capacity += 0x10;
    } else if (vc->capacity < 0x100) {
      vc->capacity += 0x20;
    } else if (vc->capacity < 0x1000) {
      vc->capacity += 0x100;
    } else {
      if (!(vc->step))
        vc->step = 0x1000;
      else if (vc->capacity >= ((vc->step) << 0x4))
        vc->step <<= 0x4;
      vc->capacity += (vc->step) >> 0x4;
    }
    byte *new_buf = (byte *)realloc(vc->data, vc->capacity * vc->element_size);
    if (!new_buf)
      return EXIT_FAILURE;
    vc->data = new_buf;
  }
  memmove(
    vc->data + (index + 0x1) * vc->element_size,
    vc->data + index * vc->element_size,
    ((vc->len) - index) * (vc->element_size)
  );
  memcpy(vc->data + index * vc->element_size, data, vc->element_size);
  return EXIT_SUCCESS;
}

static int vec_core_delete(vec_core *vc, size_t index) {
  if (!vc || index >= vc->len)
    return EXIT_FAILURE;

  memmove(
    vc->data + index * vc->element_size,
    vc->data + (index + 1) * vc->element_size,
    (--(vc->len) - index) * vc->element_size
  );
  if (!(vc->len)) {
    free(vc->data);
    vc->capacity = 0x0;
    vc->shrink = 0x0;
    return EXIT_SUCCESS;
  }

  if (vc->len <= vc->shrink) {
    vc->capacity = vc->shrink;
    byte *new_buf = (byte *)realloc(vc->data, vc->capacity * vc->element_size);
    if (!new_buf)
      return EXIT_FAILURE;
    vc->data = new_buf;

    if (vc->capacity == 0x10) {
      vc->shrink = 0x0;
    } else {
      if (vc->capacity == 0x1000) {
        vc->shrink = vc->capacity - 0x100;
        vc->step = 0x0;
      } else if (vc->capacity > 0x1000) {
        if (vc->capacity == vc->step)
          vc->step >>= 0x4;
        vc->shrink = vc->capacity - (vc->step >> 0x4);
      } else if (vc->capacity > 0x100) {
        vc->shrink = vc->capacity - 0x100;
      } else if (vc->capacity > 0x40) {
        vc->shrink = vc->capacity - 0x20;
      } else if (vc->capacity > 0x10) {
        vc->shrink = vc->capacity - 0x10;
      }
    }
  }
  return EXIT_SUCCESS;
}

static int vec_core_purge(vec_core *vc) {
  if (!vc)
    return EXIT_FAILURE;
  if (vc->data)
    free(vc->data);
  vc->data = null;
  return EXIT_SUCCESS;
}

#define vec(T) struct { vec_core core; T* raw; }

#define vec_init(v) do { \
  vec_core_init(&(v).core, sizeof(*(v).raw)); \
  (v).raw = (void*)(v).core.data; \
} while(0)

#define vec_sync_raw(v) ((v).raw = (void*)(v).core.data)

#define vec_len(v) ((v).core.len)
#define vec_cap(v) ((v).core.capacity)

#define vec_push(v, val) do { \
  vec_core_insert(&(v).core, vec_len(v), &(val)); \
  vec_sync_raw(v); \
} while(0)

#define vec_insert_at(v, idx, val) do { \
  vec_core_insert(&(v).core, (idx), &(val)); \
  vec_sync_raw(v); \
} while(0)

#define vec_delete_at(v, idx) do { \
  vec_core_delete(&(v).core, (idx)); \
  vec_sync_raw(v); \
} while(0)

#define vec_purge(v) do { \
  vec_core_purge(&(v).core); \
  (v).raw = null; \
} while(0)

#endif
