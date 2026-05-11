//include of required elements
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <thread>
#include <chrono>

// Including the required headers
#include "DataGenerator.hpp"
#include "Benchmarker.hpp"
#include "DialSort.hpp"
#include "ParallelQuickSort.hpp"
#include "ParallelBucketSort.hpp"
#include "ParallelRadixSort.hpp"
#include "Visualizer.hpp"
#include "ResultExporter.hpp"
#include <windows.h>

//global variable
std::string global_json_results = "{}";

// Auxiliar function to clean the table and print information
void printTableHeader() {
    std::cout << std::left 
              << std::setw(25) << "Algorithm" 
              << std::setw(15) << "Distribution" 
              << std::setw(12) << "Data" 
              << std::setw(12) << "Universe" 
              << std::setw(15) << "Media(ms)" 
              << std::setw(15) << "SD(ms)" 
              << std::setw(15) << "Ops/seg" 
              << std::setw(12) << "Mem (MB)"
              << "OK?\n";
    std::cout << std::string(125, '-') << "\n";
}

// Function to execute the tests
void runBenchmarkSuite() {
    std::cout << "\n=== Executing BENCHMARK SUITE ===\n";
    std::cout << "Generating Data and calculating statistics (3 iterations per test)...\n\n";

    printTableHeader();

    // 1. Instantiating the algorithms
    DialSort dialSort;
    ParallelQuickSort parallelQuickSort;
    ParallelBucketSort parallelBucketSort;
    ParallelRadixSort parallelRadixSort; 

    // Using pointers for polymorphism
    std::vector<IAlgorithm*> algorithms = {&dialSort, &parallelQuickSort, &parallelBucketSort, &parallelRadixSort};

    // 2. Defining the sizes (Exigido: 100k, 1M, 10M)
    std::vector<size_t> test_sizes = {100000, 1000000, 10000000};
    int U = 50000; 
    int iteraciones = 3; 

    std::vector<Distribution> dists = {Distribution::UNIFORM, Distribution::NORMAL, Distribution::SKEWED};

    // Variable to build the JSON string
    std::string json = "{\n";

    // 3. SECCIÓN 1: BENCHMARKS DE RENDIMIENTO
    for (size_t d = 0; d < dists.size(); ++d) {
        Distribution dist = dists[d];
        std::string dist_name = DataGenerator::distributionToString(dist);
        
        json += "  \"" + dist_name + "\": [\n";

        for (size_t i = 0; i < test_sizes.size(); ++i) {
            size_t n = test_sizes[i];
            std::vector<int> data = DataGenerator::generate(n, U, dist);
            
            json += "    {\n      \"N\": " + std::to_string(n) + ",\n";
            
            for (size_t j = 0; j < algorithms.size(); ++j) {
                IAlgorithm* algorithm = algorithms[j];
                BenchmarkResult result = Benchmarker::run(*algorithm, data, iteraciones);

                // Print to console
                std::cout << std::left 
                          << std::setw(25) << algorithm->getName()
                          << std::setw(15) << dist_name
                          << std::setw(12) << n
                          << std::setw(12) << U
                          << std::fixed << std::setprecision(3)
                          << std::setw(15) << result.mean_time_ms
                          << std::setw(15) << result.std_dev_ms
                          << std::setw(15) << static_cast<long long>(result.throughput) 
                          << std::setw(12) << (result.peak_memory_bytes / 1024.0 / 1024.0)
                          << (result.is_sorted ? "Yes" : "No") << "\n" << std::flush;

                // Exportación detallada al JSON
                json += "      \"" + algorithm->getName() + "\": {";
                json += "\"mean\": " + std::to_string(result.mean_time_ms) + ", ";
                json += "\"sd\": " + std::to_string(result.std_dev_ms) + ", ";
                json += "\"throughput\": " + std::to_string(result.throughput) + ", ";
                json += "\"memory\": " + std::to_string(result.peak_memory_bytes / 1024.0 / 1024.0);
                json += "}";
                
                if (j < algorithms.size() - 1) json += ",\n";
                else json += "\n";
            }
            json += "    }";
            if (i < test_sizes.size() - 1) json += ",\n";
            else json += "\n";

            std::cout << std::string(125, '-') << "\n";
        }
        json += "  ],\n"; // Nota la coma, porque sigue la siguiente distribución o la simulación
    }

    // 4. SECCIÓN 2: SIMULACIONES (TRACES) PARA LA WEB
    std::cout << "\nGenerating Algorithm Traces for Web Simulation...\n";
    json += "  \"simulations\": {\n";
    
    for (size_t i = 0; i < algorithms.size(); ++i) {
        // Generamos una muestra pequeña de 15 datos para que la web la reproduzca
        auto sample_data = DataGenerator::generate(15, 30, Distribution::UNIFORM);
        auto trace = algorithms[i]->getSimulationTrace(sample_data);

        json += "    \"" + algorithms[i]->getName() + "\": [\n";
        for (size_t h = 0; h < trace.size(); ++h) {
            json += "      [";
            for (size_t v = 0; v < trace[h].size(); ++v) {
                json += std::to_string(trace[h][v]) + (v < trace[h].size() - 1 ? "," : "");
            }
            json += (h < trace.size() - 1 ? "],\n" : "]\n");
        }
        json += (i < algorithms.size() - 1 ? "    ],\n" : "    ]\n");
    }
    json += "  }\n";
    json += "}\n";

    global_json_results = json;

    std::cout << "¡Benchmark and Traces Completed! Ready to export.\n";
    std::cout << "Press ENTER to continue...";
    std::cin.ignore();
    std::cin.get();
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    int option = -1;

    while (option != 0) {
        std::cout << "\033[2J\033[1;1H"; 

        std::cout << "========================================================\n";
        std::cout << "   ST0245 - Algorithmic analysis\n";
        std::cout << "   DialSort vs Parallel alternatives (C++)\n";
        std::cout << "========================================================\n";
        std::cout << "1. Execute full benchmark and generate traces\n";
        std::cout << "2. Live Console Simulation (N=15)\n";
        std::cout << "3. Export results.json for dashboard web\n"; 
        std::cout << "0. Exit\n";
        std::cout << "========================================================\n";
        std::cout << "Select an option: ";
        
        if (!(std::cin >> option)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }

        switch (option) {
            case 1:
                runBenchmarkSuite();
                break;
            case 2:
                Visualizer::run();
                break;
            case 3:
                if (global_json_results == "{}") {
                    std::cout << "\n[!] Error: No data available. Run Option 1 first.\n";
                } else {
                    ResultExporter::saveJSON(global_json_results);
                }
                std::this_thread::sleep_for(std::chrono::seconds(2)); 
                break;
            case 0:
                std::cout << "\nExiting simulator.\n";
                break;
            default:
                std::cout << "\nNo valid option.\n";
                std::this_thread::sleep_for(std::chrono::seconds(1)); 
                break;
        }
    }

    return 0;
}