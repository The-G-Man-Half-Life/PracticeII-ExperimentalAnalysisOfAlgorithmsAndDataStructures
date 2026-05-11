//Including required elements
#pragma once
#include "IAlgorithm.hpp"

class ParallelQuickSort : public IAlgorithm {
public:
    //Mehtods for the algorithm execution that need to be seen
    void sort(std::vector<int>& data) override;
    std::string getName() const override;
    std::string getComplexity() const override;
    std::vector<std::vector<int>> getSimulationTrace(std::vector<int> data);


private:
    //Methods to execute the algoritm behind curtains
    void quickSort(std::vector<int>& data, int low, int high);
    int partition(std::vector<int>& data, int low, int high);
};