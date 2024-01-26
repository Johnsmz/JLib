/* Build by Johnsmz */

#include <algorithms/binaries.h>
#include <algorithms/sort.h>
#include <complex.h>
#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <limits.h>
#include <locale.h>
#include <math.h>
#include <memory.h>
#include <pthread.h>
#include <setjmp.h>
#include <signal.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <structs/binary.h>
#include <structs/frac.h>
#include <structs/list.h>
#include <structs/str.h>
#include <structs/vector.h>
#include <tgmath.h>
#include <time.h>
#include <unistd.h>
#include <wctype.h>
#define null NULL
#define max(a, b) (((a) > (b)) ? (a) : (b))
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define length(str) strlen(str)
#define cti(c) c-'0'
#define itc(n) n+'0'
#define cap(c) c-'a'+'A'
#define decap(c) c-'A'+'a'
#define neg(n) -n
#define none(n) !n
#define cpos(n) n>0
#define cneg(n) n<0
#define qabs(n) (((n)<(0))?(-n):(n))
#define alog(x,y) log(y)/log(x)
#define type(var) _Generic((var),                                     \
            _Bool: 7,                  unsigned char: -2,             \
             char: 2,                    signed char: 22,             \
        short int: 33,            unsigned short int: -33,            \
              int: 3,                   unsigned int: -3,             \
         long int: 333,            unsigned long int: -333,           \
    long long int: 3333,      unsigned long long int: -3333,          \
            float: 4,                         double: 5,              \
      long double: 55,                        char *: 6,              \
           void *: 1,                          int *: 31,             \
           float*: 41,                      double *: 51,             \
          default: 0)
typedef int err;
typedef void *element;
typedef char *string;

err error = 0;
int LARGE_INT = 8000000;
long long LARGE_LONGLONG = 1000000000000000000;
extern int errno;

void print(string print) { printf("%s", print); }

void pprint(int n,...){
    va_list vl;
    va_start(vl,n);
    for(int i=0;i<n;i++){
        string str=va_arg(vl,string);
        printf("%s",str);
    }
    return;
}

double absolute(double in) {
    if (in < 0) {
        return -in;
    }
    return in;
}

bool equals(string ar1, string ar2) {
    if (strcmp(ar1, ar2) == 0) {
        return 1;
    }
    return 0;
}

void rand_s(void) {
    srand(time(NULL));
    rand();
}

double randd(void) { return (double)rand() / RAND_MAX; }

void suspend(double ms) {
    struct timespec ts;
    ts.tv_sec = (int)ms / 1000;
    ts.tv_nsec = (int)(ms * 1000000) % 1000000000;
    nanosleep(&ts, null);
    return;
}

void swap(void *a, void *b) {
    void *tmp = a;
    a = b;
    b = tmp;
}

int digit(int num) {
    num = qabs(num);
    int out = 0;
    while (num) {
        num /= 10;
        out++;
    }
    return out;
}

string num_to_str(double num, int dp) {
    if (dp < 0) {
        return null;
    }
    if (dp == 0) {
        if (none(num)) {
            return (string)"0";
        }
        int nn = (int)num, dig = digit(nn);
        if (cpos(nn)) {
            string out = (string)malloc(sizeof(char) * dig);
            for (int i = dig - 1; i >= 0; i--) {
                out[i] = nn % 10 + '0';
                nn /= 10;
            }
            return out;
        } else {
            nn = qabs(nn);
            string out = (string)malloc(sizeof(char) * (dig + 1));
            out[0] = '-';
            for (int i = dig; i > 0; i--) {
                out[i] = nn % 10 + '0';
                nn /= 10;
            }
            return out;
        }
    } else {
        if (none(num)) {
            string out = (string)malloc(sizeof(char) * (2 + dp));
            out[0] = '0';
            out[1] = '.';
            for (int i = 0; i < dp; i++) {
                out[i + 2] = '0';
            }
        }
        int dig = digit((int)num);
        dig = (dig) ? dig : 1;
        if (cpos(num)) {
            string out = (string)malloc(sizeof(char) * (dig + 1 + dp));
            int nn = (int)num;
            for (int i = dig - 1; i >= 0; i--) {
                out[i] = nn % 10 + '0';
                nn /= 10;
            }
            out[dig] = '.';
            num -= (int)num;
            num *= 10;
            for (int i = 1; i <= dp; i++) {
                out[i + dig] = ((int)num) % 10 + '0';
                num *= 10;
            }
            return out;
        } else {
            num = qabs(num);
            string out = (string)malloc(sizeof(char) * (dig + 2 + dp));
            int nn = (int)num;
            out[0] = '-';
            for (int i = dig; i > 0; i--) {
                out[i] = nn % 10 + '0';
                nn /= 10;
            }
            out[dig + 1] = '.';
            dp++;
            num *= 10;
            for (int i = 2; i <= dp; i++) {
                out[i + dig] = ((int)num) % 10 + '0';
                num *= 10;
            }
            return out;
        }
    }
}

double str_to_num(string str) {
    int len = (int)length(str);
    double out = 0;
    if (str[0] == '+' || str[0] == '-') {
        for (int i = 1; i < len; i++) {
            if (str[i] != '.' && (str[i] < '0' || str[i] > '9')) {
                return 0;
            }
            if (str[i] == '.') {
                int k = -1;
                i++;
                while (i < len) {
                    out += (str[i++] - '0') * pow(10, k--);
                }
            } else {
                out *= 10;
                out += str[i] - '0';
            }
        }
        if (str[0] == '-') {
            out *= -1;
        }
        return out;
    }
    for (int i = 0; i < len; i++) {
        if (str[i] != '.' && (str[i] < '0' || str[i] > '9')) {
            return 0;
        }
        if (str[i] == '.') {
            int k = -1;
            i++;
            while (i < len) {
                out += (str[i++] - '0') * pow(10, k--);
            }
        } else {
            out *= 10;
            out += str[i] - '0';
        }
    }
    return out;
}