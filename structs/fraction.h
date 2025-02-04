#ifndef __JOHNSMZ_FRACTION_H__
#define __JOHNSMZ_FRACTION_H__

#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define null NULL

typedef struct {int n, d;} * fraction;

int __jlib_frac_gcd__(int a, int b) {
    while (b != 0) {
        a %= b;
        a ^= b;
        b ^= a;
        a ^= b;
    }
    return a;
}

int __jlib_frac_lcm__(int a, int b) { return a * b / __jlib_frac_gcd__(a, b); }

fraction __jlib_fraction_create__(int n, int d) {
    fraction out = (fraction)malloc(sizeof(*out));
    out->n = n;
    out->d = d;
    return out;
}

fraction __jlib_fraction_simplify__(fraction f) {
    if (f == null || f->d == 0) {
        return null;
    } else if (f->n == 0) {
        return __jlib_fraction_create__(0, 1);
    } else if (f->n > 0 && f->d > 0) {
        int g = __jlib_frac_gcd__(f->n, f->d);
        return __jlib_fraction_create__(f->n / g, f->d / g);
    } else if (f->n < 0 && f->d > 0) {
        int g = __jlib_frac_gcd__(-f->n, f->d);
        return __jlib_fraction_create__(f->n / g, f->d / g);
    } else if (f->n > 0 && f->d < 0) {
        int g = __jlib_frac_gcd__(f->n, -f->d);
        return __jlib_fraction_create__(-f->n / g, -f->d / g);
    } else {
        int g = __jlib_frac_gcd__(-f->n, -f->d);
        return __jlib_fraction_create__(-f->n / g, -f->d / g);
    }
}

double __jlib_fraction_to_n__(fraction f) {
    if (f == null) {
        return 0;
    };
    if (f->d == 0) {
        if (f->n > 0) {
            return DBL_MAX;
        } else {
            return DBL_MIN;
        }
    }
    return (double)f->n / f->d;
}

fraction __jlib_n_to_fraction__(double n) {
    if (n == 0) {
        return __jlib_fraction_create__(0, 1);
    }
    int dp = 1;
    while (n * dp - (int)(n * dp)) {
        dp *= 10;
    }
    fraction a = __jlib_fraction_create__((int)(n * dp), dp), b = __jlib_fraction_simplify__(a);
    free(a);
    return b;
}

fraction __jlib_fraction_opp__(fraction f) {
    if (f == null) {
        return null;
    }
    return __jlib_fraction_create__(-f->n, f->d);
}

fraction __jlib_fraction_inverse__(fraction f) {
    if (f == null || f->n == 0) {
        return null;
    }
    fraction a = __jlib_fraction_create__(f->d, f->n), b = __jlib_fraction_simplify__(a);
    free(a);
    return b;
}

fraction __jlib_fraction_mult__(fraction a, fraction b) {
    if (a == null || b == null) {
        return null;
    }
    fraction c = __jlib_fraction_create__(a->n * b->n, a->d * b->d), d = __jlib_fraction_simplify__(c);
    free(c);
    return d;
}

fraction __jlib_fraction_div__(fraction a, fraction b) {
    if (a == null || b == null) {
        return null;
    }
    fraction c = __jlib_fraction_create__(a->n * b->d, a->d * b->n), d = __jlib_fraction_simplify__(c);
    free(c);
    return d;
}

fraction __jlib_fraction_add__(fraction a, fraction b) {
    if (a == null || b == null) {
        return null;
    }
    int l = __jlib_frac_lcm__(a->d, b->d);
    fraction res = __jlib_fraction_create__((a->n * l / a->d + b->n * l / b->d), l), s = __jlib_fraction_simplify__(res);
    free(res);
    return s;
}

fraction __jlib_fraction_sub__(fraction a, fraction b) {
    if (a == null || b == null) {
        return null;
    }
    int l = __jlib_frac_lcm__(a->d, b->d);
    fraction res = __jlib_fraction_create__((a->n * l / a->d - b->n * l / b->d), l), s = __jlib_fraction_simplify__(res);
    free(res);
    return s;
}

struct{
    fraction    (*create)       (int n, int d);
    fraction    (*simplify)     (fraction f);
    double      (*to_double)    (fraction f);
    fraction    (*from_double)  (double n);
    fraction    (*opposite)     (fraction f);
    fraction    (*inverse)      (fraction f);
    fraction    (*multiply)     (fraction a, fraction b);
    fraction    (*divide)       (fraction a, fraction b);
    fraction    (*add)          (fraction a, fraction b);
    fraction    (*subtract)     (fraction a, fraction b);
} Fraction = {__jlib_fraction_create__, __jlib_fraction_simplify__, __jlib_fraction_to_n__, __jlib_n_to_fraction__,__jlib_fraction_opp__,__jlib_fraction_inverse__,__jlib_fraction_mult__,__jlib_fraction_div__,__jlib_fraction_add__,__jlib_fraction_sub__};

#endif
