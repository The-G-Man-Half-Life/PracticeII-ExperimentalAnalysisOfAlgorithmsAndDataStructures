
#pragma once
#include <vector>
#include <string>

// Type of distributions that this emulator will handle
enum class Distribution { 
    UNIFORM,    // All the numbers have the same chance (Standard case)
    NORMAL,     // Bell form (A lot of data in the middle few in the extremes)
    SKEWED      // Biased (Lots of small numbers. Few big numbers)
};

class DataGenerator {
public:
    // n = Vector size, U = Universe (Maximum possible value)
    //Generates the list having in mind n, u and a distribution
    static std::vector<int> generate(size_t n, int U, Distribution dist);
    
    // Auxiliar Method to turn enum into String (Useful for printing)
    static std::string distributionToString(Distribution dist);
};