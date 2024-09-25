#include "Sorting.h"

void bubbleSort(std::vector<Complex>& arr) {
    size_t n = arr.size();
    for (size_t i = 0; i < n - 1; ++i) {
        for (size_t j = 0; j < n - i - 1; ++j) {
            if (arr[j].abs() > arr[j + 1].abs()) {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}
