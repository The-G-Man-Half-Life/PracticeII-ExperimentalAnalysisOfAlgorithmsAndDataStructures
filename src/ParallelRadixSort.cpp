//Include for all the required elements
#include "ParallelRadixSort.hpp"
#include <vector>
#include <algorithm>
#include <omp.h>
#include <atomic>

void ParallelRadixSort::sort(std::vector<int>& data) {
    if (data.size() < 2) return;

    size_t n = data.size();
    std::vector<int> buffer(n);
    int* src = data.data();
    int* dst = buffer.data();

    // Radix sort of 32 bits (4 passes of 8 bits)
    for (int shift = 0; shift < 32; shift += 8) {
        size_t count[256] = {0};

        // 1. Parallel counting phase
        #pragma omp parallel
        {
            size_t local_count[256] = {0};
            #pragma omp for nowait
            for (size_t i = 0; i < n; i++) {
                local_count[(src[i] >> shift) & 0xFF]++;
            }
            #pragma omp critical
            {
                for (int i = 0; i < 256; i++) count[i] += local_count[i];
            }
        }

        // 2. sum of prefixes and usage of atomic to make it safely
        std::vector<std::atomic<size_t>> pref(256);
        size_t current_pref = 0;
        for (int i = 0; i < 256; i++) {
            pref[i].store(current_pref);
            current_pref += count[i];
        }

        // 3. Parallel distribution in the buffer
        #pragma omp parallel for
        for (size_t i = 0; i < n; i++) {
            int digit = (src[i] >> shift) & 0xFF;
            size_t pos = pref[digit].fetch_add(1, std::memory_order_relaxed);
            dst[pos] = src[i];
        }

        std::swap(src, dst);
    }
    
    if (src != data.data()) {
        std::copy(buffer.begin(), buffer.end(), data.begin());
    }
}

// --- NUEVA FUNCIÓN PARA LA SIMULACIÓN ---
std::vector<std::vector<int>> ParallelRadixSort::getSimulationTrace(std::vector<int> data) {
    std::vector<std::vector<int>> trace;
    trace.push_back(data); // Estado inicial

    if (data.empty()) return trace;

    int max_val = *std::max_element(data.begin(), data.end());

    // Usamos Base 10 para que la animación web sea fácil de entender (Unidades, Decenas...)
    for (int exp = 1; max_val / exp > 0; exp *= 10) {
        std::vector<int> output(data.size());
        int count[10] = {0};

        // Contar ocurrencias de los dígitos
        for (int i = 0; i < (int)data.size(); i++) {
            count[(data[i] / exp) % 10]++;
        }

        // Cambio de conteo a posiciones reales
        for (int i = 1; i < 10; i++) {
            count[i] += count[i - 1];
        }

        // Construir el arreglo de salida (Fase de distribución)
        for (int i = (int)data.size() - 1; i >= 0; i--) {
            int digit = (data[i] / exp) % 10;
            output[count[digit] - 1] = data[i];
            count[digit]--;
            
            // GRABAR: Guardamos el estado parcial para ver cómo "vuelan" los números a su sitio
            trace.push_back(output); 
        }

        // Copiar el resultado al arreglo original para la siguiente potencia de 10
        for (int i = 0; i < (int)data.size(); i++) {
            data[i] = output[i];
        }
    }

    return trace;
}

std::string ParallelRadixSort::getName() const { 
    return "Parallel Radix Sort"; 
}

std::string ParallelRadixSort::getComplexity() const { 
    return "O(d * (n/p))"; 
}