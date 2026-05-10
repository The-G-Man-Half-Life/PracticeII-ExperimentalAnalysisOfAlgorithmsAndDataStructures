cmake_minimum_required(VERSION 3.10)
project(PracticeII-ExperimentalAnalysisOfAlgorithmsAndDataStructures)

#Required characteristics to make the project work
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Finding and using OpenMP to use parallelism
find_package(OpenMP REQUIRED)

#include the headers to execute the project
include_directories(include)

# Adding all of the source files
add_executable(BenchmarkSimulator 
    src/main.cpp 
    src/DataGenerator.cpp 
    src/DialSort.cpp 
    src/ParallelRadixSort.cpp 
    src/ParallelBucketSort.cpp
    src/ParallelQuickSort.cpp 
)

# Linking OpenMP to the executables
target_link_libraries(BenchmarkSimulator Private OpenMP::OpenMP_CXX)