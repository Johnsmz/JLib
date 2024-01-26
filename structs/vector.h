#include <stdio.h>
#include <stdlib.h>

typedef struct {
  size_t dimension;
  double *arr;
} * vector;

vector vector_create(int dimension) {
  vector out = (vector)malloc(sizeof(*out));
  out->dimension = dimension;
  out->arr = (double*)calloc(dimension, sizeof(double));
  return out;
}

void vector_free(vector vec) {
  free(vec->arr);
  free(vec);
  return;
}

double vector_get(vector vec, int dim) {
  if (dim < 0 || dim >= vec->dimension) {
    return 0;
  }
  return vec->arr[dim];
}

void vector_set(vector vec, int dim, double val) {
  if (dim < 0 || dim >= vec->dimension) {
    return;
  }
  vec->arr[dim] = val;
  return;
}

vector vector_clone(vector vec) {
  vector out = vector_create(vec->dimension);
  for (int i = 0; i < vec->dimension; i++) {
    out->arr[i] = vec->arr[i];
  }
  return out;
}

vector vector_resize(vector vec, int dim) {
  if (dim <= 0) {
    return NULL;
  }
  vector out = vector_create(dim);
  for (int i = 0; i < dim && i < vec->dimension; i++) {
    out->arr[i] = vec->arr[i];
  }
  for (int i = vec->dimension; i < dim; i++) {
    out->arr[i] = 0;
  }
  return out;
}

vector vector_multiply(vector vec, double mult) {
  vector out = vector_create(vec->dimension);
  for (int i = 0; i < vec->dimension; i++) {
    out->arr[i] = mult * vec->arr[i];
  }
  return out;
}

vector vector_add(vector a, vector b) {
  int dim;
  if (a->dimension > b->dimension) {
    dim = a->dimension;
  } else {
    dim = b->dimension;
  }
  vector out = vector_create(dim);
  for (int i = 0; i < dim; i++) {
    out->arr[i] = vector_get(a, i) + vector_get(b, i);
  }
  return out;
}

double vector_dot(vector a, vector b) {
  if (a->dimension != b->dimension) {
    return 0;
  }
  double out = 0;
  for (int i = 0; i < a->dimension; i++) {
    out += a->arr[i] * b->arr[i];
  }
  return out;
}

void vector_print(vector vec) {
  printf("( ");
  for (int i = 0; i < vec->dimension; i++) {
    printf("%f ", vec->arr[i]);
  }
  printf(")");
  return;
}