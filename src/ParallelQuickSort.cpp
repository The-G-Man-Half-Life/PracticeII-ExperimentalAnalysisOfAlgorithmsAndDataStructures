#include "ParallelQuickSort.hpp"
#include <algorithm>
#include <omp.h>

void ParallelQuickSort::sort(std::vector<int>& data) {
    if (data.empty()) return;
    
    #pragma omp parallel
    {
        #pragma omp single
        quickSort(data, 0, (int)data.size() - 1);
    }
}

void ParallelQuickSort::quickSort(std::vector<int>& data, int low, int high) { 
    // Umbral para evitar overhead de hilos en arreglos pequeños
    if (high - low < 100000) {
        std::sort(data.begin() + low, data.begin() + high + 1);
        return;
    }

    // Partición de Hoare para evitar colapsos con números repetidos
    int pivot = data[low + (high - low) / 2];
    int i = low;
    int j = high;

    while (i <= j) {
        while (data[i] < pivot) i++;
        while (data[j] > pivot) j--;
        
        if (i <= j) {
            std::swap(data[i], data[j]);
            i++;
            j--;
        }
    }

    #pragma omp task shared(data)
    if (low < j) {
        quickSort(data, low, j);
    }
    
    #pragma omp task shared(data)
    if (i < high) {
        quickSort(data, i, high);
    }
    
    #pragma omp taskwait
}

// --- HELPER PARA LA SIMULACIÓN (Lógica secuencial para el trace) ---
void quickTrace(std::vector<int>& a, int low, int high, std::vector<std::vector<int>>& trace) {
    if (low < high) {
        // Usamos Lomuto para la simulación porque es más fácil de seguir visualmente
        int pivot = a[high];
        int i = low - 1;
        for (int j = low; j < high; j++) {
            if (a[j] <= pivot) {
                i++;
                std::swap(a[i], a[j]);
                trace.push_back(a); // GRABAR: Cada intercambio
            }
        }
        std::swap(a[i + 1], a[high]);
        trace.push_back(a); // GRABAR: El pivote en su lugar
        
        int pi = i + 1;
        quickTrace(a, low, pi - 1, trace);
        quickTrace(a, pi + 1, high, trace);
    }
}

// --- MÉTODO DE LA INTERFAZ PARA LA WEB ---
std::vector<std::vector<int>> ParallelQuickSort::getSimulationTrace(std::vector<int> data) {
    std::vector<std::vector<int>> trace;
    trace.push_back(data); // Estado inicial
    
    if (!data.empty()) {
        quickTrace(data, 0, (int)data.size() - 1, trace);
    }
    
    return trace;
}

std::string ParallelQuickSort::getName() const { return "Parallel QuickSort"; }
std::string ParallelQuickSort::getComplexity() const { return "O(n log n)"; }