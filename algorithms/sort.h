#include <stdlib.h>

void sort(int *arr, int len) {
    for (int j = 1; j < len; j++) {
        int key = arr[j], i = j - 1;
        while (i >= 0 && arr[i] > key) {
            arr[i + 1] = arr[i];
            i--;
        }
        arr[i + 1] = key;
    }
}

int quick_partition(int *arr, int start, int end) {
    int i = start, j = end, x = arr[start];
    while (i < j) {
        while (i < j && arr[j] >= x) {
            j--;
        }
        if (i < j) {
            arr[i++] = arr[j];
        }

        while (i < j && arr[i] < x) {
            i++;
        }
        if (i < j) {
            arr[j--] = arr[i];
        }
    }
    arr[i] = x;
    return i;
}

void quick_sort(int *arr, int start, int end) {
    if (start < end) {
        int i = quick_partition(arr, start, end);
        quick_sort(arr, start, i - 1);
        quick_sort(arr, i + 1, end);
    }
}

int rand_select(int *arr, int start, int end, int rank) {
    if (!rank) {
        return 0;
    }
    double rrr = (double)rand() / RAND_MAX;
    int p = (int)(rrr * (end - start + 1)) + start;
    int *endd = &arr[end], *pp = &arr[p], *tmp = endd;
    endd = pp;
    pp = tmp;
    if (start == end) {
        return arr[start];
    }
    p = quick_partition(arr, start, end);
    int M = p - start + 1;
    if (rank == M)
        return arr[p];
    if (rank < M) {
        return rand_select(arr, start, p - 1, rank);
    }
    return rand_select(arr, p + 1, end, rank - M);
}