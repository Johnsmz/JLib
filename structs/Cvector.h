#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <errno.h>
#include <stdlib.h>
#include <string.h>

extern int errno;

typedef void *element;
typedef struct {
    element *data;
    size_t size;
    size_t capacity;
} *vector;

vector __vector_create__() {
    vector vec = (vector)malloc(sizeof(*vec));
    vec->data = NULL;
    vec->size = 0x0;
    vec->capacity = 0x0;
    return vec;
}

size_t __vector_size__(vector vec) { return vec->size; }

void __vector_destroy__(vector vec) {
    if (vec) {
        if (vec->data) { free(vec->data); }
        free(vec);
    }
    return;
}

element __vector_get__(vector vec, size_t i) {
    if (i < 0x0 || i > vec->size) {
        errno = EINVAL;
        perror("Vector.get() => index out of bound");
        exit(errno);
    }
    return vec->data[i];
}

void __vector_modify__(vector vec, size_t i, element val) {
    if (i < 0x0 || i >= vec->size) {
        errno = EINVAL;
        perror("Vector.modify() => index out of bound");
        exit(errno);
    }
    vec->data[i] = val;
    return;
}

void __vector_insert__(vector vec, size_t ii, element val) {
    if (++(vec->size) >= vec->capacity) {
        if      (vec->capacity < 0x40)      { vec->capacity += 0x10; }
        else if (vec->capacity < 0x100)     { vec->capacity += 0x20; }
        else if (vec->capacity < 0x1000)    { vec->capacity += 0x100; }
        else                                { vec->capacity += 0x1000; }
        vec->data = realloc(vec->data, vec->capacity * sizeof(element));
        if (vec->data == NULL) {
            errno = ENOSPC;
            perror("Vector.insert() => cannot alloc new memory");
            exit(errno);
        }
    }
    memmove(vec->data + ii + 0x1, vec->data + ii, (vec->size - ii) * sizeof(element));
    vec->data[ii] = val;
    return;
}

void __vector_delete__(vector vec, size_t i) {
    if (i < 0x0 || i >= vec->size) {
        errno = EINVAL;
        perror("Vector.delete() => index out of bound");
        exit(errno);
    }
    memmove(vec->data + i, vec->data + i + 0x1, (--(vec->size) - i) * sizeof(element));
    if (vec->capacity > 0x10) {
        if (vec->size <= 0x40) {
            if(vec->capacity >= vec->size + 0x10){ vec->capacity -= 0x10; vec->data = realloc(vec->data, vec->capacity * sizeof(element)); }
        } else if (vec->size <= 0x100) {
            if(vec->capacity >= vec->size + 0x20){ vec->capacity -= 0x20; vec->data = realloc(vec->data, vec->capacity * sizeof(element)); }
        } else if (vec->size <= 0x1000) {
            if(vec->capacity >= vec->size + 0x100){ vec->capacity -= 0x100; vec->data = realloc(vec->data, vec->capacity * sizeof(element)); }
        } else if (vec->capacity >= vec->size + 0x1000) { vec->capacity -= 0x1000; vec->data = realloc(vec->data, vec->capacity * sizeof(element)); }
    }
    if (vec->data == NULL) {
        errno = ENOSPC;
        perror("Vector.delete() => cannot free memory");
        exit(errno);
    }
    return;
}

struct {
    vector      (*create)       ();
    size_t      (*size)         (vector vec);
    void        (*destroy)      (vector vec);
    element     (*get)          (vector vec, size_t index);
    void        (*modify)       (vector vec, size_t index, element value);
    void        (*insert)       (vector vec, size_t index, element value);
    void        (*delete)       (vector vec, size_t index);
} Vector = {__vector_create__, __vector_size__, __vector_destroy__, __vector_get__, __vector_modify__, __vector_insert__, __vector_delete__};

#endif
