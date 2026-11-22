#pragma once

#include <vector>

namespace lab {

// Both algorithms sort the input in ascending order, in place.
void sequential_sort(std::vector<int>& values);
void parallel_sort(std::vector<int>& values);

}  // namespace lab
