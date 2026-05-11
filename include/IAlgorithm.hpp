//Includes for required elements
#pragma once
#include <vector>
#include <string>

// Interface as an abstract class
class IAlgorithm {
public:
    // Virtual destroyer for good practices in C++
    virtual ~IAlgorithm() = default;

    // Main method that receives the vector by reference to order it in order 
    virtual void sort(std::vector<int>& data) = 0;

    //Catching the steps for simulation
    virtual std::vector<std::vector<int>> getSimulationTrace(std::vector<int> data) = 0;

    // Required elements to show them in the web
    virtual std::string getName() const = 0;
    virtual std::string getComplexity() const = 0; 
};