#include "lazy_segment_tree.h"
#include <iostream>
#include <vector>

using namespace std;

namespace lazytree {
// Build the segment tree from the original array
void build(vector<int>& orig_arr, vector<int>& tree, int curr_node, int curr_left, int curr_right) {
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

// Propagate the pending updates from the lazy array down through the tree
void propagate(vector<int>& tree, vector<int>& lazy, int curr_node, int curr_left, int curr_right) {
    // If there are pending updates for the current node
    if (lazy[curr_node] != 0) {
        // Update the current node's value based on the pending update
        tree[curr_node] += (curr_right - curr_left + 1) * lazy[curr_node];
        // If the current node is not a leaf node, propagate the update to its children
        if (curr_left != curr_right) {
            lazy[2 * curr_node + 1] += lazy[curr_node]; // Propagate to lazy left child
            lazy[2 * curr_node + 2] += lazy[curr_node]; // Propagate to lazy right child
        }
        // Clear the pending update for the current node
        lazy[curr_node] = 0;
    }
}

// Update the segment tree for a given range [update_left, update_right] with a new value
void range_update(vector<int>& tree, vector<int>& lazy, int curr_node, int curr_left, int curr_right, int update_left, int update_right, int value) {
    // If the current segment is completely outside the update range, return
    if (curr_right < update_left || curr_left > update_right) {
        return;
    }
    // Propagate any pending updates for the current node
    propagate(tree, lazy, curr_node, curr_left, curr_right);

    // If the current segment is completely inside the update range, apply the update and propagate to children
    if (curr_left >= update_left && curr_right <= update_right) {
        tree[curr_node] += (curr_right - curr_left + 1) * value;
        if (curr_left != curr_right) {
            lazy[2 * curr_node + 1] += value; // Propagate to lazy left child
            lazy[2 * curr_node + 2] += value; // Propagate to lazy right child
        }
        return;
    }
    // If the current segment is partially inside the update range, recursively update the left and right subtrees
    int mid = curr_left + (curr_right - curr_left) / 2;
    range_update(tree, lazy, 2 * curr_node + 1, curr_left, mid, update_left, update_right, value);
    range_update(tree, lazy, 2 * curr_node + 2, mid + 1, curr_right, update_left, update_right, value);
    // After updating the children, update the current node's value based on the children's values
    tree[curr_node] = tree[2 * curr_node + 1] + tree[2 * curr_node + 2];
}

// Query the sum of values in a given range [query_left, query_right]
long long range_sum(vector<int>& tree, vector<int>& lazy, int curr_node, int curr_left, int curr_right, int query_left, int query_right) {
    // If the current segment is completely outside the query range, return 0
    if (curr_right < query_left || curr_left > query_right) {
        return 0;
    }
    // Propagate any pending updates for the current node    
    propagate(tree, lazy, curr_node, curr_left, curr_right);
    // If the current segment is completely inside the query range, return the value of the current node
    if (curr_left >= query_left && curr_right <= query_right) {
        return tree[curr_node];
    }
    // Otherwise, recursively query the left and right subtrees
    int mid = curr_left + (curr_right - curr_left) / 2;
    long long left_sum = range_sum(tree, lazy, 2 * curr_node + 1, curr_left, mid, query_left, query_right);
    long long right_sum = range_sum(tree, lazy, 2 * curr_node + 2, mid + 1, curr_right, query_left, query_right);
    return left_sum + right_sum;
}

// Update the value of a single element in the segment tree with lazy propagation
void point_update(vector<int>& tree, vector<int>& lazy, int curr_node, int curr_left, int curr_right, int index, int new_value) {
    // If the current segment is completely outside the index, return
    if (curr_right < index || curr_left > index) {
        return;
    }
    // If the current segment is a single element, update its value
    if (curr_left == curr_right) {
        tree[curr_node] = new_value;
        return;
    }
    // Otherwise, recursively update the left and right subtrees
    int mid = curr_left + (curr_right - curr_left) / 2;
    point_update(tree, lazy, 2 * curr_node + 1, curr_left, mid, index, new_value);
    point_update(tree, lazy, 2 * curr_node + 2, mid + 1, curr_right, index, new_value);
    // After updating the children, update the current node's value based on the children's values
    tree[curr_node] = tree[2 * curr_node + 1] + tree[2 * curr_node + 2];

}
}

void lazytree_test() {
    using namespace lazytree;
    
    // Create a testbed with 1000 elements initialized to 1
    int n = 1000;
    vector<int> orig_arr(n, 1);
    vector<int> tree(4 * n, 0); // Segment tree array
    vector<int> lazy(4 * n, 0); // Lazy propagation array

    // Build the segment tree from the original array
    build(orig_arr, tree, 0, 0, n - 1);

    // Test range sum query
    cout << endl;
    cout << "************************************" << endl << endl;

    cout << "**Test Range Sum**" << endl;
    cout << "Range sum [0, 999] should be 1000: " << range_sum(tree, lazy, 0, 0, n - 1, 0, n - 1) << endl;
    cout << endl;

    cout << "**Test Range Update add 5 to first 100 elements**" << endl;
    range_update(tree, lazy, 0, 0, n - 1, 0, 99, 5);
    cout << "After range update [0,99] +5, sum [0,99] should be 600: " << range_sum(tree, lazy, 0, 0, n - 1, 0, 99) << endl;
    cout << "After range update [0,99] +5, sum [100,999] should still be 900: " << range_sum(tree, lazy, 0, 0, n - 1, 100, n - 1) << endl;
    cout << endl;

    cout << "************************************" << endl << endl;

    return ;
}