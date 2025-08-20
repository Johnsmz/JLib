#ifndef __JOHNSMZ_STR_H__
#define __JOHNSMZ_STR_H__

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char *string;

typedef struct {
    size_t len;
    char *str;
} * str;

str str_create(void) {
    str s = (str)malloc(sizeof(*s));
    s->len = 0x0;
    s->str = NULL;
    return s;
}
str str_create_len(size_t len) {
    if (len < 0x0) {
        return NULL;
    }
    str s = (str)malloc(sizeof(*s));
    s->len = 0x0;
    s->str = (string)calloc(len, sizeof(char));
    return s;
}
str str_from_string(const string s) {
    str o = str_create();
    o->len = (int)strlen(s);
    o->str = (string)malloc(sizeof(char) * o->len);
    memcpy(o->str, s, sizeof(char) * o->len);
    return o;
}
size_t str_len(str s) {
    if (!s) {
        return 0x0;
    }
    return s->len;
}
str str_sub(str s, int x, int y) {
    if (x < 0x0 || y > s->len || x > y) {
        return NULL;
    }
    str o = str_create_len(y - x);
    for (int i = x; i < y; i++) {
        o->str[i - x] = s->str[i];
    }
    return o;
}
string str_to_string(str s) {
    string out = (string)malloc(sizeof(char) * s->len);
    memcpy(out, s->str, sizeof(char) * s->len);
    return out;
}
str str_cat(str a, str b) {
    if ( !a || !b ) {
        return NULL;
    }
    str out = str_create_len(a->len + b->len);
    memcpy(out->str, a->str, sizeof(char) * a->len);
    strncat(out->str, b->str, b->len);
    return out;
}
bool str_equal(str a, str b) {
    if (a->len != b->len) {
        return 0x0;
    }
    return !strncmp(a->str, b->str, a->len);
}

struct{
    str     (*create)       (void);
    str     (*create_len)   (size_t len);
    str     (*from_string)  (const string s);
    size_t  (*length)       (str s);
    str     (*sub)          (str s, int from, int to);
    string  (*to_string)    (str s);
    str     (*cat)          (str a, str b);
    bool    (*equal)        (str a, str b);
} String = {str_create, str_create_len,str_from_string,str_len,str_sub,str_to_string,str_cat,str_equal};

#endif
