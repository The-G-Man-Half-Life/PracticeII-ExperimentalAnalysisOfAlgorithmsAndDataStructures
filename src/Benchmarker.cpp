//include of all the required elements
#include "Benchmarker.hpp"
#include <iostream>
#include <cmath>

BenchmarkResult Benchmarker::run(IAlgorithm& algorithm, const std::vector<int>& base_data, int iterations) {
    std::vector<double> times;
    bool all_sorted_correctly = true;

    // Execute the iterations required
    for (int i = 0; i < iterations; ++i) {
        // 1. Create a fresh copy so it doesn't sort an already sorted list
        std::vector<int> data_copy = base_data;

        // 2. Start stop watch of high level
        auto start = std::chrono::high_resolution_clock::now();
        
        // 3. Execute the algorithm
        algorithm.sort(data_copy);
        
        // 4. Stop the stop watch
        auto end = std::chrono::high_resolution_clock::now();

        // 5. Calculate time in miliseconds
        std::chrono::duration<double, std::milli> duration = end - start;
        times.push_back(duration.count());

        // 6. Check if the algorithm di its work properly
        if (!checkSorted(data_copy)) {
            all_sorted_correctly = false;
        }
    }

    // --- Calculate statistics metrics ---
    
    // Mean
    double sum = 0;
    for (double t : times) {
        sum += t;
    }
    double mean = sum / iterations;

    // Standard Deviation
    double variance_sum = 0;
    for (double t : times) {
        variance_sum += std::pow(t - mean, 2);
    }
    double std_dev = std::sqrt(variance_sum / iterations);

    // Throughput (Elements by the second)
    // Mean in miliseconds turned to seconds
    double mean_sec = mean / 1000.0;
    double throughput = 0;
    if (mean_sec > 0) {
        throughput = base_data.size() / mean_sec;
    }

    // Retornar la estructura con todos los datos listos para el reporte
    return {mean, std_dev, throughput, all_sorted_correctly};
}

bool Benchmarker::checkSorted(const std::vector<int>& data) {
    if (data.empty()) return true;
    for (size_t i = 1; i < data.size(); ++i) {
        if (data[i - 1] > data[i]) {
            return false; // Se encontró un elemento fuera de orden
        }
    }
    return true;
}