#include "sort.h"

#include <algorithm>
#include <iostream>
#include <limits>
#include <random>
#include <stdexcept>
#include <vector>

namespace {

void check(const std::vector<int>& input) {
    auto expected = input;
    std::sort(expected.begin(), expected.end());

    auto sequential = input;
    lab::sequential_sort(sequential);
    if (sequential != expected) {
        throw std::runtime_error("sequential_sort differs from std::sort");
    }

    auto parallel = input;
    lab::parallel_sort(parallel);
    if (parallel != expected) {
        throw std::runtime_error("parallel_sort differs from std::sort");
    }
}

}  // namespace

int main() {
    check({});
    check({42});
    check({2, 1});
    check({1, 2, 3, 4, 5});
    check({5, 4, 3, 2, 1});
    check({3, -1, 3, 0, -1, 2});
    check({std::numeric_limits<int>::min(), 0,
            std::numeric_limits<int>::max()});
    check(std::vector<int>(10000, 0));

    std::mt19937 generator(42);
    std::uniform_int_distribution<int> distribution(-1000, 1000);
    for (const auto size : {10, 100, 1000, 10000, 100000}) {
        std::vector<int> values(size);
        for (auto& value : values) {
            value = distribution(generator);
        }
        check(values);
        std::sort(values.begin(), values.end());
        check(values);
        std::reverse(values.begin(), values.end());
        check(values);
    }
    std::cout << "All correctness checks passed\n";
}
