//include of the required elements
#pragma once
#include "IAlgorithm.hpp"
#include <vector>
#include <chrono>

// Structure to save the required metrics
struct BenchmarkResult {
    double mean_time_ms;     // average time in miliseconds
    double std_dev_ms;       // Standard Deviation in miliseconds
    double throughput;       // Processed elements by the second (Keys/s)
    size_t peak_memory_bytes; //will calculate memory in bytes
    bool is_sorted;          // Verifying if array is sorted
};

class Benchmarker {
public:
    // parameters: algorithm to try, data base and number of iterations
    static BenchmarkResult run(IAlgorithm& algorithm, const std::vector<int>& base_data, int iterations = 5);

private:
    // Auxiliar algorithm to check if everything is really sorted
    static bool checkSorted(const std::vector<int>& data);
};