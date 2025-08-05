#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <errno.h>
#include <limits.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

typedef int8_t *element;
typedef struct {
    int8_t *data;
    size_t size;
    size_t capacity;
    size_t shrink;
    size_t ele_size;
} *dvector;

dvector __dvector_create__(size_t _Ele_size) {
    dvector vec = (dvector)malloc(sizeof(*vec));
    vec->data = NULL;
    vec->size = 0x0;
    vec->capacity = 0x0;
    vec->shrink = 0x0;
    vec->ele_size = _Ele_size;
    return vec;
}

size_t __dvector_size__(dvector vec) { return vec->size; }

void __dvector_destroy__(dvector vec) {
    if (vec) {
        if (vec->data) { free(vec->data); }
        free(vec);
    }
    return;
}

element __dvector_get__(dvector vec, size_t i) {
    if (i > vec->size) {
        errno = EINVAL;
        perror("Vector.get() => index out of bound");
        exit(errno);
    }
    return (element)(vec->data + i*vec->ele_size);
}

void __dvector_modify__(dvector vec, size_t i, element val) {
    if (i >= vec->size) {
        errno = EINVAL;
        perror("Vector.modify() => index out of bound");
        exit(errno);
    }
    memcpy(vec->data + i*vec->ele_size , val, vec->ele_size);
    return;
}

void __dvector_insert__(dvector vec, size_t ii, element val) {
    if(!vec->capacity){ vec->capacity = 0x10; vec->data = (int8_t*)malloc(0x10*vec->ele_size); }
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
        vec->data = (int8_t*)realloc(vec->data, vec->capacity * vec->ele_size);
        if (vec->data == NULL) {
            errno = ENOSPC;
            perror("Vector.insert() => cannot alloc new memory");
            exit(errno);
        }
    }
    memmove(vec->data + ii + 0x1, vec->data + ii, (vec->size - ii) * vec->ele_size);
    memcpy(vec->data + ii*vec->ele_size, val, vec->ele_size);
    return;
}

void __dvector_remove__(dvector vec, size_t i) {
    if (i >= vec->size) {
        errno = EINVAL;
        perror("Vector.remove() => index out of bound");
        exit(errno);
    }
    memmove(vec->data + i, vec->data + i + 0x1, (--(vec->size) - i) * vec->ele_size);
    if (!vec->size) { free(vec->data); vec->capacity = 0x0; vec->shrink = 0x0; return; }
    if (vec->size<=vec->shrink) {
        vec->capacity = vec->shrink;
        vec->data = (int8_t*)realloc(vec->data, vec->capacity * vec->ele_size);
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
    dvector     (*create)       (size_t _Ele_size);
    size_t      (*size)         (dvector vec);
    void        (*destroy)      (dvector vec);
    element     (*get)          (dvector vec, size_t index);
    void        (*modify)       (dvector vec, size_t index, element value);
    void        (*insert)       (dvector vec, size_t index, element value);
    void        (*remove)       (dvector vec, size_t index);
} DVector = {__dvector_create__, __dvector_size__, __dvector_destroy__, __dvector_get__, __dvector_modify__, __dvector_insert__, __dvector_remove__};

#endif
