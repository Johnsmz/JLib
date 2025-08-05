#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <errno.h>
#include <limits.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

typedef void *element;
typedef struct {
    element *data;
    size_t size;
    size_t capacity;
    size_t shrink;
} *vector;

vector __vector_create__() {
    vector vec = (vector)malloc(sizeof(*vec));
    vec->data = NULL;
    vec->size = 0x0;
    vec->capacity = 0x0;
    vec->shrink = 0x0;
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
    if (i > vec->size) {
        errno = EINVAL;
        perror("Vector.get() => index out of bound");
        exit(errno);
    }
    return vec->data[i];
}

void __vector_modify__(vector vec, size_t i, element val) {
    if (i >= vec->size) {
        errno = EINVAL;
        perror("Vector.modify() => index out of bound");
        exit(errno);
    }
    vec->data[i] = val;
    return;
}

void __vector_insert__(vector vec, size_t ii, element val) {
    if(!vec->capacity){vec->capacity = 0x10; vec->data = (element*)malloc(0x10*sizeof(element));}
    if (++(vec->size) >= vec->capacity) {
        
        vec->shrink = vec->capacity;
        if      (vec->capacity < 0x40)      { vec->capacity += 0x10; }
        else if (vec->capacity < 0x100)     { vec->capacity += 0x20; }
        else if (vec->capacity < 0x1000)    { vec->capacity += 0x100; }
        else {
            int log = 0x0, n = vec->capacity;
            while (n) { log++; n >>= 0x4; }
            vec->capacity += (0x1<<((log-0x2)<<0x2));
        }
        vec->data = (element*)realloc(vec->data, vec->capacity * sizeof(element));
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

void __vector_remove__(vector vec, size_t i) {
    if (i >= vec->size) {
        errno = EINVAL;
        perror("Vector.remove() => index out of bound");
        exit(errno);
    }
    memmove(vec->data + i, vec->data + i + 0x1, (--(vec->size) - i) * sizeof(element));
    if (!vec->size) { free(vec->data); vec->capacity = 0x0; vec->shrink = 0x0; return; }
    if (vec->size<=vec->shrink) {
        vec->capacity = vec->shrink;
        vec->data = (element*)realloc(vec->data, vec->capacity * sizeof(element));
        if (vec->capacity==0x10) { vec->shrink = 0x0; }
        else {
            if (vec->capacity>0x1000){
                unsigned int log = 0x0; unsigned int n = vec->capacity;
                while (n) { log++; n >>= 0x4; }
                if (vec->capacity == (size_t)(0x1<<((log-0x1)<<0x2))) { vec->shrink = vec->capacity - (0x1<<((log-0x3)<<0x2)); }
                else { vec->shrink = vec->capacity - (0x1<<((log-0x2)<<0x2)); }
            }
            else if (vec->capacity>0x100) { vec->shrink = vec->capacity - 0x100; }
            else if (vec->capacity>0x40)  { vec->shrink = vec->capacity - 0x20; }
            else if (vec->capacity>0x10)  { vec->shrink = vec->capacity - 0x10; }
        }
    }
    if (vec->data == NULL) {
        errno = ENOSPC;
        perror("Vector.remove() => cannot free memory");
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
    void        (*remove)       (vector vec, size_t index);
} Vector = {__vector_create__, __vector_size__, __vector_destroy__, __vector_get__, __vector_modify__, __vector_insert__, __vector_remove__};

#endif
