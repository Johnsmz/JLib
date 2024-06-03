#include <stdlib.h>

int *cloneintarray(int *arr, int len) {
  int *out = (int*)malloc(sizeof(int) * len);
  for (int i = 0; i < len; i++) {
    out[i] = arr[i];
  }
  return out;
}

int *subintarr(int *arr, int start, int end) {
  int *out = (int*)malloc(sizeof(int) * (end - start));
  for (int i = 0; i < end - start; i++) {
    out[i] = arr[start + i];
  }
  return out;
}

void merge_sort(int *arr, int len) {
  if (len == 1) {
    return;
  }
  int mid = len / 2, *left = subintarr(arr, 0, mid), *right = subintarr(arr, mid, len), l = 0, r = 0;
  merge_sort(left, mid);
  merge_sort(right, len - mid);
  while (l + r < len) {
    if (l == mid) {
      arr[l + r] = right[r];
      r++;
    } else if (r == len - mid) {
      arr[l + r] = left[l];
      l++;
    } else {
      if (left[l] <= right[r]) {
        arr[l + r] = left[l];
        l++;
      } else {
        arr[l + r] = right[r];
        r++;
      }
    }
  }
  free(left);
  free(right);
}

int binary_search(int *arr, int from, int to, int target) {
  if (to < from) {
    return -1;
  }
  int mid = (from + to) / 2;
  if (arr[mid] == target) {
    return mid;
  }
  if (arr[mid] > target) {
    return binary_search_with_m1(arr, from, mid - 1, target);
  } else {
    return binary_search_with_m1(arr, mid + 1, to, target);
  }
}

int binary_search_insert(int *arr, int from, int to, int target) {
  if (to < from) {
    return from;
  }
  int mid = (from + to) / 2;
  if (arr[mid] == target) {
    return mid;
  }
  if (arr[mid] > target) {
    return binary_search(arr, from, mid - 1, target);
  } else {
    return binary_search(arr, mid + 1, to, target);
  }
}
