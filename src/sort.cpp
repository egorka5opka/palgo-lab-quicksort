#include "sort.h"

#include <algorithm>
#include <parlay/parallel.h>
#include <parlay/primitives.h>

namespace lab {

static thread_local std::mt19937_64 rnd(123);

void qsort_seq(std::vector<int>& a, size_t l, size_t r) {
    if (r - l <= 1) {
        return;
    }
    int pivot = a[rnd() % (r - l) + l];
    int lt = l, eq = l, gt = r;
    while (eq < gt) {
        if (a[eq] < pivot) {
            std::swap(a[eq], a[lt]);
            ++eq;
            ++lt;
        } else if (a[eq] > pivot) {
            --gt;
            std::swap(a[eq], a[gt]);
        } else {
            ++eq;
        }
    }
    qsort_seq(a, l, lt);
    qsort_seq(a, gt, r);
}

void sequential_sort(std::vector<int>& values) {
    qsort_seq(values, 0, values.size());
}


void qsort_par(std::vector<int>& a, size_t l, size_t r) {
    if (r - l <= 1) {
        qsort_seq(a, l, r);
        return;
    }
    int pivot = a[rnd() % (r - l) + l];
    int lt = l, eq = l, gt = r;
    while (eq < gt) {
        if (a[eq] < pivot) {
            std::swap(a[eq], a[lt]);
            ++eq;
            ++lt;
        } else if (a[eq] > pivot) {
            --gt;
            std::swap(a[eq], a[gt]);
        } else {
            ++eq;
        }
    }
    parlay::par_do (
        [&](){qsort_par(a, l, lt);},
        [&](){qsort_par(a, gt, r);}
    );
}

void parallel_sort(std::vector<int>& values) {
    qsort_par(values, 0, values.size());
}

}  // namespace lab
