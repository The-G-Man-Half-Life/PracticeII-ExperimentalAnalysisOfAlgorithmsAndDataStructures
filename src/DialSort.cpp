#include "DialSort.hpp"
#include <iostream>
#include <algorithm>

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

// --- NUEVA FUNCIÓN PARA LA SIMULACIÓN ---
std::vector<std::vector<int>> DialSort::getSimulationTrace(std::vector<int> data) {
    std::vector<std::vector<int>> trace;
    trace.push_back(data); // Guardar estado inicial

    const size_t n = data.size();
    if (n <= 1) return trace;

    // Lógica para encontrar el universo (U) y mínimo (mn)
    int mn = data[0], mx = data[0];
    for (size_t i = 1; i < n; ++i) {
        if (data[i] < mn) mn = data[i];
        if (data[i] > mx) mx = data[i];
    }
    const size_t U = static_cast<size_t>(mx - mn + 1);

    // Fase 1: Ingestión (Histograma)
    std::vector<int> H(U, 0);
    for (size_t i = 0; i < n; ++i) {
        H[static_cast<size_t>(data[i] - mn)]++;
    }

    // Fase 2: Proyección (Aquí es donde el arreglo cambia visualmente)
    size_t out = 0;
    for (size_t y = 0; y < U; ++y) {
        const int val = static_cast<int>(y) + mn;
        while (H[y] > 0) {
            data[out++] = val;
            trace.push_back(data); // GRABAR: Guardamos cómo va quedando el arreglo
            H[y]--;
        }
    }
    return trace;
}

std::string DialSort::getName() const {
    return "DialSort (Sequential)";
}

std::string DialSort::getComplexity() const {
    return "O(n + U)"; 
}