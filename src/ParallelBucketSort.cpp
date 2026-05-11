//Include of all the required elements
#include "ParallelBucketSort.hpp"
#include <algorithm>
#include <vector>
#include <omp.h>

void ParallelBucketSort::sort(std::vector<int>& data) {
    if (data.empty()) return;

    int n = (int)data.size();
    int num_buckets = n / 1000; // Empirical adjust of 1000 elements by bucket
    if (num_buckets < 1) num_buckets = 1;

    int min_val = *std::min_element(data.begin(), data.end());
    int max_val = *std::max_element(data.begin(), data.end());
    double range = (double)(max_val - min_val + 1);

    std::vector<std::vector<int>> buckets(num_buckets);

    // 1. Distribution (Ingestion)
    for (int x : data) {
        int b_idx = (int)((x - min_val) / range * num_buckets);
        if (b_idx >= num_buckets) b_idx = num_buckets - 1; // Seguridad
        buckets[b_idx].push_back(x);
    }

    // 2. Parallel organization by bucket
    #pragma omp parallel for
    for (int i = 0; i < num_buckets; ++i) {
        if (!buckets[i].empty()) {
            std::sort(buckets[i].begin(), buckets[i].end());
        }
    }

    // 3. Concatenation of all the elements
    int idx = 0;
    for (int i = 0; i < num_buckets; ++i) {
        for (int x : buckets[i]) {
            data[idx++] = x;
        }
    }
}

// --- NUEVA FUNCIÓN PARA LA SIMULACIÓN ---
std::vector<std::vector<int>> ParallelBucketSort::getSimulationTrace(std::vector<int> data) {
    std::vector<std::vector<int>> trace;
    trace.push_back(data); // Estado inicial

    if (data.empty()) return trace;

    int n = (int)data.size();
    int num_buckets = 3; // Usamos pocos buckets para que sea visual en el DashBoard

    int min_val = *std::min_element(data.begin(), data.end());
    int max_val = *std::max_element(data.begin(), data.end());
    double range = (double)(max_val - min_val + 1);

    std::vector<std::vector<int>> buckets(num_buckets);

    // Fase 1: Distribución a buckets
    for (int x : data) {
        int b_idx = (int)((x - min_val) / range * num_buckets);
        if (b_idx >= num_buckets) b_idx = num_buckets - 1;
        buckets[b_idx].push_back(x);
    }

    // Fase 2: Ordenar cada bucket individualmente
    for (int i = 0; i < num_buckets; ++i) {
        std::sort(buckets[i].begin(), buckets[i].end());
    }

    // Fase 3: Concatenación (Aquí es donde grabamos los pasos del arreglo final)
    int idx = 0;
    for (int i = 0; i < num_buckets; ++i) {
        for (int x : buckets[i]) {
            data[idx++] = x;
            trace.push_back(data); // GRABAR: Se ve cómo el arreglo se llena ordenadamente
        }
    }

    return trace;
}

std::string ParallelBucketSort::getName() const { return "Parallel BucketSort"; }
std::string ParallelBucketSort::getComplexity() const { return "O(n + k)"; }