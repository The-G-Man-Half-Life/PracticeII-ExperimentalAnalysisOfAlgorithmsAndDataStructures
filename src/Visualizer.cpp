#include "Visualizer.hpp"
#include "DataGenerator.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <algorithm>

void Visualizer::clearScreen() {
    std::cout << "\033[2J\033[1;1H"; // ANSI code to clean the screen
}

void Visualizer::sleepMs(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void Visualizer::printBars(const std::vector<int>& data, int highlight1, int highlight2) {
    clearScreen();
    std::cout << "=== SIMULATING SORTING ===\n\n";
    for (size_t i = 0; i < data.size(); ++i) {
        if (data[i] < 10) std::cout << "0";
        std::cout << data[i] << " | ";
        
        for (int j = 0; j < data[i]; ++j) {
            if ((int)i == highlight1 || (int)i == highlight2) {
                std::cout << "\033[31m█\033[0m"; // Red if it is selected
            } else {
                std::cout << "█";
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

// ---------------------------------------------------------
// 1. Visual DialSort
// ---------------------------------------------------------
void Visualizer::visualDialSort(std::vector<int>& data) {
    if (data.empty()) return;
    int U = *std::max_element(data.begin(), data.end()) + 1;
    std::vector<int> H(U, 0);

    std::cout << "Phase 1: Ingestion (Filling histogram)...\n";
    sleepMs(1000);
    for (size_t i = 0; i < data.size(); ++i) {
        H[data[i]]++;
        printBars(data, i);
        sleepMs(150);
    }

    std::cout << "Phase 2: Reconstruction (Emiting keys)...\n";
    sleepMs(1000);
    int index = 0;
    for (int k = 0; k < U; ++k) {
        while (H[k] > 0) {
            data[index] = k;
            printBars(data, index);
            sleepMs(150);
            index++;
            H[k]--;
        }
    }
}

// ---------------------------------------------------------
// 2. Visual QuickSort
// ---------------------------------------------------------
int Visualizer::visualPartition(std::vector<int>& data, int low, int high) {
    int pivot = data[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        printBars(data, j, high);
        sleepMs(150);
        if (data[j] <= pivot) {
            i++;
            std::swap(data[i], data[j]);
            printBars(data, i, j);
            sleepMs(150);
        }
    }
    std::swap(data[i + 1], data[high]);
    printBars(data, i + 1, high);
    sleepMs(150);
    return i + 1;
}

void Visualizer::visualQuickSort(std::vector<int>& data, int low, int high) {
    if (low < high) {
        int pi = visualPartition(data, low, high);
        visualQuickSort(data, low, pi - 1);
        visualQuickSort(data, pi + 1, high);
    }
}

// ---------------------------------------------------------
// 3. Visual BucketSort
// ---------------------------------------------------------
void Visualizer::visualBucketSort(std::vector<int>& data) {
    if (data.empty()) return;
    int num_buckets = 3;
    int min_val = *std::min_element(data.begin(), data.end());
    int max_val = *std::max_element(data.begin(), data.end());
    double range = (double)(max_val - min_val + 1);
    std::vector<std::vector<int>> buckets(num_buckets);

    std::cout << "Phase 1: Distribution in " << num_buckets << " Buckets...\n";
    sleepMs(1000);
    for (size_t i = 0; i < data.size(); ++i) {
        printBars(data, i);
        sleepMs(150);
        int b_idx = (int)((data[i] - min_val) / range * num_buckets);
        if (b_idx >= num_buckets) b_idx = num_buckets - 1;
        buckets[b_idx].push_back(data[i]);
    }

    std::cout << "Phase 2: Internal sorting of buckets...\n";
    sleepMs(1000);
    for (int i = 0; i < num_buckets; ++i) {
        std::sort(buckets[i].begin(), buckets[i].end());
    }

    std::cout << "Phase 3: Concat...\n";
    sleepMs(1000);
    int idx = 0;
    for (int i = 0; i < num_buckets; ++i) {
        for (int x : buckets[i]) {
            data[idx] = x;
            printBars(data, idx);
            sleepMs(150);
            idx++;
        }
    }
}

// ---------------------------------------------------------
// 4. Visual RadixSort (Base 10 to be comprehensible)
// ---------------------------------------------------------
void Visualizer::visualRadixSort(std::vector<int>& data) {
    int max_val = *std::max_element(data.begin(), data.end());
    
    for (int exp = 1; max_val / exp > 0; exp *= 10) {
        std::cout << "Phase: Ordenating by the actual digit (exp=" << exp << ")...\n";
        sleepMs(1500);
        
        std::vector<int> output(data.size());
        int count[10] = {0};

        for (size_t i = 0; i < data.size(); i++) {
            printBars(data, i); 
            sleepMs(100);
            count[(data[i] / exp) % 10]++;
        }
        for (int i = 1; i < 10; i++) count[i] += count[i - 1];
        
        for (int i = data.size() - 1; i >= 0; i--) {
            output[count[(data[i] / exp) % 10] - 1] = data[i];
            count[(data[i] / exp) % 10]--;
        }
        
        for (size_t i = 0; i < data.size(); i++) {
            data[i] = output[i];
            printBars(data, i); 
            sleepMs(100);
        }
    }
}

// ---------------------------------------------------------
// Interactive menu visualizer
// ---------------------------------------------------------
void Visualizer::run() {
    int option = -1;
    while (option != 0) {
        clearScreen();
        std::cout << "=== Visual module (N = 15) ===\n";
        std::cout << "1. DialSort\n";
        std::cout << "2. QuickSort\n";
        std::cout << "3. BucketSort\n";
        std::cout << "4. RadixSort\n";
        std::cout << "0. Return to the main menu\n";
        std::cout << "Select an algorithm to visualize: ";
        
        if (!(std::cin >> option)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }

        if (option >= 1 && option <= 4) {
            // N = 15 so it fits on the screen, Universe = 35 so the bars are not huge
            std::vector<int> data = DataGenerator::generate(15, 35, Distribution::UNIFORM);
            
            if (option == 1) visualDialSort(data);
            else if (option == 2) visualQuickSort(data, 0, data.size() - 1);
            else if (option == 3) visualBucketSort(data);
            else if (option == 4) visualRadixSort(data);
            
            std::cout << "\nSorting completed! Press enter to continue...";
            std::cin.ignore();
            std::cin.get();
        }
    }
}