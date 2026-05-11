//include of all the required elements
#include "Benchmarker.hpp"
#include <iostream>
#include <cmath>
#include <chrono>
#include <windows.h>
#include <psapi.h>

//Method to obtain memory usage
size_t getPeakRSS(){
    PROCESS_MEMORY_COUNTERS info;
    GetProcessMemoryInfo(GetCurrentProcess(), &info, sizeof(info));
    return info.PeakWorkingSetSize; //Max memory used in bytes
}

BenchmarkResult Benchmarker::run(IAlgorithm& algorithm, const std::vector<int>& base_data, int iterations) {
    std::vector<double> times;
    size_t peak_mem = 0;
    bool all_sorted_correctly = true;

    // Execute the iterations required
    for (int i = 0; i < iterations; ++i) {
        // 1. Create a fresh copy so it doesn't sort an already sorted list
        std::vector<int> data_copy = base_data;
        
        // Measure memory before
        size_t mem_start = getPeakRSS();

        // 2. Start stop watch of high level
        auto start = std::chrono::high_resolution_clock::now();
        
        // 3. Execute the algorithm
        algorithm.sort(data_copy);
        
        // 4. Stop the stop watch
        auto end = std::chrono::high_resolution_clock::now();

        // Measure memory after and calculate peak
        size_t mem_end = getPeakRSS();
        if((mem_end - mem_start) > peak_mem) {
            peak_mem = mem_end - mem_start;
        }

        // 5. Calculate time in miliseconds
        std::chrono::duration<double, std::milli> duration = end - start;
        times.push_back(duration.count());

        // 6. Check if the algorithm did its work properly
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

    // Return the data structure with everything ready to present
    return {mean, std_dev, throughput, peak_mem, all_sorted_correctly};
}

bool Benchmarker::checkSorted(const std::vector<int>& data) {
    if (data.empty()) return true;
    for (size_t i = 1; i < data.size(); ++i) {
        if (data[i - 1] > data[i]) {
            return false; // Return if an element is out of order
        }
    }
    return true;
}