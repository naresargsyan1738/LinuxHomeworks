#include <iostream>
#include <vector>
#include "Complex.h"
#include "Sorting.h"

int main() {
    std::vector<Complex> complexNum = {
        Complex(1, 3),
        Complex(2, 0),
        Complex(0, 4),
        Complex(-1, -2),
        Complex(1, -1)
    };
    for (const auto& c : complexNum) {
        std::cout << c << std::endl;
    }
    bubbleSort(complexNum);
    for (const auto& c : complexNum) {
        std::cout << c << std::endl;
    }

    return 0;
}

