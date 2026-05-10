#pragma once
#include "IAlgorithm.hpp"
#include <vector>
#include <string>
#include <cstdint>

class DialSort : public IAlgorithm {
public:
    void sort(std::vector<int>& data) override;

    std::string getName() const override;
    std::string getComplexity() const override;

private:
    static constexpr uint64_t MAX_U = 10000000ULL; 
};