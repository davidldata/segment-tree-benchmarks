#include "segment_tree.h"
#include <iostream>
#include <vector>

using namespace std;

namespace segtree {
// Build the segment tree from the original array
void build(vector<int>& orig_arr, vector<long long>& tree, int curr_node, int curr_left, int curr_right) {
    // Base case: if the current segment has only one element
    if (curr_left == curr_right) {
        tree[curr_node] = orig_arr[curr_left];
        return;
    }
    // Recursively build the left and right subtrees
    int mid = curr_left + (curr_right - curr_left) / 2;
    build(orig_arr, tree, 2 * curr_node + 1, curr_left, mid);
    build(orig_arr, tree, 2 * curr_node + 2, mid + 1, curr_right);
    tree[curr_node] = tree[2 * curr_node + 1] + tree[2 * curr_node + 2];
}

// Update the value at a specific point in the segment tree
void point_update(vector<long long>& tree, int curr_node, int curr_left, int curr_right, int target_idx, int new_value) {
    // Base case: if the current segment has only one element
    if (curr_left == curr_right) {
        tree[curr_node] = new_value;
        return;
    }
    // Recursively update the left or right subtree based on the target index
    int mid = curr_left + (curr_right - curr_left) / 2;
    if (target_idx <= mid) {
        point_update(tree, 2 * curr_node + 1, curr_left, mid, target_idx, new_value);
    } else {
        point_update(tree, 2 * curr_node + 2, mid + 1, curr_right, target_idx, new_value);
    }
    // After updating the child, update the current node's value
    tree[curr_node] = tree[2 * curr_node + 1] + tree[2 * curr_node + 2];
}

// Query the sum of values in a specific range
long long range_sum(vector<long long>& tree, int curr_node, int curr_left, int curr_right, int query_left, int query_right) {
    // If the current segment is completely outside the query range
    if (query_left > curr_right || query_right < curr_left) {
        return 0; // No overlap
    }
    // If the current segment is completely inside the query range
    if (query_left <= curr_left && curr_right <= query_right) {
        return tree[curr_node]; // Total overlap
    }
    // If the current segment is partially inside and partially outside the query range
    int mid = curr_left + (curr_right - curr_left) / 2;
    int left_sum = range_sum(tree, 2 * curr_node + 1, curr_left, mid, query_left, query_right);
    int right_sum = range_sum(tree, 2 * curr_node + 2, mid + 1, curr_right, query_left, query_right);
    return left_sum + right_sum; // Partial overlap
}

void range_update(vector<long long>& tree, int curr_node, int curr_left, int curr_right, int update_left, int update_right, int value) {
    // No overlap
    if (update_left > curr_right || update_right < curr_left) {
        return;
    }
    // Leaf node
    if (curr_left == curr_right) {
        tree[curr_node] += value;
        return;
    }
    // Partial or full overlap: recurse to children
    int mid = curr_left + (curr_right - curr_left) / 2;
    range_update(tree, 2 * curr_node + 1, curr_left, mid, update_left, update_right, value);
    range_update(tree, 2 * curr_node + 2, mid + 1, curr_right, update_left, update_right, value);
    // Recompute this node from children
    tree[curr_node] = tree[2 * curr_node + 1] + tree[2 * curr_node + 2];
}
}

void segtree_test() {
    using namespace segtree;
    // Create a testbed with 1000 elements initialized to 1    
    int n = 1000;
    vector<int> testbed(n, 1);

    // Test each of the functions and print the results.

    // Build the segment tree from the testbed
    vector<long long> tree(4 * n); // Allocate space for the segment tree
    build(testbed, tree, 0, 0, n - 1); // Build the segment tree from the original array

    cout << "****************************" << endl << endl;
    cout << "**Test Range Sum**" << endl;

    // Example range_sum queries
    cout << "Sum from index 0 to 999: " << range_sum(tree, 0, 0, n - 1, 0, n - 1) << endl; // Should print 1000. Sum from index 0 to 999: 1000
    cout << "Sum from index 100 to 199: " << range_sum(tree, 0, 0, n - 1, 100, 199) << endl; // Should print 100. Sum from index 100 to 199: 100

    cout << endl;
    cout << "**Test Point Update**" << endl;

    // Update a point
    point_update(tree, 0, 0, n - 1, 500, 42); // Update index 500 to 42
    cout << "Sum from index 500 to 500 after update: " << range_sum(tree, 0, 0, n - 1, 500, 500) << endl; // Should print 42. Sum from index 500 to 500 after update: 42

    cout << "****************************" << endl << endl;

    return ;
}   