#include "sort.h"

#include <parlay/parallel.h>

#include <algorithm>
#include <charconv>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string_view>
#include <vector>

namespace {

using Sort = void (*)(std::vector<int>&);

double measure(Sort sort, const std::vector<int>& input,
               const std::vector<int>& expected) {
    auto values = input;  // Copying and correctness checks are not timed.
    const auto start = std::chrono::steady_clock::now();
    sort(values);
    const auto finish = std::chrono::steady_clock::now();
    if (values != expected) {
        throw std::runtime_error("Incorrect sorting result");
    }
    return std::chrono::duration<double, std::milli>(finish - start).count();
}

}  // namespace

int main(int argc, char** argv) {
    if (argc > 3) {
        throw std::runtime_error("Usage: sort_benchmark [size] [repetitions]");
    }
    const auto size = argc > 1 ? std::stoi(argv[1]) : 1000000;
    const auto repetitions = argc > 2 ? std::stoi(argv[2]) : 5;
    std::vector<int> input(size);
    std::mt19937 generator(42);
    std::uniform_int_distribution<int> distribution(-1000000, 1000000);
    for (auto& value : input) {
        value = distribution(generator);
    }
    auto expected = input;
    std::sort(expected.begin(), expected.end());

    const auto workers = parlay::num_workers();
    //warm up
    measure(lab::sequential_sort, input, expected);
    measure(lab::parallel_sort, input, expected);

    double sequential_ms = 0;
    double parallel_ms = 0;
    for (std::size_t i = 0; i < repetitions; ++i) {
        // Alternate order to reduce systematic measurement bias.
        if (i % 2 == 0) {
            sequential_ms += measure(lab::sequential_sort, input, expected);
            parallel_ms += measure(lab::parallel_sort, input, expected);
        } else {
            parallel_ms += measure(lab::parallel_sort, input, expected);
            sequential_ms += measure(lab::sequential_sort, input, expected);
        }
    }
    sequential_ms /= repetitions;
    parallel_ms /= repetitions;
    std::cout << "size=" << size << ", repetitions=" << repetitions
                << ", workers=" << workers << '\n'
                << std::fixed << std::setprecision(3)
                << "sequential mean: " << sequential_ms << " ms\n"
                << "parallel mean:   " << parallel_ms << " ms\n";
    if (parallel_ms > 0) {
        std::cout << "speedup: " << sequential_ms / parallel_ms << "x\n";
    }
}
