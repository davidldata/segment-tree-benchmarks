#pragma once
#include <vector>

namespace lazytree {
    // Build segment tree with lazy propagation from the original array
    void build(std::vector<int>& orig_arr, std::vector<int>& tree, int curr_node, int curr_left, int curr_right);

    // Update the values in a specific range in the segment tree with lazy propagation
    void range_update(std::vector<int>& tree, std::vector<int>& lazy, int curr_node, int curr_left, int curr_right, int update_left, int update_right, int new_value);

    // Query the sum of values in a specific range with lazy propagation
    long long range_sum(std::vector<int>& tree, std::vector<int>& lazy, int curr_node, int curr_left, int curr_right, int query_left, int query_right);

    // Propagate the lazy updates to the children of the current node
    void propagate(std::vector<int>& tree, std::vector<int>& lazy, int curr_node, int curr_left, int curr_right);

    // Update the value of a single element in the segment tree with lazy propagation
    void point_update(std::vector<int>& tree, std::vector<int>& lazy, int curr_node, int curr_left, int curr_right, int index, int new_value);
}