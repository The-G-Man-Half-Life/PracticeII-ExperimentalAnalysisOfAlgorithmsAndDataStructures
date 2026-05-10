#include "DialSort.hpp"
#include <iostream>

void DialSort::sort(std::vector<int>& data) {
    const size_t n = data.size();
    if (n <= 1) return;

    int mn = data[0], mx = data[0];
    for (size_t i = 1; i < n; ++i) {
        if (data[i] < mn) mn = data[i];
        if (data[i] > mx) mx = data[i];
    }

    const uint64_t U64 = static_cast<uint64_t>(
                             static_cast<int64_t>(mx) - static_cast<int64_t>(mn)
                         ) + 1ULL;
                         
    if (U64 > MAX_U) {
        std::cerr << "[WARN] dialsort: U > MAX_U\n";
        return; 
    }
    const size_t U = static_cast<size_t>(U64);

    std::vector<int> H(U, 0);
    for (size_t i = 0; i < n; ++i) {
        H[static_cast<size_t>(data[i] - mn)]++;
    }

    size_t out = 0;
    for (size_t y = 0; y < U; ++y) {
        const int val = static_cast<int>(y) + mn;
        for (int c = H[y]; c > 0; --c) {
            data[out++] = val;
        }
    }
}

std::string DialSort::getName() const {
    return "DialSort (Secuential)";
}

std::string DialSort::getComplexity() const {
    return "O(n + U)"; 
}