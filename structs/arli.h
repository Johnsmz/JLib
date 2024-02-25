#include <stdlib.h>
#include <string.h>
#define null NULL
#define elem void *
typedef struct {
    elem a;
    size_t size, block;
} * arli;

arli arli_create(size_t block_size) {
    arli out = (arli)malloc(sizeof(*out));
    out->size = 0;
    out->block = block_size;
    return out;
}
elem arli_get(arli li, int index) {
    if (li == null || index < 0 || index >= li->size) {
        exit(2);
    }
    elem out = malloc(li->block);
    memcpy(out, li->a + index * li->block, li->block);
    return out;
}
void arli_add(arli li, int index, elem data) {
    if (li == null || index < 0 || index > li->size) {
        exit(2);
    }
    li->size++;
    li->a = realloc(li->a, li->block * li->size);
    if (li->a == null) {
        exit(3);
    }
    memmove(li->a + li->block * index + li->block, li->a + li->block * index, li->block * (li->size - 1 - index));
    memcpy(li->a + li->block * index, data, li->block);
    return;
}
void arli_remove(arli li, int index) {
    if (li == null || index < 0 || index >= li->size) {
        exit(2);
    }
    li->size--;
    memmove(li->a + li->block * index, li->a + li->block * index + li->block, li->block * (li->size - index));
    li->a = realloc(li->a, li->block * li->size);
    return;
}
void arli_set(arli li, int index, elem data) {
    if (li == null || index < 0 || index >= li->size) {
        exit(2);
    }
    memcpy(li->a + index * li->block, data, li->block);
    return;
}
void *arli_grep(arli li) { return li->a; }
size_t arli_size(arli li) { return li->size; }
