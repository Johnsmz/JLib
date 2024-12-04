#ifndef __JOHNSMZ_ARRAY_H__
#define __JOHNSMZ_ARRAY_H__

#include <stdio.h>
#include <stdlib.h>

#define __jlib_larray_section_width__ 0x10

typedef int size_l;
typedef void *element;
typedef struct __jlib_larray_section_prototype__ {element* data; unsigned int size; struct __jlib_larray_section_prototype__ *next, *prev;} *larray;

larray __jlib_larray_create__(void){larray out = (larray)malloc(sizeof(*out)); out->data = (element*)calloc(__jlib_larray_section_width__, sizeof(element)); out->size = 0; out->next = NULL; out->prev = NULL; return out;}

size_l __jlib_larray_size__(larray l){return l->size;}

element __jlib_larray_get__(larray l, int i){
    if(i >= (int)(l->size) || i < 0){return NULL;}
    if(i<__jlib_larray_section_width__){return l->data[i];}
    else{return __jlib_larray_get__(l->next, i-__jlib_larray_section_width__);}
}

void __jlib_larray_set__(larray l, int i, element d){
    if(i >= (int)(l->size) || i < 0){return;}
    if(i<__jlib_larray_section_width__){l->data[i] = d;}
    else{__jlib_larray_set__(l->next, i-__jlib_larray_section_width__, d);}
}

void __jlib_larray_roffset__(larray l){
    l->size += 1;
    if ((l->size) > __jlib_larray_section_width__) {__jlib_larray_roffset__(l->next); for (int i = __jlib_larray_section_width__-1; i > 0; i--){l->data[i] = l->data[i-1];} l->data[0] = l->prev->data[__jlib_larray_section_width__-1];}
    else if ((l->size) == __jlib_larray_section_width__) {l->next = __jlib_larray_create__(); l->next->prev = l; l->next->size = 1; l->next->data[0] = l->data[__jlib_larray_section_width__-1]; for (int i = __jlib_larray_section_width__-1; i > 0; i--){l->data[i] = l->data[i-1];} l->data[0] = l->prev->data[__jlib_larray_section_width__-1];}
    else if ((l->size) < __jlib_larray_section_width__) {for (int i = l->size; i > 0; i--){l->data[i] = l->data[i-1];} l->data[0] = l->prev->data[__jlib_larray_section_width__-1];}
}

void __jlib_larray_loffset__(larray l){
    l->prev->data[__jlib_larray_section_width__-1] = l->data[0];
    if (!(l->size)){l->prev->next = NULL; free(l);}
    else if (l->size <= __jlib_larray_section_width__) {for(int i = 0; i < (int)(l->size-1); i++){l->data[i] = l->data[i+1];} (l->size)-=1;}
    else {for(int i = 0; i < __jlib_larray_section_width__-1; i++){l->data[i] = l->data[i+1];} __jlib_larray_loffset__(l->next);  (l->size)-=1;}
}

void __jlib_larray_add__(larray l, int i, element d){
    if (i<0 || i > (int)(l->size)) {return;}
    (l->size) += 1;
    if (i<__jlib_larray_section_width__) {
        if (l->size<__jlib_larray_section_width__) {for(int j = l->size; j > i; j--){l->data[j] = l->data[j-1];} l->data[i] = d;}
        else if (l->size==__jlib_larray_section_width__) {
            l->next = __jlib_larray_create__(); l->next->prev = l; l->next->size += 1;
            if(i==__jlib_larray_section_width__){l->next->data[0] = d;}
            else{l->next->data[0] = l->data[__jlib_larray_section_width__-1];for(int j = __jlib_larray_section_width__-1; j > i; j--){l->data[j] = l->data[j-1];} l->data[i] = d;}
        } else {__jlib_larray_roffset__(l->next); for(int j = __jlib_larray_section_width__-1; j > i; j--){l->data[j] = l->data[j-1];} l->data[i] = d;}
    } else {__jlib_larray_add__(l->next, i-__jlib_larray_section_width__, d);}
}

void __jlib_larray_remove__(larray l, int i){
    if (i<0 || i >= (int)(l->size)){return;}
    (l->size) -= 1;
    if (i<__jlib_larray_section_width__) {
        if(!(l->size)){l->prev->next = NULL; free(l);}
        else if ((l->size) <= __jlib_larray_section_width__) {for(int j = i; j < (int)(l->size); j--){l->data[j] = l->data[j+1];}}
        else {for(int j = i; j < __jlib_larray_section_width__-1; j++){l->data[j] = l->data[j+1];} __jlib_larray_loffset__(l->next);}
    } else {__jlib_larray_remove__(l, i-__jlib_larray_section_width__);}
}

struct {
    larray      (*create)       (void);
    size_l      (*size)         (larray larray);
    element     (*get)          (larray larray, int index);
    void        (*set)          (larray larray, int index, element data);
    void        (*add)          (larray larray, int index, element data);
    void        (*remove)       (larray larray, int index);
} LArray = {
    __jlib_larray_create__,
    __jlib_larray_size__,
    __jlib_larray_get__,
    __jlib_larray_set__,
    __jlib_larray_add__,
    __jlib_larray_remove__
};

#endif
