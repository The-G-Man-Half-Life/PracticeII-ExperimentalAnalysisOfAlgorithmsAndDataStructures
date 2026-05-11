#pragma once
#include <vector>
#include <string>

class Visualizer {
public:
    // Main function that will execute the visualizer
    static void run();

private:
    // Auxiliar functions to clean the console
    static void printBars(const std::vector<int>& data, int highlight1 = -1, int highlight2 = -1);
    static void clearScreen();
    static void sleepMs(int ms);

    // Slowed and comprehensible versions of the algorithms
    static void visualDialSort(std::vector<int>& data);
    static void visualQuickSort(std::vector<int>& data, int low, int high);
    static int visualPartition(std::vector<int>& data, int low, int high);
    static void visualBucketSort(std::vector<int>& data);
    static void visualRadixSort(std::vector<int>& data);
};