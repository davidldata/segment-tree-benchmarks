#pragma once
#include <vector>

namespace segtree {
    // Build the segment tree from the original array
    void build(std::vector<int>& orig_arr, std::vector<long long>& tree, int curr_node, int curr_left, int curr_right);

    // Update the value at a specific point in the segment tree
    void point_update(std::vector<long long>& tree, int curr_node, int curr_left, int curr_right, int target_idx, int new_value);

    // Query the sum of values in a specific range
    long long range_sum(std::vector<long long>& tree, int curr_node, int curr_left, int curr_right, int query_left, int query_right);

    // Update the values in a specific range by adding a given value
    void range_update(std::vector<long long>& tree, int curr_node, int curr_left, int curr_right,int update_left, int update_right, int value);
}
