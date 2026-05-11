//Include of all required elements
#pragma once
#include "IAlgorithm.hpp"

class ParallelBucketSort : public IAlgorithm {
public:
    //Required methods for execution
    void sort(std::vector<int>& data) override;
    std::string getName() const override;
    std::string getComplexity() const override;
    std::vector<std::vector<int>> getSimulationTrace(std::vector<int> data);

};