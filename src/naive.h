#pragma once
#include <vector>

namespace naive {
    // Update the value at a specific point in the array
    void point_update(std::vector<int>& arr, int index, int value);

    // Query the sum of a range [left, right] from the array
    long long range_sum(std::vector<int>& arr, int left, int right);

    // Perform a range update by adding 'value' to all elements in the range [left, right]
    void range_update(std::vector<int>& arr, int left, int right, int value);
}