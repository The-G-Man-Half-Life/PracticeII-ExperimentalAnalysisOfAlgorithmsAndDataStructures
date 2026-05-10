//Includes for the required elements
#include "DataGenerator.hpp"
#include <random>
#include <algorithm>
#include <cmath>

/*Functions that takes care of generating the dataset 
Having in mind n the universe and its distribution*/
std::vector<int> DataGenerator::generate(size_t n, int U, Distribution dist) {
    //Dataset in which the data will be placed
    std::vector<int> data(n);
    
    // Configuring the generation of random numbers (Mersenne Twister)
    std::random_device rd;  // Hardware Seed
    std::mt19937 gen(rd()); // Generator

    //Generator of the dataset having in mind the wished distribution
    switch (dist) {
        //Uniform distribution
        case Distribution::UNIFORM: {
            // Unifoorm distribution between 0 and the value of U
            std::uniform_int_distribution<> d(0, U);
            for(size_t i = 0; i < n; ++i) {
                data[i] = d(gen);
            }
            break;
        }

        //Uniform distribution
        case Distribution::NORMAL: {
            // This will take care of generating values as those of a Gauss bell
            std::normal_distribution<> d(U / 2.0, U / 6.0);
            for(size_t i = 0; i < n; ++i) {
                int val = static_cast<int>(std::round(d(gen)));
                // Makes sure no value goes out of the wished range
                data[i] = std::clamp(val, 0, U); 
            }
            break;
        }

        //Skewed distribution
        case Distribution::SKEWED: {
            // Exponential distribution generates a lot of small numbers and few big ones
            std::exponential_distribution<> d(3.0 / U); 
            for(size_t i = 0; i < n; ++i) {
                int val = static_cast<int>(std::round(d(gen)));
                // Makes sure no value goes out of the wished range
                data[i] = std::clamp(val, 0, U);
            }
            break;
        }
    }

    return data;
}

//Takes care of indicating the Distribution used for the dataset
std::string DataGenerator::distributionToString(Distribution dist) {
    switch (dist) {
        case Distribution::UNIFORM: return "Uniform";
        case Distribution::NORMAL: return "Normal";
        case Distribution::SKEWED: return "Biased";
        default: return "Unknown";
    }
}