#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdlib.h>

#define null NULL

typedef struct {
    int num, den;
} * frac;

#define frac_num(f) f->num
#define frac_den(f) f->den
#define frac_set_num(f,n) f->num=n
#define frac_set_den(f,d) f->den=d

int gcd(int a, int b) {
    while (b != 0) {
        a %= b;
        a ^= b;
        b ^= a;
        a ^= b;
    }
    return a;
}

int lcm(int a, int b) { return a * b / gcd(a, b); }

frac frac_create(int num, int den) {
    frac out = (frac)malloc(sizeof(*out));
    out->num = num;
    out->den = den;
    return out;
}

frac frac_simp(frac f) {
    if (f == null || f->den == 0) {
        return null;
    } else if (f->num == 0) {
        return frac_create(0, 1);
    } else if (f->num > 0 && f->den > 0) {
        int g = gcd(f->num, f->den);
        return frac_create(f->num / g, f->den / g);
    } else if (f->num < 0 && f->den > 0) {
        int g = gcd(-f->num, f->den);
        return frac_create(f->num / g, f->den / g);
    } else if (f->num > 0 && f->den < 0) {
        int g = gcd(f->num, -f->den);
        return frac_create(-f->num / g, -f->den / g);
    } else {
        int g = gcd(-f->num, -f->den);
        return frac_create(-f->num / g, -f->den / g);
    }
}

double frac_to_num(frac f) {
    if (f == null) {
        return 0;
    };
    if (f->den == 0) {
        if (f->num > 0) {
            return DBL_MAX;
        } else {
            return DBL_MIN;
        }
    }
    return (double)f->num / f->den;
}

frac num_to_frac(double num) {
    if (num == 0) {
        return frac_create(0, 1);
    }
    int dp = 1;
    while (num * dp - (int)(num * dp)) {
        dp *= 10;
    }
    frac a = frac_create((int)(num * dp), dp), b = frac_simp(a);
    free(a);
    return b;
}

frac frac_opp(frac f) {
    if (f == null) {
        return null;
    }
    return frac_create(-f->num, f->den);
}

frac frac_rec(frac f) {
    if (f == null || f->num == 0) {
        return null;
    }
    frac a = frac_create(f->den, f->num), b = frac_simp(a);
    free(a);
    return b;
}

frac frac_mult(frac a, frac b) {
    if (a == null || b == null) {
        return null;
    }
    frac c = frac_create(a->num * b->num, a->den * b->den), d = frac_simp(c);
    free(c);
    return d;
}

frac frac_div(frac a, frac b) {
    if (a == null || b == null) {
        return null;
    }
    frac c = frac_create(a->num * b->den, a->den * b->num), d = frac_simp(c);
    free(c);
    return d;
}

frac frac_add(frac a, frac b) {
    if (a == null || b == null) {
        return null;
    }
    int l = lcm(a->den, b->den);
    frac res = frac_create((a->num * l / a->den + b->num * l / b->den), l), s = frac_simp(res);
    free(res);
    return s;
}

frac frac_sub(frac a, frac b) {
    if (a == null || b == null) {
        return null;
    }
    int l = lcm(a->den, b->den);
    frac res = frac_create((a->num * l / a->den - b->num * l / b->den), l), s = frac_simp(res);
    free(res);
    return s;
}