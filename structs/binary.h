#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
  bool *arr, neg;
  size_t len;
} * binary;

binary binary_create(int size) {
  binary out = (binary)malloc(sizeof(*out));
  out->arr = (bool*)calloc(size, sizeof(bool));
  out->len = size;
  out->neg = 0;
  return out;
}

void binary_free(binary bin) {
  free(bin->arr);
  free(bin);
  return;
}

size_t binary_length(binary bin) {
  if (bin->neg) {
    return bin->len + 1;
  }
  return bin->len;
}

int bin_length(int num) {
  if (num == 0) {
    return 0;
  }
  int out;
  if (num > 0) {
    for (int i = 1; num; i++) {
      if (num & 1) {
        out = i;
      }
      num >>= 1;
    }
  } else {
    num = -num;
    for (int i = 1; num; i++) {
      if (num & 1) {
        out = i;
      }
      num >>= 1;
    }
    out *= -1;
  }
  return out;
}

binary Bin_fromint(int num) {
  int len = bin_length(num);
  if (num == 0) {
    len = 1;
  }
  if (num < 0) {
    len *= -1;
  }
  binary out = binary_create(len);
  if (num < 0) {
    out->neg = 1;
    num *= -1;
  }
  for (int i = 0; i < out->len; i++) {
    out->arr[i] = num & 1;
    num >>= 1;
  }
  return out;
}

binary Bin_fromint_size(int num, int size) {
  int len = bin_length(num);
  if (len > size) {
    return NULL;
  }
  binary out = binary_create(size);
  if (num < 0) {
    out->neg = -1;
    num *= -1;
  }
  for (int i = 0; i < len; i++) {
    out->arr[i] = num & 1;
    num >>= 1;
  }
  return out;
}

int binpow(int pow) {
  if (pow == 0) {
    return 1;
  }
  if (pow < 0) {
    return 0;
  }
  int out = 1;
  for (int i = 0; i < pow; i++) {
    out *= 2;
  }
  return out;
}

int Bin_toint(binary bin) {
  int out = 0;
  for (int i = 0; i < bin->len; i++) {
    out += bin->arr[i] * binpow(i);
  }
  if (bin->neg) {
    out *= -1;
  }
  return out;
}

char *Bin_tostr(binary bin) {
  char *out = (char*)malloc(binary_length(bin) * sizeof(char));
  if (bin->neg) {
    out[0] = '-';
    for (int i = 0; i < bin->len; i++) {
      out[i + 1] = bin->arr[bin->len - i - 1] + '0';
    }
  } else {
    for (int i = 0; i < bin->len; i++) {
      out[i] = bin->arr[bin->len - 1 - i] + '0';
    }
  }
  return out;
}

int Binary_count(int num) {
  if (num == 0) {
    return 0;
  }
  int out = 0;
  if (num > 0) {
    while (num) {
      out += num & 1;
      num >>= 1;
    }
  } else {
    num *= -1;
    while (num) {
      out += num & 1;
      num >>= 1;
    }
    out *= -1;
  }
  return out;
}

binary Bin_add(binary a, binary b) { return Bin_fromint(Bin_toint(a) + Bin_toint(b)); }

binary Bin_minus(binary a, binary b) { return Bin_fromint(Bin_toint(a) - Bin_toint(b)); }

binary Bin_multiply(binary a, binary b) { return Bin_fromint(Bin_toint(a) * Bin_toint(b)); }

binary Bin_fromstr(char *str) {
  int len = strlen(str);
  binary out;
  if (str[0] == '-') {
    out = binary_create(len - 1);
    out->neg = 1;
    for (int i = 1; i < len; i++) {
      if (str[i] != '0' && str[i] != '1') {
        binary_free(out);
        return NULL;
      }
      out->arr[i - 1] = str[len - i] - '0';
    }
  } else {
    out = binary_create(len);
    for (int i = 0; i < len; i++) {
      if (str[i] != '0' && str[i] != '1') {
        binary_free(out);
        return NULL;
      }
      out->arr[i] = str[len - i - 1] - '0';
    }
  }
  return out;
}