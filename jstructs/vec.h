#ifndef VEC_H
#define VEC_H

#include "../defs.h"

#include <string.h>

typedef struct {
  size_t len;
  size_t element_size;
  size_t capacity;
  size_t shrink;
  size_t step;
  byte *data;
} vec;

int vec_init(vec *vec, size_t element_size) {
  if (!vec)
    return EXIT_FAILURE;

  vec->len = 0x0;
  vec->element_size = element_size;
  vec->capacity = 0x0;
  vec->shrink = 0x0;
  vec->step = 0x0;
  vec->data = null;
  return EXIT_SUCCESS;
}

ptr vec_read(vec *vec, size_t index) {
  if (!vec || index >= vec->len)
    return null;
  return vec->data + index * vec->element_size;
}

int vec_write(vec *vec, size_t index, ptr data) {
  if (!vec || index >= vec->len)
    return EXIT_FAILURE;

  memcpy(vec->data + index * vec->element_size, data, vec->element_size);
  return EXIT_SUCCESS;
}

int vec_insert(vec *vec, size_t index, ptr data) {
  if (!vec || index > vec->len)
    return EXIT_FAILURE;

  if (!vec->capacity) {
    vec->capacity = 0x10;
    vec->data = (byte *)malloc(0x10 * vec->element_size);
  }
  if (++(vec->len) > vec->capacity) {
    vec->shrink = vec->capacity;
    if (vec->capacity < 0x40) {
      vec->capacity += 0x10;
    } else if (vec->capacity < 0x100) {
      vec->capacity += 0x20;
    } else if (vec->capacity < 0x1000) {
      vec->capacity += 0x100;
    } else {
      if (!(vec->step))
        vec->step = 0x1000;
      else if (vec->capacity >= ((vec->step) << 0x4))
        vec->step <<= 0x4;
      
      vec->capacity += (vec->step) >> 0x4;
    }
    vec->data = (byte *)realloc(vec->data, vec->capacity * vec->element_size);
    if (!vec->data)
      return EXIT_FAILURE;
  }
  memmove(vec->data + (index + 0x1) * vec->element_size, vec->data + index * vec->element_size, ((vec->len) - index) * (vec->element_size));
  memcpy(vec->data + index * vec->element_size, data, vec->element_size);
  return EXIT_SUCCESS;
}

int vec_delete(vec *vec, size_t index) {
  if (!vec || index >= vec->len)
    return EXIT_FAILURE;

  memmove(vec->data + index * vec->element_size, vec->data + (index + 0x1) * vec->element_size, (--(vec->len) - index) * vec->element_size);
  if (!(vec->len)) {
    free(vec->data);
    vec->capacity = 0x0;
    vec->shrink = 0x0;
    return EXIT_SUCCESS;
  }

  if (vec->len <= vec->shrink) {
    vec->capacity = vec->shrink;
    vec->data = (byte *)realloc(vec->data, vec->capacity * vec->element_size);

    if (vec->capacity == 0x10) {
      vec->shrink = 0x0;
    } else {
      if (vec->capacity == 0x1000) {
        vec->shrink = vec->capacity - 0x100;
        vec->step = 0x0;
      } else if (vec->capacity > 0x1000) {
        if (vec->capacity == vec->step)
          vec->step >>= 0x4;
        vec->shrink = vec->capacity - (vec->step >> 0x4);
      } else if (vec->capacity > 0x100) {
        vec->shrink = vec->capacity - 0x100;
      } else if (vec->capacity > 0x40) {
        vec->shrink = vec->capacity - 0x20;
      } else if (vec->capacity > 0x10) {
        vec->shrink = vec->capacity - 0x10;
      }
    }
  }
  if (!(vec->data))
    return EXIT_FAILURE;
  return EXIT_SUCCESS;
}

int vec_purge(vec *vec) {
  if (!vec)
    return EXIT_FAILURE;
  if (vec->data)
    free(vec->data);
  free(vec);
  return EXIT_SUCCESS;
}

#endif
