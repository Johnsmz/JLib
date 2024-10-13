#ifndef __JOHNSMZ_ARRAY_H__
#define __JOHNSMZ_ARRAY_H__

#include <stdio.h>
#include <stdlib.h>

typedef void *element;
typedef struct {element* data; unsigned int size; unsigned int capacity;} *array;


array __jlib_array_create__(void) {
    array arr = (array)malloc(sizeof(array));
    arr->data = (element*)malloc(sizeof(element) * 0x10);
    arr->size = 0x0;
    arr->capacity = 0x10;
    return arr;
}

array __jlib_array_create_with_capacity__(unsigned int capacity) {
    array arr = (array)malloc(sizeof(array));
    arr->data = (element*)malloc(sizeof(element) * capacity);
    arr->size = 0x0;
    arr->capacity = capacity;
    return arr;
}

void __jlib_array_expand_capacity__(array arr) {
    int newCapacity = (arr->capacity) + ((arr->capacity)>>1);
    arr->data = (element*)realloc(arr->data, sizeof(element) * newCapacity);
    if (arr->data == NULL) {
        fprintf(stderr, "\033[31mError\033[0m Memory allocation failed\n");
        exit(1);
    }
    arr->capacity = newCapacity;
}


void __jlib_array_append__(array arr, element elem) {
    if (arr->size == arr->capacity) {
        __jlib_array_expand_capacity__(arr);
    }
    arr->data[arr->size++] = elem;
}


element __jlib_array_get__(array arr, unsigned int index) {
    if (index < 0 || index >= arr->size) {
        fprintf(stderr, "\033[31mError\033[0m Index out of bounds\n");
        exit(1);
    }
    return arr->data[index];
}


void __jlib_array_set__(array arr, unsigned int index, element elem) {
    if (index < 0 || index >= (arr->size)) {
        fprintf(stderr, "\033[31mError\033[0m Index out of bounds\n");
        exit(1);
    }
    arr->data[index] = elem;
}


void __jlib_array_insert__(array arr, unsigned int index, element elem) {
    if (index < 0 || index > arr->size) {
        fprintf(stderr, "\033[31mError\033[0m Index out of bounds\n");
        exit(1);
    }

    if (arr->size == arr->capacity) {
        __jlib_array_expand_capacity__(arr);
    }

    for (unsigned int i = arr->size; i > index; i--) {
        arr->data[i] = arr->data[i - 1];
    }

    arr->data[index] = elem;
    arr->size++;
}


void __jlib_array_remove__(array arr, unsigned int index) {
    if (index < 0 || index >= arr->size) {
        fprintf(stderr, "Error: Index out of bounds\n");
        exit(1);
    }

    for (unsigned int i = index; i < arr->size - 1; i++) {
        arr->data[i] = arr->data[i + 1];
    }

    arr->size--;

    if (arr->size < arr->capacity / 4) {
        int newCapacity = arr->capacity / 2;
        element* newData = (element*)realloc(arr->data, sizeof(element) * newCapacity);
        if (newData == NULL) {
            fprintf(stderr, "Error: Memory allocation failed\n");
            exit(1);
        }
        arr->data = newData;
        arr->capacity = newCapacity;
    }
}


unsigned int __jlib_array_size__(array arr) {
    return arr->size;
}

void __jlib_array_free__(array arr) {free(arr->data);free(arr);}

struct {
    array   (*create)   (void);
    array   (*create_capacity)  (unsigned int capacity);
    void    (*append)   (array arr, element elem);
    element (*get)      (array arr, unsigned int index);
    void    (*set)      (array arr, unsigned int index, element elem);
    void    (*insert)   (array arr, unsigned int index, element elem);
    void    (*remove)   (array arr, unsigned int index);
    unsigned int  (*size)   (array arr);
    void    (*free)     (array arr);
} Array = {
    __jlib_array_create__,
    __jlib_array_create_with_capacity__,
    __jlib_array_append__,
    __jlib_array_get__,
    __jlib_array_set__,
    __jlib_array_insert__,
    __jlib_array_remove__,
    __jlib_array_size__,
    __jlib_array_free__
};

#endif
