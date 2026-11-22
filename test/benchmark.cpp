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
    const Sort std_sort = [](std::vector<int>& values) {
        std::sort(values.begin(), values.end());
    };
    //warm up
    measure(lab::sequential_sort, input, expected);
    measure(lab::parallel_sort, input, expected);
    measure(std_sort, input, expected);

    const Sort sorts[] = {lab::sequential_sort, lab::parallel_sort, std_sort};
    double times_ms[] = {0, 0, 0};
    for (std::size_t i = 0; i < repetitions; ++i) {
        // Rotate all three algorithms to reduce measurement order bias.
        for (std::size_t j = 0; j < 3; ++j) {
            const auto index = (i + j) % 3;
            times_ms[index] += measure(sorts[index], input, expected);
        }
    }
    const double sequential_ms = times_ms[0] / repetitions;
    const double parallel_ms = times_ms[1] / repetitions;
    const double std_sort_ms = times_ms[2] / repetitions;
    std::cout << "size=" << size << ", repetitions=" << repetitions
                << ", workers=" << workers << '\n'
                << std::fixed << std::setprecision(3)
                << "sequential mean: " << sequential_ms << " ms\n"
                << "parallel mean:   " << parallel_ms << " ms\n"
                << "std::sort mean:  " << std_sort_ms << " ms\n";
    if (parallel_ms > 0) {
        std::cout << "parallel speedup vs sequential: "
                  << sequential_ms / parallel_ms << "x\n"
                  << "parallel speedup vs std::sort:  "
                  << std_sort_ms / parallel_ms << "x\n";
    }
}
