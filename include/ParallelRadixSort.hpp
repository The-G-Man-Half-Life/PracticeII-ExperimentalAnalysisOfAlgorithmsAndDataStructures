//All of the required includes
#pragma once
#include "IAlgorithm.hpp"
#include <vector>
#include <string>

class ParallelRadixSort : public IAlgorithm {
public:
    // Constructor
    ParallelRadixSort() = default;

    // Interfeca implementation
    void sort(std::vector<int>& data) override;

    // Methods
    std::string getName() const override;
    std::string getComplexity() const override;
    std::vector<std::vector<int>> getSimulationTrace(std::vector<int> data);

};