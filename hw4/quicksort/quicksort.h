#ifndef QSORT_H
#define QSORT_H
#include <algorithm>
#include <vector>

// Returns a pivot
template<class T, class TComparator>
T& median_of_three(T& a, T& b, T& c, TComparator comparator) {
    // Assuming <
    // If b < a else a < b
    if (comparator(b, a)) {
        // If a < c
        if (comparator(a, c)) {
            return a;
            // Breaking up ties
        } else if (comparator(c, b)) {
            return b;
        } else {
            return c;
        }
    } else {
        // If b < c
        if (comparator(b, c)) {
            return b;
            // Breaking up ties
        } else if (comparator(c, a)) {
            return a;
        } else {
            return c;
        }
    }
}

// Partition algorithm
template<class T, class TComparator>
size_t partition(std::vector<T>& vec, TComparator& comparator, size_t low, size_t high) {
    size_t i = low;
    T pivot = median_of_three(vec[low], vec[low + high / 2], vec[high], comparator);

    // If the pivot is on the left
    if (pivot == vec[low]) {
        std::swap(vec[low], vec[high]);
    }
    // If the pivot is in the middle
    if (pivot == vec[low + high / 2]) {
        std::swap(vec[low + high / 2], vec[high]);
    }
    // Loop through and bubble highest number to end
    for (size_t j = low; j < high; j++) {
        if (comparator(vec[j], pivot)) {
            std::swap(vec[i], vec[j]);
            i++;
        }
    }
    std::swap(vec[i], vec[high]);
    return i;
}

// Helper function
template<class T, class TComparator>
void helper(std::vector<T>& vec, int start, int end, TComparator comparator) {
    if (start < end) {
        int loc = partition(vec, comparator, start, end);
        helper(vec, start, loc - 1, comparator);
        helper(vec, loc + 1, end, comparator);
    }
}

// Quicksort
template<class T, class TComparator>
void quicksort(std::vector<T>& vec, TComparator comparator) {
    helper(vec, 0, vec.size() - 1, comparator);
}

#endif
