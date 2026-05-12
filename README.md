# ST0245 — Practice II: Experimental Analysis of Algorithms and Data Structures
 
> **DialSort vs. Parallel Sorting Algorithms — Benchmark & Visualization Suite**  
> School of Applied Sciences and Engineering · EAFIT University · April 2026
 
---
 
## Table of Contents
 
1. [Team Members](#team-members)
2. [Project Overview](#project-overview)
3. [Algorithms Implemented](#algorithms-implemented)
4. [Project Structure](#project-structure)
5. [Prerequisites](#prerequisites)
6. [Build Instructions](#build-instructions)
7. [Execution Instructions](#execution-instructions)
8. [Benchmark Results](#benchmark-results)
9. [Algorithmic Complexity Analysis](#algorithmic-complexity-analysis)
10. [Comparative Analysis](#comparative-analysis)
11. [Web Visualization](#web-visualization)
12. [Datasets](#datasets)
13. [License](#license)
---
 
## Team Members

 Mateo Montoya Ospina
 
> **Course:** ST0245 / SI001 — Data Structures and Algorithms  
> **Lecturer:** Alexander Narváez Berrío
 
---
 
## Project Overview
 
This project experimentally analyzes and compares **DialSort** — a counting-based, linear-time sorting algorithm — against three parallel sorting alternatives implemented in **C++17** with **OpenMP**.
 
The benchmark suite measures:
 
- Mean execution time (ms)
- Standard deviation (ms)
- Throughput (elements/second)
- Peak memory usage (MB)
- Correctness verification
Datasets range from **100,000 to 10,000,000 records** across three data distributions (uniform, normal, and skewed), with a fixed universe size `U = 50,000`. All results are exported to a JSON file and rendered in an interactive web dashboard.
 
---
 
## Algorithms Implemented
 
### 1. DialSort (Sequential) — Baseline
 
A histogram-based, non-comparative sorting algorithm derived from counting sort, specialized for integer keys within a bounded universe `U`.
 
**Core idea:**
1. **Ingestion phase** — scan the input array and build a frequency histogram `H[0..U]`.
2. **Projection phase** — reconstruct the sorted output by iterating over the histogram.
**Complexity:**
 
| Case | Time | Space |
|------|------|-------|
| Best | `O(n + U)` | `O(U)` |
| Average | `O(n + U)` | `O(U)` |
| Worst | `O(n + U)` | `O(U)` |
 
> DialSort degrades when `U >> n`. A safety guard rejects inputs where `U > MAX_U`.
 
---
 
### 2. Parallel QuickSort
 
A divide-and-conquer algorithm parallelized with **OpenMP tasks**. Uses Hoare's partition scheme and falls back to `std::sort` for sub-arrays smaller than 100,000 elements to avoid thread-overhead on small partitions.
 
**Complexity:**
 
| Case | Time | Space |
|------|------|-------|
| Best | `O(n log n)` | `O(log n)` |
| Average | `O(n log n)` | `O(log n)` |
| Worst | `O(n²)` | `O(n)` |
 
---
 
### 3. Parallel BucketSort
 
Distributes elements into `n / 1000` buckets proportional to the value range, sorts each bucket independently using `std::sort` via an **OpenMP parallel for** loop, and then concatenates the results.
 
**Complexity:**
 
| Case | Time | Space |
|------|------|-------|
| Best | `O(n + k)` | `O(n + k)` |
| Average | `O(n + k)` | `O(n + k)` |
| Worst | `O(n²)` *(skewed distribution)* | `O(n + k)` |
 
> `k` = number of buckets.
 
---
 
### 4. Parallel Radix Sort
 
Sorts integers digit by digit using a stable counting sort as a subroutine, with digit-level parallelism via **OpenMP**.
 
**Complexity:**
 
| Case | Time | Space |
|------|------|-------|
| Best | `O(d · (n + b))` | `O(n + b)` |
| Average | `O(d · (n + b))` | `O(n + b)` |
| Worst | `O(d · (n + b))` | `O(n + b)` |
 
> `d` = number of digits, `b` = base (radix).
 
---
 
## Project Structure
 
```
PracticeII-ExperimentalAnalysisOfAlgorithmsAndDataStructures/
│
├── src/
│   ├── main.cpp                # Entry point: menu, benchmark suite orchestration
│   ├── DialSort.cpp            # DialSort implementation + simulation trace
│   ├── ParallelQuickSort.cpp   # Parallel QuickSort (OpenMP tasks)
│   ├── ParallelBucketSort.cpp  # Parallel BucketSort (OpenMP parallel for)
│   ├── ParallelRadixSort.cpp   # Parallel Radix Sort (OpenMP)
│   ├── DataGenerator.cpp       # Random dataset generation (uniform, normal, skewed)
│   ├── Benchmarker.cpp         # Timing, statistics, memory measurement
│   └── Visualizer.cpp          # Console-based step-by-step visualization
│
├── include/
│   ├── IAlgorithm.hpp          # Polymorphic interface for all sorting algorithms
│   ├── DialSort.hpp
│   ├── ParallelQuickSort.hpp
│   ├── ParallelBucketSort.hpp
│   ├── ParallelRadixSort.hpp
│   ├── DataGenerator.hpp
│   ├── Benchmarker.hpp
│   ├── Visualizer.hpp
│   └── ResultExporter.hpp      # JSON export utility
│
├── datasets/
│   └── GenerateDatasets.cpp    # Standalone dataset generator
│
├── web/
│   ├── index.html              # Interactive benchmark dashboard (HTML + JS)
│   └── results.json            # Pre-generated benchmark output for the web UI
│
├── build/
│   └── results.json            # Output from the latest benchmark run
│
├── CMakeLists.txt              # CMake build configuration
└── README.md
```
 
---
 
## Prerequisites
 
| Dependency | Version | Purpose |
|------------|---------|---------|
| C++ Compiler | C++17 or later | Core language standard |
| [CMake](https://cmake.org/) | ≥ 3.10 | Build system |
| [OpenMP](https://www.openmp.org/) | Any | Parallelism for QuickSort, BucketSort, and RadixSort |
| Windows SDK / PSAPI | — | Peak memory measurement (`GetProcessMemoryInfo`) |
 
> **Note:** The project currently uses `<windows.h>` and `psapi.h` for memory profiling. Compilation requires a **Windows** environment (MSVC, MinGW, or LLVM/Clang targeting Win32).
 
---
 
## Build Instructions
 
### Using CMake (recommended)
 
```bash
# 1. Clone or download the repository
git clone <repository-url>
cd PracticeII-ExperimentalAnalysisOfAlgorithmsAndDataStructures
 
# 2. Create and enter the build directory
mkdir build && cd build
 
# 3. Configure the project
cmake .. 
 
# 4. Compile
cmake --build .
```
 
After a successful build, the executable `BenchmarkSimulator` (or `BenchmarkSimulator.exe` on Windows) will be located inside the `build/` directory.
 
---
 
## Execution Instructions
 
```bash
# From the build/ directory:
./BenchmarkSimulator        # Linux/macOS
BenchmarkSimulator.exe      # Windows
```
 
### Interactive Menu
 
```
========================================================
   ST0245 - Algorithmic analysis
   DialSort vs Parallel alternatives (C++)
========================================================
1. Execute full benchmark and generate traces
2. Live Console Simulation (N=15)
3. Export results.json for dashboard web
0. Exit
========================================================
```
 
| Option | Description |
|--------|-------------|
| `1` | Runs the full benchmark suite across all algorithms, sizes, and distributions. Outputs a results table to the console and stores traces in memory. |
| `2` | Launches a step-by-step console visualization of each algorithm sorting a small array (`N = 15`). |
| `3` | Exports the in-memory benchmark results to `results.json`. **Run Option 1 first.** |
| `0` | Exits the program. |
 
---
 
## Benchmark Results
 
Results obtained on the test machine with `U = 50,000` and **3 iterations per test**. Times reported as mean (ms).
 
### Uniform Distribution
 
| N | DialSort | Parallel QuickSort | Parallel BucketSort | Parallel RadixSort |
|---|----------|--------------------|---------------------|--------------------|
| 100,000 | **3.46** | 20.07 | 5.77 | 26.90 |
| 1,000,000 | **22.43** | 63.79 | 44.89 | 183.29 |
| 10,000,000 | **215.59** | 549.41 | 489.96 | 1,780.61 |
 
### Normal Distribution
 
| N | DialSort | Parallel QuickSort | Parallel BucketSort | Parallel RadixSort |
|---|----------|--------------------|---------------------|--------------------|
| 100,000 | **3.36** | 17.09 | 5.16 | 27.70 |
| 1,000,000 | **22.51** | 61.02 | 45.56 | 183.64 |
| 10,000,000 | **215.26** | 494.45 | 475.43 | 1,802.50 |
 
### Skewed (Biased) Distribution
 
| N | DialSort | Parallel QuickSort | Parallel BucketSort | Parallel RadixSort |
|---|----------|--------------------|---------------------|--------------------|
| 100,000 | **3.30** | 17.53 | 5.33 | 28.00 |
| 1,000,000 | **22.67** | 50.94 | 44.34 | 186.72 |
| 10,000,000 | **215.44** | 476.20 | 499.23 | 1,813.03 |
 
> All times in milliseconds (ms). **Bold** = fastest for that configuration.
 
---
 
## Algorithmic Complexity Analysis
 
| Algorithm | Time (Best) | Time (Average) | Time (Worst) | Space |
|-----------|-------------|----------------|--------------|-------|
| DialSort | `O(n + U)` | `O(n + U)` | `O(n + U)` | `O(U)` |
| Parallel QuickSort | `O(n log n)` | `O(n log n)` | `O(n²)` | `O(log n)` |
| Parallel BucketSort | `O(n + k)` | `O(n + k)` | `O(n²)` | `O(n + k)` |
| Parallel Radix Sort | `O(d(n + b))` | `O(d(n + b))` | `O(d(n + b))` | `O(n + b)` |
 
**Key variables:**
- `n` — number of elements
- `U` — universe size (range of values)
- `k` — number of buckets in BucketSort
- `d` — number of digits, `b` — radix base
---
 
## Comparative Analysis
 
### Which algorithm performed best?
 
**DialSort was consistently the fastest across all tested configurations** — by a factor of 2× to 8× depending on the distribution and dataset size. Its advantage stems from its `O(n + U)` linear complexity, which avoids comparisons entirely. With `U = 50,000` fixed and `n` reaching 10,000,000, the `U` term is negligible relative to `n`, making DialSort's theoretical advantage manifest directly in wall-clock time.
 
### Why does DialSort outperform parallel alternatives?
 
The parallel algorithms incur **thread management overhead** (task spawning, synchronization, and cache invalidation) that is not offset unless the workload per thread is large enough. For the tested sizes and the fixed universe, DialSort's linear scan of a 50,000-slot histogram fits well in CPU cache, while parallel sorting algorithms distribute work across threads at the cost of coordination.
 
### When would parallel algorithms win?
 
- **Large universe sizes (`U >> n`):** DialSort's memory footprint and initialization cost grow with `U`. When `U` approaches or exceeds `n`, comparison-based alternatives become competitive.
- **Non-integer or floating-point keys:** DialSort requires bounded integer keys; parallel QuickSort and BucketSort handle arbitrary comparable types.
- **Highly parallel hardware:** With many CPU cores and very large `n`, parallel algorithms amortize their overhead more effectively.
### Memory consumption
 
DialSort allocates `O(U)` auxiliary memory (one integer per universe slot), which is independent of `n`. For `U = 50,000`, this is approximately **200 KB** — negligible. Parallel BucketSort allocates `O(n)` auxiliary space (the bucket arrays), which scales directly with input size.
 
---
 
## Web Visualization
 
An interactive dashboard is provided at `web/index.html`. It reads from `web/results.json` (or the exported `build/results.json`) and displays:
 
- Step-by-step algorithm animation for each sorting strategy on a small array (`N = 15`)
- Bar charts comparing mean execution time across algorithms, distributions, and input sizes
- Side-by-side simulation traces generated by `getSimulationTrace()`
**To use the dashboard:**
 
1. Run the benchmark (Option 1 from the menu).
2. Export the results (Option 3).
3. Copy `build/results.json` to the `web/` folder.
4. Open `web/index.html` in any modern browser.
> No server is required. The dashboard runs entirely client-side.
 
---
 
## Datasets
 
The `DataGenerator` module (`src/DataGenerator.cpp`) generates synthetic datasets on the fly using the **Mersenne Twister** (`std::mt19937`) engine seeded from hardware entropy (`std::random_device`).
 
| Distribution | Generator | Description |
|---|---|---|
| `UNIFORM` | `std::uniform_int_distribution` | Values drawn uniformly from `[0, U]` |
| `NORMAL` | `std::normal_distribution` | Gaussian bell centered at `U/2`, σ = `U/6`, clamped to `[0, U]` |
| `SKEWED` | `std::exponential_distribution` | Exponential decay; many small values, few large values |
 
Standalone dataset files can also be generated using `datasets/GenerateDatasets.cpp`.
 
---
